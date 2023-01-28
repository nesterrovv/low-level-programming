#ifndef LAB1_BASIC_OPERATIONS_H
#define LAB1_BASIC_OPERATIONS_H

#include <unistd.h>
#include <string.h>
#include "utils.h"

enum status_of_basic_operation {
    IS_OK = 0,
    IS_INVALID
};

enum status_of_basic_operation swap_last_tuple_to(FILE *file, uint64_t pos_to, size_t tuple_size);
enum status_of_basic_operation insert_string_tuple(FILE *file, char *string, size_t tuple_size, uint64_t *str_pos);
enum status_of_basic_operation delete_last_tuple(FILE *file, size_t full_tuple_size);
enum status_of_basic_operation insert_new_tuple(FILE *file, struct my_tuple *tuple, size_t full_tuple_size, uint64_t *tuple_pos);
enum status_of_basic_operation remove_from_id_array(FILE *file, uint64_t id, uint64_t* offset);
enum status_of_basic_operation id_to_offset(FILE *file, uint64_t id, uint64_t* offset);
enum status_of_basic_operation offset_to_id(FILE *file, uint64_t* id, uint64_t offset);
enum status_of_basic_operation change_string_tuple(FILE *file, uint64_t offset, char *new_string,  uint64_t size);
enum status_of_basic_operation check_id(FILE *file, uint64_t id);
enum status_of_basic_operation link_strings_to_tuple(FILE *file, struct my_tuple* tpl, uint64_t tpl_offset);
enum status_of_basic_operation change_parameter(FILE *file, enum tree_subheader_parameter parameter, uint64_t value);
void get_types(FILE *file, uint32_t **types, size_t *size);
size_t append_to_id_array(FILE *file, uint64_t offset);

#endif //LAB1_BASIC_OPERATIONS_H
