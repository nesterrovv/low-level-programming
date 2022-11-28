#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include "../include/structures.h"
#include "../include/hashset.h"
#include "../include/hashset_itr.h"
#include "../include/arraylist.h"
#include "../include/string_methods.h"
#include "../include/hashmap.h"
#include "../include/file_manager.h"

#define NAVIGATOR_READING_ERROR 11;

// argv[1] - filepath to data file (expected of it)
int main(int argc, char *argv[]) {
    char k[4096] = "/come/nesterrovv/folder/file";
    char kk[4096] = "/fdfdf/fdovv";
    printf("Size of k string: %zu byte\n", sizeof(k));
    printf("Size of kk string: %zu byte\n", sizeof(kk));
    if (argc == 0) {
        printf("Filepath is not defined. You should define it as a command line argument.\n");
        return 1;
    }
    char *filepath = argv[1];
    HASHMAP(char, int) map;
    /* Initialize with default string key hash function and comparator */
    hashmap_init(&map, hashmap_hash_string, strcmp);
    int block_amount = count_blocks();
    char navigator_data[block_amount][2][4096];
    /* Downloading beta-version of navigation information */
    FILE* file_pointer;
    char* line = NULL;
    ssize_t read;
    size_t len = 0;
    file_pointer = fopen("resources/navigator.txt", "r");
    if (file_pointer == NULL) {
        return 1111;
    }
    int block_counter = count_blocks();
    char navigation_data[block_counter][2][4096];
    int current_block_number = 0;
    while ((read = getline(&line, &len, file_pointer)) != -1) {
        char new_line[4096];
        strcpy(new_line, line);
        char **tokens;
        char split_data[2][4096];
        tokens = str_split(new_line, ' ');
        if (tokens) {
            int i;
            for (i = 0; *(tokens + i); i++) {
                strcpy(split_data[i], *(tokens + i));
                free(*(tokens + i));
            }
            free(tokens);
            strcpy(navigation_data[current_block_number][0], split_data[0]);
            strcpy(navigation_data[current_block_number][1], split_data[1]);
            printf("--------------------Data preparing. Step #%d--------------------\n", current_block_number);
            char* k = navigation_data[current_block_number][0];
            int v = atoi(navigation_data[current_block_number][1]);
            printf("Key: '%s'\n", k);
            printf("Value: '%d'\n", v);
            int r = hashmap_put(&map, k, &v);
            if (r == 0) {
                printf("Node successfully added.\n");
            } else {
                printf("WARNING. Downloading skipped, because node is already exists.\n");
            }
            printf("---------------------------------------------------------------\n");
        }
        current_block_number++;
    }
    printf("System is ready!\n");
    // user greeting
    fprintf(stdout, "\"Big data\" manager has been successfully started!\n");
    fprintf(stdout, "Which data manipulation do you wish to run?\n");
    fprintf(stdout, "Variants:\n");
    fprintf(stdout, "  1. READ;\n");
    fprintf(stdout, "  2. DELETE;\n");
    fprintf(stdout, "  3. WRITE;\n");
    fprintf(stdout, "  4. EDIT.\n");
    fprintf(stdout, "  5. EXIT - for finishing a program.\n");
    fprintf(stdout, "You should type number of operation or it's name here.\n");
    char enter_before_trim[4096];
    char enter_after_trim[4096];
    while (!(strcmp(enter_after_trim, "EXIT") == 0 || strcmp(enter_after_trim, "5") == 0)) {
        printf("Type a command: ");
        fgets(enter_before_trim, 1024, stdin);
        trim(enter_after_trim, enter_before_trim);
        to_upper_case(enter_after_trim);
        printf("Your input: \'%s\'.\n", enter_after_trim);
        if (strcmp(enter_after_trim, "READ") == 0 || strcmp(enter_after_trim, "1") == 0) {
            char entered_path[4096];
            printf("Enter a filepath. It have to be path like in *NIX systems. For example, /home/folder/file. It can be:\n");
            printf("\t1. Path to folder. In this case, you'll see list of files/directories in this folder.\n");
            printf("\t2. Path to file. In this case, you'll see content of file.\n");
            printf("Enter in here: ");
            fgets(entered_path, 4096, stdin);
            strtok(entered_path, "\n");
            printf("You're entered: '%s'\n", entered_path);
            printf("sizeof: %zu\n", sizeof(entered_path));
            int* found_offset;
            found_offset = hashmap_get(&map, entered_path);
            if (found_offset) {
                printf("Item found! Value: %d\n", *found_offset);
                // TODO: add downloading block of data from file with data heap via found offset
            } else {
                printf("%s\n", "Not found item of document tree with this path.");
            }
        } else if (strcmp(enter_after_trim, "DELETE") == 0 || strcmp(enter_after_trim, "2") == 0) {
            // TODO: add delete function call
        } else if (strcmp(enter_after_trim, "WRITE") == 0 || strcmp(enter_after_trim, "3") == 0) {
            // TODO: add write function call
        } else if (strcmp(enter_after_trim, "EDIT") == 0 || strcmp(enter_after_trim, "4") == 0) {
            // TODO edit function call
        } else {
            printf("%s\n", "Unknown command. Please, try again.");
        }
    }
    hashmap_cleanup(&map);
    printf("%s\n", "Finishing a program. Thanks for using!");
}
