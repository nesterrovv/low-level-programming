#include "user_ui.h"

enum status_of_read_operation read_tree_header(struct header_of_tree *header, FILE *file);
void console_mode(FILE *f) {
    size_t pattern_size;
    struct header_of_tree *header = malloc_test(sizeof(struct header_of_tree));

    read_tree_header(header, f);
    pattern_size = header -> subheader -> pattern_size;
    uint32_t *pattern_types = malloc_test(sizeof(uint32_t) * pattern_size);
    char **pattern_names = malloc_test(sizeof(char *) * pattern_size);

    for (int i = 0; i < pattern_size; i++) {
        pattern_types[i] = header -> pattern[i] -> header -> type;
        pattern_names[i] = header -> pattern[i] -> key_value;
    }


    printf("+++ File initiated successfully! +++\n");
    printf("Enter 'help' to see what can you do\n");
    printf("Enter 'exit' to exit the program, if you're don't wanna do something\n");

    char *input_str = malloc(INPUT_LINE_SIZE);
    size_t len = 0;
    size_t c;
    char **arr;
    getline(&input_str, &len, stdin);
    if (strcmp(input_str, "\n") == 0) {
        printf("Enter the command: ");
        getline(&input_str, &len, stdin);
    }
    c = split(input_str, ' ', &arr);

    while (strcmp(arr[0], "exit") != 0) {
        if (strcmp(arr[0], "help") == 0) print_help();
         else if (strcmp(arr[0], "add") == 0) {
            if (c == pattern_size + 2) {
                size_t code = add_input_item(f, arr, pattern_size, pattern_types, pattern_names);
                if (code != 0) {
                    printf("Exception code: %zu\n", code);
                }
                printf("New descendant added successfully!\n");
            } else
                printf("Wrong number of parameters(including parent_id): %lu expected, %lu entered.\n",
                       pattern_size + 1, c - 1);

        } else if (strcmp(arr[0], "update") == 0) {
            size_t code = update_item(f, arr, pattern_size, pattern_types, pattern_names, c);
            if (code != 0) {
                printf("Exception code: %zu\n", code);
            }
        } else if (strcmp(arr[0], "delete") == 0) {
            if (c == 2) {
                if (isNumeric(arr[1])) {
                    if (remove_tuple(f, atoi(arr[1]), 0) == IS_INVALID)
                        printf("Such id does not exist.\n");
                    else
                        printf("Deleted successfully.\n");
                } else
                    printf("Not-numeric id.\n");
            } else if (c == 1)
                printf("Missing parameter <node_id>.\n");
            else printf("Too many parameters.\n");
        } else if (strcmp(arr[0], "print_header") == 0) {
            print_tree_header_from_file(f);
        } else if (strcmp(arr[0], "print_data") == 0) {
            print_tuple_array_from_file(f);
        }
        else if (strcmp(arr[0], "select_by") == 0) {
            find_by(f, arr, pattern_size, pattern_types, pattern_names, c);
        }
        else printf("Exception! Incorrect command!\n");

        free_test(arr);
        printf("Enter the command: ");
        getline(&input_str, &len, stdin);
        c = split(input_str, ' ', &arr);
    }
    close_file(f);
}
