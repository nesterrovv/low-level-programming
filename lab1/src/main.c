#include "user_ui.h"
#include "basic_operations.h"

void init_file(FILE *file) {
    printf("### Welcome to my program! ###\n");
    printf("Type the number of fields in data(int):");
    char *count_str = malloc(INPUT_LINE_SIZE);
    scanf("%s", count_str);
    while (!isNumeric(count_str)) {
        printf("ERROR! Input must be an Integer! ");
        scanf("%s", count_str);
    }
    size_t count = strtol(count_str, NULL, 10);
    char *str;
    char **str_array = malloc_test(count * sizeof(char *));
    char *type = malloc(INPUT_LINE_SIZE);
    uint32_t *types = malloc_test(count * sizeof(uint32_t));
    size_t *sizes = malloc_test(count * sizeof(size_t));
    size_t temp_size;
    for (size_t iter = 0; iter < count; iter++) {
        printf("### Field %-3zu###\n", iter);
        str = malloc_test(INPUT_LINE_SIZE);
        printf("Input the name of the field:");
        scanf("%s", str);
        str_array[iter] = str;
        temp_size = strlen(str);
        sizes[iter] = temp_size + (!(temp_size % FILE_GRANULARITY) ? 1 : 0);
        printf("Choose type:\n");
        printf("%d. Boolean\n", BOOLEAN_TYPE);
        printf("%d. Integer\n", INTEGER_TYPE);
        printf("%d. Float\n", FLOAT_TYPE);
        printf("%d. String\n", STRING_TYPE);
        scanf("%s", type);
        while (strlen(type) != 1) {
            printf("ERROR! Incorrect input! ");
            scanf("%s", type);
        }
        types[iter] = strtol(type, NULL, 10);
    }
    create_empty_file(file, str_array, types, count, sizes);
    for (size_t iter = 0; iter < count; iter++) free_test(str_array[iter]);
    free_test(str_array);
    free_test(sizes);
    free_test(types);
    free_test(count_str);
    free_test(type);
}

int main(int argc, char **argv) {
    char *main_filename;
    char *arg_filename;
    FILE *file;
    FILE *parsable;
    char flag;
    if (argc < 3 || argc > 4) {
        printf("ERROR! Check your input, wrong number of args!");
        return 1;
    } else if (argc == 4) {
        arg_filename = argv[3];
    }
    main_filename = argv[2];
    flag = argv[1][1];

    switch (flag) {
        case 'c':
            close_empty_file(main_filename, &file);
            init_file(file);
            break;
        case 'o':
            open_parse_file(arg_filename, &parsable);
            close_empty_file(main_filename, &file);
            init_file(file);
            parse_file(file, parsable);
            break;
        default:
            printf("ERROR! Unknown flag: -%c", flag);
            return 1;
    }
    console_mode(file);
    return 0;
}



