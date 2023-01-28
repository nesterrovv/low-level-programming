#include "add_command.h"

size_t add_input_item(FILE *f, char **str, size_t pattern_size, const uint32_t *pattern_types, char **pattern_names) {
    char **key_value;
    size_t count;
    uint64_t fields[pattern_size];
    size_t par_pos = -1;
    if (!isNumeric(str[1])) {
        printf("It is not numeric ID.\n");
        return 1;
    }
    for (size_t iter = 2; iter < pattern_size + 2; iter++) {
        count = split(str[iter], '=', &key_value);
        if (count != 2) {
            return 2;
        }
        for (size_t in_iter = 0; in_iter < pattern_size; in_iter++) {
            if (strcmp(key_value[0], pattern_names[in_iter]) == 0) {
                par_pos = in_iter;
                break;
            }
        }
        if (par_pos == -1) {
            printf("'%s' Field does not match this pattern.\n", str[iter]);
            return 3;
        }
        double val;
        switch (pattern_types[par_pos]) {
            case BOOLEAN_TYPE:
                if (strcmp(key_value[1], "True") == 0)
                    fields[par_pos] = true;
                else if (strcmp(key_value[1], "False") == 0)
                    fields[par_pos] = false;
                else {
                    printf("Non-boolean value '%s'\n", key_value[1]);
                    return 4;
                }
                break;
            case FLOAT_TYPE:
                val = strtod(key_value[1], NULL);
                if (val == 0.0) {
                    printf("Non-floating-number-value '%s'\n", key_value[1]);
                    return 4;
                }
                memcpy(&fields[par_pos], &val, sizeof(val));
                break;
            case INTEGER_TYPE:
                if (!isNumeric(key_value[1])) {
                    printf("Non-integer-number-value '%s'\n", key_value[1]);
                    return 4;
                }
                fields[par_pos] = strtol(key_value[1], NULL, 10);
                break;
            case STRING_TYPE:
                fields[par_pos] = (uint64_t) key_value[1];
                break;
        }
        par_pos = -1;
        free_test(key_value);
    }
    add_tuple(f, fields, strtol(str[1], NULL, 10));
    return 0;
}
