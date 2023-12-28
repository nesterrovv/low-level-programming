#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

#define MAXIMAL_COMMAND_LENGTH 1000
size_t size = 0;

enum possible_states {
    HAS_NEXT = 0,
    IS_NAME,
    IS_ATTRIBUTE,
    IS_ERROR
};

enum node_relations {
    IS_ROOT_NODE = 0,
    IS_CHILD_NODE,
    IS_FREE_NODE,
};

enum base_operations {
    INSERT = '+',
    UPDATE = '=',
    DELETE = '-',
    GET = '?'
};



enum comparison_operations_symbols {
    GREATER = '>',
    LESS = '<',
    EQUALS = '=',
    IS_SUBSTRING = ':'
};



enum data_types_names {
    STRING = 0,
    INTEGER,
    FLOATING_NUMBER,
    BOOLEAN,
};

struct my_string {
    size_t string_size;
    char* content;
};

union data_types {
    int64_t integer;
    double floating_number;
    int64_t boolean;
    struct my_string* string;
};

struct operator_of_comparison {
    uint8_t field_of_operator;
    enum data_types_names data_type_name;
    union data_types content;
};

struct comparator {
    uint8_t false;
    uint8_t true;
    enum comparison_operations_symbols comparison_operation;
    struct operator_of_comparison* first_operator;
    struct operator_of_comparison* second_operator;
};

struct list_of_comparators {
    struct comparator* current_comparator;
    struct list_of_comparators* next_comparator;
};

struct filter {
    uint8_t not_passed;
    struct list_of_comparators* list_of_comparators;
};

struct list_of_filters {
    struct list_of_filters* next_filter;
    struct filter* current_filter;
};

struct list_of_items {
    struct list_of_items* next_item;
    int64_t current_item;
};

struct list_of_tree_level {
    uint8_t false;
    uint8_t any;
    enum node_relations location;
    struct list_of_tree_level* next_list_of_tree_level;
    struct list_of_items* item;
    struct list_of_filters* list_of_filters;
};

struct representation {
    enum base_operations base_operation;
    struct list_of_tree_level* document_tree;
};

void* check_malloc(size_t new_memory){
    size += new_memory;
    return malloc(new_memory);
}

enum possible_states execute_next(char **string, struct list_of_tree_level *level) {
    if (**string == '/' && *(*string)++) {
        return IS_NAME;
    } else {
        return IS_ERROR;
    }
}

struct list_of_items* create_new_list_of_items(int64_t ID){
    struct list_of_items* new_list_of_items = check_malloc(sizeof(struct list_of_items));
    new_list_of_items -> current_item = ID;
    new_list_of_items -> next_item = NULL;
    return new_list_of_items;
}

enum possible_states execute_name(char **string, struct list_of_tree_level* current_level) {
    uint8_t negative = 0, any = 0;
    if (**string == '!' && *(*string)++) negative = 1;
    if (**string == '*' && *(*string)++) any = 1;
    if (!any) {
        if (**string && **string > '0' && **string < '9') {
            char *temp_p = *string;
            size_t len = 0;
            while (temp_p[len] >= '0' && temp_p[len] <= '9') len++;
            temp_p = *string;
            int64_t id = 0;
            for (size_t iter = 0; iter < len; iter++) {
                id = 10 * id + temp_p[iter] - '0';
            }
            current_level->item = create_new_list_of_items(id);
            current_level->false = negative;
            *string = temp_p + len;
        } else {
            return IS_ERROR;
        }
    } else {
        current_level->any = 1;
    }
    if (**string == '[' || **string == '!') {
        return IS_ATTRIBUTE;
    } else {
        return HAS_NEXT;
    }
}
