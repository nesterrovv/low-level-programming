#include "update_command.h"

size_t update_item(FILE *f, char **str, size_t pattern_size, const uint32_t *pattern_types, char **pattern_names, size_t fields_count) {
    char **key_value;
    size_t count;
    uint64_t value;
    size_t par_pos = -1;

    if (!isNumeric(str[1])) {
        printf("Not-numeric id.\n");
        return 1;
    }
    for (size_t iter = 2; iter < fields_count; iter++) {
        count = split(str[iter], '=', &key_value);
        if (count != 2) {
            printf("Invalid input.\n");
            return 2;
        }
        for (size_t in_iter = 0; in_iter < pattern_size; in_iter++) {
            if (strcmp(key_value[0], pattern_names[in_iter]) == 0) {
                par_pos = in_iter;
                break;
            }
        }
        if (par_pos == -1) {
            printf("'%s' field does not match pattern.\n", str[iter]);
            return 3;
        }

        double val;
        switch (pattern_types[par_pos]) {
            case BOOLEAN_TYPE:
                if (strcmp(key_value[1], "True") == 0)
                    value = true;
                else if (strcmp(key_value[1], "False") == 0)
                    value = false;
                else {
                    printf("Not-bool '%s' parameter.\n", key_value[1]);
                    return 4;
                }
                break;
            case FLOAT_TYPE:
                val = strtod(key_value[1], NULL);
                if (val == 0.0) {
                    printf("Not-float '%s' parameter.\n", key_value[1]);
                    return 4;
                }
                memcpy(&value, &val, sizeof(val));

                break;
            case INTEGER_TYPE:
                if (!isNumeric(key_value[1])) {
                    printf("Not-integer '%s' parameter.\n", key_value[1]);
                    return 4;
                }
                value = atoi(key_value[1]);
                break;
            case STRING_TYPE:
                value = key_value[1];
                break;
        }
        update_tuple(f, par_pos, &value, atoi(str[1]));
        free_test(key_value);
        par_pos = -1;
    }

    return 0;
}


