#ifndef LAB1_UTILS_H
#define LAB1_UTILS_H
#include <stdlib.h>

#include "structures.h"
#include "header_gen.h"
#include "basic_file_manager.h"
#include "basic_interface.h"

#define FILE_GRANULARITY 4
#define MINIMAL_TUPLE_SIZE 256
#define SINGLE_TUPLE_VALUE_SIZE 8
#define OFFSET_VALUE_SIZE 8
#define MIN_ID_ARRAY_SIZE 1
#define NULL_VALUE 0
#define INPUT_LINE_SIZE 1024
#define BIG_ENDIAN_SIGNATURE 0xFFFE
#define PRINT_ID_ARRAY_LEN 16
#define BOOLEAN_TYPE 0
#define INTEGER_TYPE 1
#define FLOAT_TYPE 2
#define STRING_TYPE 3

enum status_of_write_operation write_tuple(FILE *file, struct my_tuple *my_tuple, size_t tuple_size);
enum status_of_open_operation open_file_anyway(FILE **file, char *filename);
enum status_of_write_operation create_empty_file(FILE *file, char **pattern, uint32_t *types, size_t pattern_size, size_t *key_sizes);
enum status_of_write_operation write_header_of_tree(FILE *file, struct header_of_tree *header);
enum status_of_read_operation read_string_tuple(FILE *file, struct my_tuple **my_tuple, uint64_t pattern_size);
enum status_of_read_operation read_basic_tuple(FILE *file, struct my_tuple **my_tuple, uint64_t pattern_size);
enum status_of_read_operation read_header_of_tree(struct header_of_tree *header, FILE *file);
enum status_of_read_operation read_string_from_tuple(FILE *file, char **string, uint64_t pattern_size, uint64_t offset);
enum status_of_write_operation write_tuple(FILE *file, struct my_tuple *my_tuple, size_t tuple_size);
void free_test_header_of_tree(struct header_of_tree* header);
void free_test_tuple(struct my_tuple* my_tuple);
void *malloc_test(size_t size);
void free_test(void *ptr);
void print_ram();
size_t get_info_array_size(uint64_t pattern_size, uint64_t cur_id);
size_t get_real_info_size(uint64_t pattern_size);

#endif //LAB1_UTILS_H
