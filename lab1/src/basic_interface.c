#include <time.h>
#include "basic_interface.h"

void clean_test_tree_header(struct header_of_tree* header);
enum status_of_basic_operation insert_string_tuple(FILE *file, char *string, size_t tuple_size, uint64_t *str_pos);
enum status_of_read_operation read_tree_header(struct header_of_tree *header, FILE *file);
size_t add_tuple(FILE *file, uint64_t *fields, uint64_t parent_id) {

    uint32_t *types;
    size_t size;
    get_types(file, &types, &size);
    struct header_of_tree* header = malloc(sizeof(struct header_of_tree));
    read_tree_header(header, file);

    struct my_tuple* new_tuple = malloc_test(sizeof(struct my_tuple));
    union header_tuple new_tuple_header = {.next = parent_id, .previous = header->subheader->cur_id};

    new_tuple->header = new_tuple_header;
    new_tuple->data = malloc_test(get_real_info_size(size));
    uint64_t link;


    for (size_t iter = 0; iter < size; iter++) {
        if (types[iter] == STRING_TYPE) {
            insert_string_tuple(file, fields[iter], get_real_info_size(size), &link);
            new_tuple->data[iter] = link;
        } else {
            new_tuple->data[iter] = (uint64_t) fields[iter];
        }
    }


    size_t full_tuple_size = sizeof(union header_tuple) + get_real_info_size(size);

    enum status_of_basic_operation status = insert_new_tuple(file, new_tuple, full_tuple_size, &link);


    link_strings_to_tuple(file, new_tuple, link);

    size_t id = append_to_id_array(file, link);



    free_test_tuple(new_tuple);
    free_test(types);
    clean_test_tree_header(header);

    return status;
}

enum status_of_basic_operation get_tuple(FILE *file, uint64_t **fields, uint64_t id) {
    uint64_t offset;
    if (id_to_offset(file, id, &offset) == IS_INVALID) return IS_INVALID;
    struct my_tuple *cur_tuple;
    uint32_t *types;
    size_t size;
    get_types(file, &types, &size);
    fseek(file, offset, SEEK_SET);
    read_basic_tuple(file, &cur_tuple, (uint64_t) size);
    *fields = malloc_test(sizeof(uint64_t) * size);
    for (size_t iter = 0; iter < size; iter++) {
        if (types[iter] == STRING_TYPE) {
            char *s;
            read_string_from_tuple(file, &s, size, cur_tuple->data[iter]);
            memcpy(&(*fields)[iter], &s, sizeof(s));
        } else {
            (*fields)[iter] = cur_tuple->data[iter];
        }

    }
    free_test_tuple(cur_tuple);
    free_test(types);
    return IS_OK;
}

enum status_of_basic_operation remove_tuple(FILE *file, uint64_t id, uint8_t str_flag) {
    uint32_t *types;
    size_t size;
    get_types(file, &types, &size);
//    printf("%lu\n", id);

    if (!str_flag) {
        uint64_t offset;
        if (remove_from_id_array(file, id, &offset) == IS_INVALID) {
            // invalid id
            return IS_INVALID;
        }

        for (size_t field_num = 0; field_num < size; field_num++) {
            if (types[field_num] == STRING_TYPE) {
                struct my_tuple *tpl;
                fseek(file, (long) offset, SEEK_SET);
                read_basic_tuple(file, &tpl, size);
                remove_tuple(file, tpl->data[field_num], 1);
                free_test_tuple(tpl);
            }
        }


        swap_last_tuple_to(file, offset, get_real_info_size(size));

        struct result_list_tuple *children = NULL;
        find_by_parent(file, id, &children);
        while (children != NULL) {
            remove_tuple(file, children->id, 0);
            children = children->original;
        }
    } else {
        struct my_tuple *str_tpl;
        while (id != NULL_VALUE) {
            fseek(file, id, SEEK_SET);
            read_string_tuple(file, &str_tpl, size);
            swap_last_tuple_to(file, id, get_real_info_size(size) + sizeof(union header_tuple));
            id = str_tpl->header.buff;
            free_test_tuple(str_tpl);
        }

    }
    free_test(types);
    return IS_OK;
}

static void append_to_result_list(struct my_tuple **tuple_to_add, uint64_t id, struct result_list_tuple **result) {
    if ((*result) == NULL) {
        *result = malloc_test(sizeof(struct result_list_tuple));
        (*result)->original = NULL;
    } else {
        struct result_list_tuple *new_result = malloc_test(sizeof(struct result_list_tuple));
        new_result->original = *result;
        *result = new_result;
    }
    (*result)->value = *tuple_to_add;
    (*result)->id = id;
    *tuple_to_add = malloc_test(sizeof(struct my_tuple));
}

enum status_of_basic_operation find_by_field(FILE *file, uint64_t field_number, uint64_t *condition, struct result_list_tuple **result) {
    uint32_t *types;
    size_t size;
    get_types(file, &types, &size);
    uint64_t type = types[field_number];
    struct header_of_tree *header = malloc_test(sizeof(struct header_of_tree));
    read_tree_header(header, file);
    struct my_tuple *cur_tuple = NULL;
    for (size_t i = 0; i < header->subheader->cur_id; i++) {
        if (header->id_sequence[i] == NULL_VALUE) continue;
        fseek(file, header->id_sequence[i], SEEK_SET);
        read_basic_tuple(file, &cur_tuple, size);
        if (type == STRING_TYPE) {
            char *s;
            read_string_from_tuple(file, &s, size, cur_tuple->data[field_number]);
            if (!strcmp(s, (char *) condition)) {
                append_to_result_list(&cur_tuple, i, result);
            }
            free_test(s);
        } else {
            if (cur_tuple->data[field_number] == *condition) {
                append_to_result_list(&cur_tuple, i, result);
            }
        }

    }
    clean_test_tree_header(header);
    free_test(types);
    return 0;
}

enum status_of_basic_operation find_by_parent(FILE *file, uint64_t parent_id, struct result_list_tuple **result) {
    struct header_of_tree *header = malloc_test(sizeof(struct header_of_tree));
    read_tree_header(header, file);
    struct my_tuple *cur_tuple = malloc_test(sizeof(struct my_tuple));
    for (size_t i = 0; i < header->subheader->cur_id; i++) {
        if (header->id_sequence[i] == NULL_VALUE) continue;
        fseek(file, header->id_sequence[i], SEEK_SET);
        read_basic_tuple(file, &cur_tuple, header->subheader->pattern_size);
        if (cur_tuple->header.next == parent_id) {
            append_to_result_list(&cur_tuple, i, result);
        }

    }
    clean_test_tree_header(header);
    return 0;
}

enum status_of_basic_operation update_tuple(FILE *file, uint64_t field_number, uint64_t *new_value, uint64_t id) {
    uint32_t *types;
    size_t size;
    get_types(file, &types, &size);
    uint64_t type = types[field_number];
    uint64_t offset;
    id_to_offset(file, id, &offset);
    struct my_tuple *cur_tuple;
    fseek(file, offset, SEEK_SET);
    read_basic_tuple(file, &cur_tuple, size);
    if (type == STRING_TYPE) {
        change_string_tuple(file, cur_tuple->data[field_number], (char *) new_value, get_real_info_size(size));
    } else {
        memcpy(&(cur_tuple->data[field_number]), new_value, sizeof(*new_value));
        fseek(file, offset, SEEK_SET);
        write_tuple(file, cur_tuple, get_real_info_size(size));
    }
    free_test_tuple(cur_tuple);
    free_test(types);
    return 0;
}

void print_tuple_array_from_file(FILE *file) {
    struct header_of_tree header;
    read_tree_header(&header, file);
    uint32_t *types;
    size_t size;
    get_types(file, &types, &size);
    struct my_tuple *cur_tuple;
    for (size_t i = 0; i < header.subheader->cur_id; i++) {
        if (header.id_sequence[i] == NULL_VALUE) continue;
        fseek(file, header.id_sequence[i], SEEK_SET);
        read_basic_tuple(file, &cur_tuple, size);
        printf("+++ CURRENT TUPLE %3zu +++\n", i);
        for (size_t iter = 0; iter < size; iter++) {
            if (types[iter] == STRING_TYPE) {
                char *s;
                read_string_from_tuple(file, &s, header.subheader->pattern_size, cur_tuple->data[iter]);
                printf("%-20s %s\n", header.pattern[iter]->key_value, s);
                free_test(s);
            } else if (types[iter] == INTEGER_TYPE || types[iter] == BOOLEAN_TYPE) {
                printf("%-20s %lu\n", header.pattern[iter]->key_value, cur_tuple->data[iter]);
            } else if (types[iter] == FLOAT_TYPE) {
                double res;
                memcpy(&res, &(cur_tuple->data[iter]), sizeof(cur_tuple->data[iter]));
                printf("%-20s %.6f\n", header.pattern[iter]->key_value, res);
            }
        }
        free_test_tuple(cur_tuple);
    }
    printf("++++++++++++++++\n");
    free_test(types);

}

void print_tree_header_from_file(FILE *file) {
    struct header_of_tree* header = malloc_test(sizeof(struct header_of_tree));
    read_tree_header(header, file);
    printf("+++ CHILDREN HEADER DESCRIPTION +++\n");
    printf("%-20s%ld\n", "Current ID: ",        header -> subheader -> cur_id);
    printf("%-20s%ld\n", "Root Offset: ",       header -> subheader -> root_offset);
    printf("%-20s%ld\n", "Pattern Size: ",      header -> subheader -> pattern_size);
    printf("%-20s%ld\n", "First Sequence: ",    header -> subheader -> sequence_first);
    printf("%-20s%ld\n", "Second Sequence: ",   header -> subheader -> sequence_second);
    printf("%-20s%ld\n", "ASCII Signature: ",   header -> subheader -> signature_of_ASCII);
    printf("--- PATTERN ---\n");
    for (size_t iter = 0; iter < header->subheader->pattern_size; iter++) {
        printf("Key %3d [Type %3d]: %s\n",
               header->pattern[iter]->header->size, header->pattern[iter]->header->type,
               header->pattern[iter]->key_value);
    }
    clean_test_tree_header(header);
}