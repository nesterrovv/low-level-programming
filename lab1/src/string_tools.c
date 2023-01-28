#include "string_tools.h"

char *concat(const char *s1, const char *s2) {
    char *result = malloc_test(strlen(s1) + strlen(s2) + 1);
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

size_t split(char *str, const char c, char ***arr) {

    int count = 1;
    int i = 0;
    char *p;

    p = str;
    while (*p != '\n' && *p != '\0') {
        if (*p == c)
            count++;
        p++;
    }

    *arr = (char **) malloc_test(sizeof(char *) * count);
    if (*arr == NULL)
        exit(1);


    for (char *pch = strtok(str, &c); pch != NULL; pch = strtok(NULL, &c)) {
        if (i == count - 1 && pch[strlen(pch) - 1] == '\n')
            pch[strlen(pch) - 1] = '\0';
        (*arr)[i++] = pch;
    }

    return count;
}

bool isNumeric(const char *str) {
    while (*str != '\0' && *str != 13) {
        if (*str < '0' || *str > '9')
            return false;
        str++;
    }
    return true;
}

void clean_test_tree_header(struct header_of_tree* header);
enum status_of_read_operation read_tree_header(struct header_of_tree *header, FILE *file);
void parse_file(FILE *to, FILE *from) {
    char line[INPUT_LINE_SIZE];
    char **args = NULL;
    size_t pattern_size;
    struct header_of_tree *header = malloc_test(sizeof(struct header_of_tree));

    read_tree_header(header, to);
    pattern_size = header->subheader->pattern_size;
    uint32_t *pattern_types = malloc_test(sizeof(uint32_t) * pattern_size);
    char **pattern_names = malloc_test(sizeof(char *) * pattern_size);

    for (int i = 0; i < pattern_size; i++) {
        pattern_types[i] = header->pattern[i]->header->type;
        pattern_names[i] = header->pattern[i]->key_value;
    }


    fgets(line, INPUT_LINE_SIZE, from);
    while (!feof(from)) {
        if (strlen(line) == 0)
            break;
        line[strlen(line) - 1] = '\0';
        char *prefix = concat("add ", line);
        split(prefix, ' ', &args);

        size_t code = add_input_item(to, args, pattern_size, pattern_types, pattern_names);


        if (code != 0) {
            printf("Error code %zu\n In line: %s\n", code, line);
        }
        free_test(prefix);
        free_test(args);
        fgets(line, INPUT_LINE_SIZE, from);
    }


    clean_test_tree_header(header);
    fclose(from);
    fflush(to);
    free_test(pattern_types);
    free_test(pattern_names);


}