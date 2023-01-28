#include <time.h>
#include "basic_operations.h"
#include "utils.h"
#include "basic_interface.h"

void clean_test_tree_header(struct header_of_tree* header);
enum status_of_read_operation read_tree_header(struct header_of_tree *header, FILE *file);
enum status_of_basic_operation swap_tuple_position(FILE *file, uint64_t pos_to, uint64_t pos_from, size_t tuple_size) {
    uint32_t *types;
    size_t size;
    get_types(file, &types, &size);
    if (pos_from != pos_to) {
        fseek(file, pos_from, SEEK_SET);
        void *buffer = malloc_test(tuple_size);
        read_file(file, buffer, tuple_size);
        fseek(file, pos_to, SEEK_SET);
        write_file(file, buffer, tuple_size);
        free_test(buffer);
        fseek(file, 0, SEEK_SET);
        struct header_of_tree *header = malloc_test(sizeof(struct header_of_tree));
        read_tree_header(header, file);
        uint64_t id;
        struct my_tuple *tpl;
        enum status_of_basic_operation st = offset_to_id(file, &id, pos_from);
        if (st == IS_INVALID) {
            fseek(file, pos_from, SEEK_SET);
            read_string_tuple(file, &tpl, size);
            union header_tuple *temp_header = malloc_test(sizeof(union header_tuple));
            if (tpl->header.buff != 0) {
                fseek(file, tpl -> header.buff, SEEK_SET);
                read_file(file, temp_header, sizeof(union header_tuple));
                temp_header -> original = pos_to;
                fseek(file, tpl->header.buff, SEEK_SET);
                write_file(file, temp_header, sizeof(union header_tuple));
            }
            fseek(file, tpl -> header.original, SEEK_SET);
            read_file(file, temp_header, sizeof(union header_tuple));
            if (temp_header -> buff == pos_from) {
                temp_header -> buff = pos_to;
                fseek(file, tpl -> header.original, SEEK_SET);
                write_file(file, temp_header, sizeof(union header_tuple));
            } else {
                struct my_tuple *next;
                fseek(file, tpl -> header.original, SEEK_SET);
                read_basic_tuple(file, &next, size);
                for (size_t iter = 0; iter < size; iter++) {
                    if (types[iter] == STRING_TYPE && next->data[iter] == pos_from) {
                        next->data[iter] = pos_to;
                        break;
                    }
                }
                fseek(file, (tpl->header.original), SEEK_SET);
                write_tuple(file, next, get_real_info_size(size));
                free_test_tuple(next);
            }
            free_test(temp_header);
        } else {
            fseek(file, pos_from, SEEK_SET);
            read_basic_tuple(file, &tpl, size);
            link_strings_to_tuple(file, tpl, pos_to);
            header->id_sequence[id] = pos_to;
            write_header_of_tree(file, header);
        }
        free_test_tuple(tpl);
        clean_test_tree_header(header);
    }
    free_test(types);
    return IS_OK;
}

enum status_of_basic_operation delete_last_tuple(FILE *file, size_t full_tuple_size) {
    fseek(file, full_tuple_size, SEEK_END);
    int fd = fileno(file);
    return ftruncate(fd, ftell(file));
}

enum status_of_basic_operation swap_last_tuple_to(FILE *file, uint64_t pos_to, size_t tuple_size){
    uint32_t *types;
    size_t size;
    get_types(file, &types, &size);

    fseek(file, (long) -(get_real_info_size(size) + sizeof(union header_tuple)), SEEK_END);
    uint64_t pos_from = ftell(file);

    free_test(types);
    enum status_of_basic_operation status = swap_tuple_position(file, pos_to, pos_from, tuple_size);
    ftruncate(fileno(file), (long) pos_from);
    return status;
}

enum status_of_basic_operation insert_new_tuple(FILE *file, struct my_tuple *tuple, size_t full_tuple_size, uint64_t *tuple_pos) {
    fseek(file, 0, SEEK_END);
    *tuple_pos = ftell(file);
    int fd = fileno(file);
    ftruncate(fd, ftell(file) + full_tuple_size);
    enum status_of_write_operation status = write_tuple(file, tuple, full_tuple_size - sizeof(union header_tuple));
    return status == IS_WRITE_OK ? IS_OK : IS_INVALID;
}


enum status_of_basic_operation insert_string_tuple(FILE *file, char *string, size_t tuple_size, uint64_t *str_pos) {
    size_t len = strlen(string);
    size_t count = len / tuple_size + (len % tuple_size ? 1 : 0);
    struct my_tuple *temp_tuple = malloc_test(sizeof(struct my_tuple));
    char *temp_tuple_content = string;
    size_t pos = (size_t) ftell(file);
    uint64_t fake_pos;
    fseek(file, 0, SEEK_END);
    *str_pos = ftell(file);
    for (size_t iter = 0; count > iter; iter++) {
        if (count - 1 == iter) {
            temp_tuple->header.buff = 0;
        } else {
            temp_tuple->header.buff = pos + (tuple_size + sizeof(union header_tuple)) * (iter + 1);
        }
        if (0 == iter) {
            temp_tuple->header.original = 0;
        } else {
            temp_tuple->header.original = pos + (tuple_size + sizeof(union header_tuple)) * (iter - 1);
        }
        temp_tuple->data = (uint64_t *) (temp_tuple_content + tuple_size * iter);
        insert_new_tuple(file, temp_tuple, tuple_size + sizeof(union header_tuple), &fake_pos);
    }
    free_test(temp_tuple);
    return 0;
}

enum status_of_basic_operation link_strings_to_tuple(FILE *file, struct my_tuple *tpl, uint64_t tpl_offset) {
    uint32_t *types;
    size_t size;
    get_types(file, &types, &size);
    struct my_tuple *str;

    for (uint64_t iter = 0; iter < size; iter++) {
        if (types[iter] == STRING_TYPE) {
            fseek(file, tpl->data[iter], SEEK_SET);
            read_string_tuple(file, &str, size);
            str->header.original = tpl_offset;
            fseek(file, tpl->data[iter], SEEK_SET);
            write_tuple(file, str, size);
            free_test_tuple(str);
        }
    }
    free_test(types);
    return IS_OK;
}

void get_types(FILE *file, uint32_t **types, size_t *size) {
    fseek(file, 0, SEEK_SET);
    struct header_of_tree *header = malloc_test(sizeof(struct header_of_tree));
    read_tree_header(header, file);
    uint32_t *temp_types = malloc_test(header->subheader->pattern_size * sizeof(uint32_t));
    for (size_t iter = 0; iter < header->subheader->pattern_size; iter++) {
        temp_types[iter] = header->pattern[iter]->header->type;
    }
    *types = temp_types;
    *size = header->subheader->pattern_size;
    clean_test_tree_header(header);
}


size_t append_to_id_array(FILE *file, uint64_t offset) {

    size_t id;
    struct header_of_tree *header = malloc_test(sizeof(struct header_of_tree));
    read_tree_header(header, file);
    uint64_t from = ftell(file);
   uint64_t real_tuple_size = get_info_array_size(header->subheader->pattern_size, header->subheader->cur_id);

    if (!((header->subheader->cur_id + 1) % real_tuple_size)){
        fseek(file, 0, SEEK_END);
        uint64_t cur_end = ftell(file);
        ftruncate(fileno(file), cur_end + get_real_info_size(header->subheader->pattern_size) + sizeof(union header_tuple));

        swap_tuple_position(file, cur_end, from, get_real_info_size(header->subheader->pattern_size));



        clean_test_tree_header(header);
        header = malloc_test(sizeof(struct header_of_tree));
        read_tree_header(header, file);
    }


    header->id_sequence[header->subheader->cur_id] = offset;
    header->subheader->cur_id++;
    id = header->subheader->cur_id - 1;

    fseek(file, 0, SEEK_SET);
    if (write_header_of_tree(file, header) != IS_WRITE_OK){
        printf("WRITE ERROR\n");
    }

    clean_test_tree_header(header);

    return id;
}


enum status_of_basic_operation remove_from_id_array(FILE *file, uint64_t id, uint64_t *offset) {
    fseek(file, 0, SEEK_SET);
    struct header_of_tree *header = malloc_test(sizeof(struct header_of_tree));
    read_tree_header(header, file);
    if (header->id_sequence[id] == 0) {
        clean_test_tree_header(header);
        return IS_INVALID;
    }
    else {
        *offset = header->id_sequence[id];
        if (header->subheader->cur_id - 1 == id) {
            header->subheader->cur_id--;
        }
        header->id_sequence[id] = 0;
        write_header_of_tree(file, header);
        clean_test_tree_header(header);
        return IS_OK;
    }
}

enum status_of_basic_operation id_to_offset(FILE *file, uint64_t id, uint64_t *offset) {
    fseek(file, 0, SEEK_SET);
    struct header_of_tree *header = malloc_test(sizeof(struct header_of_tree));
    read_tree_header(header, file);
    if (header->id_sequence[id] == NULL_VALUE){
        clean_test_tree_header(header);
        return IS_INVALID;
    }else{
        *offset = header->id_sequence[id];
        clean_test_tree_header(header);
        return IS_OK;
    }

}

enum status_of_basic_operation offset_to_id(FILE *file, uint64_t *id, uint64_t offset) {

    struct header_of_tree *header = malloc_test(sizeof(struct header_of_tree));
    read_tree_header(header, file);

    struct my_tuple *tpl;
    fseek(file, offset, SEEK_SET);
    read_basic_tuple(file, &tpl, header->subheader->pattern_size);



    if (header->id_sequence[tpl->header.previous] == offset) {
        *id = tpl->header.previous;
        clean_test_tree_header(header);
        free_test_tuple(tpl);
        return IS_OK;
    } else{
        clean_test_tree_header(header);
        free_test_tuple(tpl);
        return IS_INVALID;
    }

}

enum status_of_basic_operation change_string_tuple(FILE *file, uint64_t offset, char *new_string, uint64_t size) {
    struct my_tuple *cur_tuple = NULL;
    uint64_t len = strlen(new_string);
    uint64_t old_offset = offset;
    do {
        offset = old_offset;
        fseek(file, offset, SEEK_SET);
        read_string_tuple(file, &cur_tuple, size);
        fseek(file, offset, SEEK_SET);
        cur_tuple->data = (uint64_t *) (new_string);
        new_string = new_string + size;
        write_tuple(file, cur_tuple, size);
        old_offset = cur_tuple->header.buff;
        len -= size;
    } while (cur_tuple->header.buff && len > 0);
    uint64_t fpos;
    if (len > 0) {
        insert_string_tuple(file, new_string, size, &fpos);
        cur_tuple->header.buff = fpos;
        fseek(file, offset, SEEK_SET);
        write_tuple(file, cur_tuple, size);
    }
    return IS_OK;
}




