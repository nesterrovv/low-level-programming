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
