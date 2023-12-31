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

struct operator_of_comparison *create_operator(uint8_t field_of_operator, enum data_types_names type_name,
                                               union data_types content) {
    struct operator_of_comparison* new_operator = check_malloc(sizeof(struct operator_of_comparison));
    new_operator -> content = content;
    new_operator -> data_type_name = type_name;
    new_operator -> field_of_operator = field_of_operator;
    return new_operator;
}

struct my_string* create_string(size_t size_of_string, char* content){
    struct my_string* new_string = check_malloc(sizeof(struct my_string));
    new_string -> content = content;
    new_string -> string_size = size_of_string;
    return new_string;
}

struct operator_of_comparison* read_operator_of_comparison(char** string){
    uint8_t integer_number = 0;
    if (**string && **string > '0' && **string < '9') {
        integer_number = 1;
    }
    char* temporary_parent = *string;
    size_t length = 0;
    struct operator_of_comparison *operator_of_comparison;
    if (integer_number) {
        double value = 0;
        uint8_t floating_number = 0;
        while (temporary_parent[length] && (temporary_parent[length] >= '0' && temporary_parent[length] <= '9' ||
                                            temporary_parent[length] == '.')) {
            if (temporary_parent[length] == '.') {
                length++;
                if (floating_number) {
                    return NULL;
                }
                floating_number = 1;
                continue;
            }
            if (floating_number) {
                double exponent = 0.1;
                for(size_t iteration_number = 1; iteration_number < floating_number; iteration_number++) {
                    exponent *= 0.1;
                }
                floating_number++;
                value = value +  exponent * (temporary_parent[length] - '0');
            } else {
                value = 10 * value + temporary_parent[length] - '0';
            }
            length++;
        }
        union data_types data_type;
        if (floating_number) {
            data_type.floating_number = value;
            operator_of_comparison = create_operator(!integer_number, FLOATING_NUMBER,
                                                     data_type);
        } else {
            data_type.integer = (int64_t) value;
            operator_of_comparison = create_operator(!integer_number, INTEGER, data_type);
        }

    } else {
        uint8_t string_data = 1;
        if (**string == '\'' && *(*string)++) {
            string_data = 0;
        }
        temporary_parent = *string;
        while (temporary_parent[length] && (temporary_parent[length] >= 'a' && temporary_parent[length] <= 'z' ||
                                            temporary_parent[length] >= 'A' && temporary_parent[length] <= 'Z')){
            length++;
        }
        char *value = check_malloc(sizeof(char) * length);
        temporary_parent = *string;
        for (size_t iter = 0; iter < length; iter++){
            value[iter] = temporary_parent[iter];
        }
        struct my_string* str_data = create_string(length, value);
        union data_types new_data_type;
        new_data_type.string = str_data;
        operator_of_comparison = create_operator(!integer_number, STRING, new_data_type);
        operator_of_comparison -> field_of_operator = string_data;
    }
    *string += length;
    return operator_of_comparison;
}

struct filter *create_filter(uint8_t false){
    struct filter* filter = check_malloc(sizeof(struct filter));
    filter -> not_passed = false;
    filter -> list_of_comparators = NULL;
    return filter;
}

struct list_of_filters* create_filter_list(){
    struct list_of_filters* list_of_filters = check_malloc(sizeof(struct list_of_filters));
    list_of_filters -> next_filter = NULL;
    list_of_filters -> current_filter = NULL;
    return list_of_filters;
}


struct list_of_filters* create_filter_list_with_filter(uint8_t false){
    struct filter* my_filter = create_filter(false);
    struct list_of_filters* list_of_filters = create_filter_list();
    list_of_filters -> current_filter = my_filter;
    return list_of_filters;
}

struct list_of_comparators* create_list_of_comparators(){
    struct list_of_comparators* list_of_comparators = check_malloc(sizeof(struct list_of_comparators));
    list_of_comparators -> current_comparator = NULL;
    list_of_comparators -> next_comparator = NULL;
    return list_of_comparators;
}

struct comparator* create_comparator(){
    struct comparator* new_comparator = check_malloc(sizeof(struct comparator));
    new_comparator -> false = 0;
    new_comparator -> true = 0;
    new_comparator -> comparison_operation = 0;
    new_comparator -> first_operator = NULL;
    new_comparator -> second_operator = NULL;
    return new_comparator;
}

struct comparator* execute_filter(char** string){
    struct comparator* new_comparator = create_comparator();
    if (**string && **string=='!' && *(*string)++) {
        new_comparator -> false = 1;
    }
    if (**string && **string=='@' && *(*string)++) {
        new_comparator -> true = 1;
    } else {
        new_comparator -> first_operator = read_operator_of_comparison(string);
        new_comparator -> comparison_operation = (enum comparison_operations_symbols) *(*string)++;
        new_comparator -> second_operator = read_operator_of_comparison(string);
    }
    return new_comparator;
}

void add_comparator(struct list_of_filters* list_of_filters, struct comparator* new_comparator){
    struct list_of_comparators* list_of_comparators = create_list_of_comparators();
    list_of_comparators -> current_comparator = new_comparator; // adding
    // including this object to the chain
    list_of_comparators -> next_comparator = list_of_filters -> current_filter -> list_of_comparators;
    list_of_filters -> current_filter -> list_of_comparators = list_of_comparators;
}

enum possible_states execute_attribute(char** string, struct list_of_tree_level* level) {
    uint8_t false = 0;
    struct comparator* new_comparator;
    if (**string && **string == '!' && *(*string)++) {
        false++;
    }
    if (**string == '[') {
        struct list_of_filters* list_of_filters = create_filter_list_with_filter(false);
        list_of_filters -> next_filter = level -> list_of_filters;
        level -> list_of_filters = list_of_filters;
        while (**string != ']' && *(*string)++) {
            new_comparator = execute_filter(string);
            add_comparator(level -> list_of_filters, new_comparator);
            if (**string != '|' && **string != ']' && *(*string)++) {
                return IS_ERROR;
            }
        }
        (*string)++;
        if (**string == '[' || **string == '!') {
            return IS_ATTRIBUTE;
        } else {
            return HAS_NEXT;
        }
    } else {
        return IS_ERROR;
    }
}

enum possible_states execute_error(char** string, struct list_of_tree_level *level) {
    return IS_ERROR;
}

enum possible_states (*choose_operation_by_state[4]) (char**, struct list_of_tree_level*) = {
        [HAS_NEXT] = execute_next,
        [IS_NAME] = execute_name,
        [IS_ATTRIBUTE] = execute_attribute,
        [IS_ERROR] = execute_error,
};