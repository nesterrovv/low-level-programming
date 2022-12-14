#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include        <io.h>
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
    hashset_t free_offsets = hashset_create();
    if (free_offsets == NULL) {
        fprintf(stderr, "Failed to create hashset for free offsets storing.\n");
        abort();
    }
    int available_offset = 0;
//    //clear_navigator();
//    printf("start\n");
//    struct some_type* some_type = malloc(sizeof(struct some_type));
//    printf("start2\n");
//    some_type -> type = my_int;
//    some_type -> i = 42;
//    printf("Value: %d\n", some_type -> i);
//    printf("start3\n");
//    char* p = "/root/example/first/";
//    int rrr = write_to_data_heap(some_type, p);
//    char* necessaryPath = "/root/example/folder";
//    long longNum = 0;
//    FILE* fp = fopen("resources/navigator.txt", "r");
//    fseek(fp, longNum, SEEK_END);
//    long size = ftell(fp);
//    printf("Size: %ld\n", size);
//    printf("method ended.\n");
//    printf("hello\n");
//    char k[4096] = "/come/nesterrovv/folder/file";
//    char kk[4096] = "/fdfdf/fdovv";
//    printf("Size of k string: %zu byte\n", sizeof(k));
//    printf("Size of kk string: %zu byte\n", sizeof(kk));
//    if (argc == 0) {
//        printf("Filepath is not defined. You should define it as a command line argument.\n");
//        return 1;
//    }
    char *filepath = argv[1];
    HASHMAP(char, int) map;
    /* Initialize with default string key hash function and comparator */
    hashmap_init(&map, hashmap_hash_string, strcmp);
    printf("map created\n");
    printf("counted\n");
    //char navigator_data[block_amount][2][1024];
    //printf("Amount of blocks: %d\n", block_amount);
    /* Downloading beta-version of navigation information */
    FILE* file_pointer;
    char* line = NULL;
    FILE* file = fopen("resources/persons.txt", "wb");
    integer integer11;
    integer11.data_type = 0;
    strcpy(integer11.pathToThisNode, "/root/folder");
    integer11.offset_of_current_block = 1;
    integer11.offset_of_previous_block = 0;
    integer11.offset_of_next_block = 0;
    integer11.offset_to_parent_node = 0;
    uint64_t children[10] = {2, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    for (int i = 0; i < 10; ++i) {
        integer11.children[i] = children[i];
    }
    integer11.isDirectory = 1;
    integer11.data_cell = 0;
    integer integer12;
    integer12.data_type = 1;
    strcpy(integer12.pathToThisNode, "/root/folder/first_int.INT");
    integer12.offset_of_current_block = 2;
    integer12.offset_of_previous_block = 0;
    integer12.offset_of_next_block = 0;
    integer12.offset_to_parent_node = 1;
    uint64_t children2[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    for (int i = 0; i < 10; ++i) {
        integer12.children[i] = children2[i];
    }
    integer12.isDirectory = 0;
    integer12.data_cell = 42;
    fwrite(&integer11, sizeof(integer11), 1, file);
    fwrite(&integer12, sizeof(integer11), 1, file);
    printf("Test data has been already written!\n");
    fclose(file);
    file_pointer = fopen("resources/navigator.txt", "r");
    if (file_pointer == NULL) {
        printf("check wtf\n");
        return 1111;
    }
    int block_counter = count_blocks();
    char navigation_data[block_counter][2][1024];
    int current_block_number = 0;
    ssize_t read;
    size_t len = 0;
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

    //user greeting
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
        //printf("Your input: \'%s\'.\n", enter_after_trim);
        // block for definition type of data chunk
        if (strcmp(enter_after_trim, "READ") == 0 || strcmp(enter_after_trim, "1") == 0) {
            char entered_path[4096];
            printf("Enter a filepath. It have to be path like in *NIX systems. For example, /home/folder/file. It can be:\n");
            printf("\t1. Path to folder. In this case, you'll see list of data chunks or directories in this folder.\n");
            printf("\t2. Data chunk. In this case, you'll see content of the chunk.\n");
            printf("Enter in here: ");
            fgets(entered_path, 4096, stdin);
            strtok(entered_path, "\n");
            //printf("sizeof: %zu\n", sizeof(entered_path));
            int* found_offset;
            found_offset = hashmap_get(&map, entered_path);
            if (found_offset) {
                printf("Data chunk found! Data reading...\n");
                int length_of_path = strlen(entered_path);
                char *extension = &entered_path[length_of_path - 4];
                if (strcmp(extension, ".INT") == 0) {
                    integer read_integer;
                    read_chunk_integer(*found_offset, &read_integer);
                    printf("Integer number data chunk found! Value: %d\n", read_integer.data_cell);
                } else if (strcmp(extension, ".FLT") == 0) {
                    floating_number  read_float;
                    read_chunk_floating_number(*found_offset, &read_float);
                    printf("Floating number data chunk found! Value: %f\n", read_float.data_cell);
                } else if (strcmp(extension, ".BLN") == 0) {
                    boolean read_boolean;
                    read_chunk_boolean(*found_offset, &read_boolean);
                    printf("Boolean data chunk found! Value: %s\n", read_boolean.data_cell ? "true" : "false");
                } else if (strcmp(extension, ".STR") == 0) {
                    string  read_string;
                    read_chunk_string(*found_offset, &read_string);
                    printf("String data chunk found! Value: %s\n", read_string.data_cell);
                } else {
                    printf("It's a directory. List of inline items:\n");
                    integer read_directory;
                    int item_counter = 1;
                    read_chunk_integer(*found_offset, &read_directory);
                    for (int child_number = 0; child_number <
                            (sizeof(read_directory.children) / sizeof(read_directory.children[0])); child_number++) {
                        int child_offset = read_directory.children[child_number];
                        if (child_offset != 0) {
                            char child_path[4096];
                            read_item_path(*child_offset, child_path);
                            printf("\t%d. %s\n", child_number + 1, child_path);
                            item_counter++;
                        }
                    }
                }
            } else {
                printf("%s\n", "Not found item of document tree with this path.");
            }
        } else if (strcmp(enter_after_trim, "DELETE") == 0 || strcmp(enter_after_trim, "2") == 0) {
            // TODO: add deleting of data piece
            char entered_path[4096];
            printf("Enter a filepath. It have to be path like in *NIX systems. For example, /home/folder/file:\n");
            fgets(entered_path, 4096, stdin);
            strtok(entered_path, "\n");
            printf("You're entered: '%s'\n", entered_path);
            int* found_offset;
            found_offset = hashmap_get(&map, entered_path);
            if (found_offset) {
                printf("Item found! Value: %d\n", *found_offset);
                hashset_add(free_offsets, found_offset);
                // TODO: add downloading block of data from file with data heap via found offset
            } else {
                printf("%s\n", "Not found item of document tree with this path.");
            }
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
