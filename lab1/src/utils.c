#include <time.h>
#include "utils.h"
#include "basic_operations.h"

size_t get_real_info_size(uint64_t pattern_size) {
    return pattern_size * SINGLE_TUPLE_VALUE_SIZE < MINIMAL_TUPLE_SIZE ? MINIMAL_TUPLE_SIZE : pattern_size * SINGLE_TUPLE_VALUE_SIZE;
}

static uint64_t get_max(uint64_t n1, uint64_t n2) {
    if (n1 > n2) return n1;
    return n2;
}

size_t get_info_array_size(uint64_t pattern_size, uint64_t cur_id) {
    size_t real_tuple_size = get_real_info_size(pattern_size) + sizeof(union header_tuple);
    if (cur_id == 0) cur_id++;
    size_t whole = (cur_id * OFFSET_VALUE_SIZE / real_tuple_size);
    size_t frac = (cur_id * OFFSET_VALUE_SIZE % real_tuple_size ? 1: 0);
    size_t value = get_max( (frac + whole) * real_tuple_size / OFFSET_VALUE_SIZE, MIN_ID_ARRAY_SIZE * real_tuple_size / OFFSET_VALUE_SIZE);
    return value;
}


static enum status_of_read_operation read_tree_subheader(struct tree_subheader *header, FILE *file) {
    enum status_of_read_operation code = read_file(file, header, sizeof(struct tree_subheader));
    return code;
}

static enum status_of_read_operation read_key(struct key *key, FILE *file) {
    struct key_header *header = malloc_test(sizeof(struct key_header));
    enum status_of_read_operation code = read_file(file, header, sizeof(struct key_header));
    key -> header = header;

    char *key_value = (char *) malloc_test(
            header -> size / FILE_GRANULARITY + (header -> size % FILE_GRANULARITY ? FILE_GRANULARITY : 0));
    code |= read_file(file, key_value, header -> size);
    key -> key_value = key_value;

    return code;
}

enum status_of_read_operation read_tree_header(struct header_of_tree *header, FILE *file) {
    fseek(file, 0, SEEK_SET);
    struct tree_subheader *subheader = malloc_test(sizeof(struct tree_subheader));
    enum status_of_read_operation code = read_tree_subheader(subheader, file);
    header -> subheader = subheader;

    struct key **array_of_key = malloc_test(sizeof(struct key *) * subheader -> pattern_size);
    header -> pattern = array_of_key;
    for (size_t iter = subheader -> pattern_size; iter-- > 0; array_of_key++) {
        struct key *element_pattern = malloc_test(sizeof(struct key));
        code |= read_key(element_pattern, file);
        *array_of_key = element_pattern;
    }

    size_t real_id_array_size = get_info_array_size(header -> subheader -> pattern_size, header -> subheader -> cur_id);
    uint64_t *id_array = (uint64_t *) malloc_test(real_id_array_size * sizeof(uint64_t));
    header -> id_sequence = id_array;
    code |= read_file(file, id_array, real_id_array_size * sizeof(uint64_t));

    return code;
}

enum status_of_read_operation read_basic_tuple(FILE *file, struct my_tuple **tuple, uint64_t pattern_size) {
    union header_tuple *header = malloc_test(sizeof(union header_tuple));
    enum status_of_read_operation code = read_file(file, header, sizeof(union header_tuple));
    struct my_tuple *temp_tuple = malloc_test(sizeof(struct my_tuple));
    temp_tuple -> header = *header;
    free_test(header);


    uint64_t *data = malloc_test(get_real_info_size(pattern_size));
    code |= read_file(file, data, get_real_info_size(pattern_size));
    temp_tuple -> data = data;

    *tuple = temp_tuple;

    return code;
}

enum status_of_read_operation read_string_tuple(FILE *file, struct my_tuple **tuple, uint64_t pattern_size) {
    union header_tuple *header = malloc_test(sizeof(union header_tuple));
    enum status_of_read_operation code = read_file(file, header, sizeof(union header_tuple));
    struct my_tuple *temp_tuple = malloc_test(sizeof(struct my_tuple));
    temp_tuple -> header = *header;
    free_test(header);
    uint64_t *data = (uint64_t *) malloc_test(get_real_info_size(pattern_size));
    code |= read_file(file, data, get_real_info_size(pattern_size));
    temp_tuple -> data = data;
    *tuple = temp_tuple;
    return code;
}

static size_t how_long_string_is(FILE *file, uint64_t offset) {
    fseek(file, offset, SEEK_SET);
    size_t len = 1;
    union header_tuple *temp_header = malloc_test(sizeof(union header_tuple));
    read_file(file, temp_header, sizeof(union header_tuple));
    while (temp_header -> buff) {
        fseek(file, temp_header -> buff, SEEK_SET);
        read_file(file, temp_header, sizeof(union header_tuple));
        len++;
    }
    free_test(temp_header);
    return len;
}

enum status_of_read_operation read_string_from_tuple(FILE *file, char **string, uint64_t pattern_size, uint64_t offset) {
    size_t str_len = how_long_string_is(file, offset);
    size_t rts = get_real_info_size(pattern_size);
    *string = malloc_test(str_len * rts);
    struct my_tuple *temp_tuple;
    for (size_t iter = 0; iter < str_len; iter++) {
        fseek(file, offset, SEEK_SET);
        read_string_tuple(file, &temp_tuple, pattern_size);
        offset = temp_tuple -> header.buff;
        strncpy((*string) + rts * iter, (char *) temp_tuple -> data, rts);
        free_test_tuple(temp_tuple);
    }
    return 0;
}

enum status_of_write_operation write_tree_subheader(FILE *file, struct tree_subheader *subheader) {
    enum status_of_write_operation code = write_file(file, subheader, sizeof(struct tree_subheader));
    return code;
}

enum status_of_write_operation write_pattern(FILE *file, struct key **pattern, size_t pattern_size) {
    enum status_of_write_operation code = NULL_VALUE;
    for (; pattern_size-- > 0; pattern++) {
        code |= write_file(file, (*pattern) -> header, sizeof(struct key_header));
        code |= write_file(file, (*pattern) -> key_value, (*pattern) -> header -> size);
    }
    return code;
}

enum status_of_write_operation write_id_sequence(FILE *file, uint64_t *id_sequence, size_t size) {
    enum status_of_write_operation code = write_file(file, id_sequence, size);
    return code;
}

enum status_of_write_operation write_header_of_tree(FILE *file, struct header_of_tree *header) {
    fseek(file, 0, SEEK_SET);
    size_t pattern_size = header -> subheader -> pattern_size;

    enum status_of_write_operation code = write_tree_subheader(file, header -> subheader);
    if (code != IS_WRITE_OK){
        printf("WRITE ERROR\n");
    }

    fseek(file, sizeof(struct tree_subheader), SEEK_SET);
    code |= write_pattern(file, header -> pattern, pattern_size);
    size_t real_id_array_size = get_info_array_size(header -> subheader -> pattern_size, header -> subheader -> cur_id);
    code |= write_id_sequence(file, header -> id_sequence, real_id_array_size * sizeof(uint64_t));


    if (code == IS_INVALID){
        printf("WRITE ERROR\n");
    }
    return code;
}
void clean_test_tree_header(struct header_of_tree* header){
    for (size_t iter = 0; iter < header -> subheader -> pattern_size; iter++){
        free_test(header -> pattern[iter] -> key_value);
        free_test(header -> pattern[iter] -> header);
        free_test(header -> pattern[iter]);
    }
    free_test(header -> pattern);
    free_test(header -> id_sequence);
    free_test(header -> subheader);
    free_test(header);
}

enum status_of_write_operation create_empty_file(FILE *file, char **pattern, uint32_t *types, size_t pattern_size, size_t *key_sizes) {
    fseek(file, 0, SEEK_SET);
    struct header_of_tree *header = malloc_test(sizeof(struct header_of_tree));
    gen_empty_tree_header(pattern, types, pattern_size, key_sizes, header);
    enum status_of_write_operation code = write_header_of_tree(file, header);
    clean_test_tree_header(header);
    return code;
}

enum status_of_open_operation open_file_anyway(FILE **file, char *filename) {
    enum status_of_open_operation code = open_existing_file(filename, file);
    if (code) {
        code = create_new_file(filename, file);
    }
    return code;
}

enum status_of_write_operation write_tuple(FILE *file, struct my_tuple *tuple, size_t tuple_size) {
    union header_tuple *header_tuple = malloc_test(sizeof(union header_tuple));
    *header_tuple = tuple -> header;
    enum status_of_write_operation code = write_file(file, header_tuple, sizeof(union header_tuple));
    free_test(header_tuple);
    code |= write_file(file, tuple -> data, tuple_size);
    return code;
}

void free_test_tuple(struct my_tuple* tuple){
    free_test(tuple -> data);
    free_test(tuple);
}


struct map_data {
    void *ptr;
    size_t size;
};

struct map_data map[100000] = {0};
size_t glob_size = 0;
size_t iter = 0;

void *malloc_test(size_t size){
    void *ptr = malloc(size);
    return ptr;
}

void free_test(void *ptr){
    free(ptr);
}

void print_ram() {
    printf("'%zu'\n", glob_size);
}
