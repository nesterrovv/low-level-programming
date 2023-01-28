#ifndef LAB1_BASIC_INTERFACE_H
#define LAB1_BASIC_INTERFACE_H

#include <stdio.h>
#include "basic_operations.h"

struct result_list_tuple {
    struct result_list_tuple *original;
    struct my_tuple *value;
    uint64_t id;
};

size_t add_tuple(FILE *file, uint64_t *fields, uint64_t parent_id);
enum status_of_basic_operation remove_tuple(FILE *file, uint64_t id, uint8_t str_flag);
enum status_of_basic_operation get_tuple(FILE *file, uint64_t **fields, uint64_t id);
enum status_of_basic_operation find_by_field(FILE *file, uint64_t field_number, uint64_t *condition, struct result_list_tuple **result);
enum status_of_basic_operation find_by_parent(FILE *file, uint64_t parent_id, struct result_list_tuple **result);
enum status_of_basic_operation update_tuple(FILE *file, uint64_t field_number, uint64_t *new_value, uint64_t id);
void print_tree_header_from_file(FILE *file);
void print_tuple_array_from_file(FILE *file);

#endif //LAB1_BASIC_INTERFACE_H

