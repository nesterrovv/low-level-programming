//
// Created by Ivan Nesterov on 11/17/2022.
//

#include <stdlib.h>
#include <stdio.h>
#define  _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include "../include/string_methods.h"

#define FILE_NOT_FOUND      1
#define SUCCESS             0

int clear_navigator() {
    FILE* file_pointer;
    file_pointer = fopen("resources/navigator.txt", "w");
    if (file_pointer == NULL) {
        return FILE_NOT_FOUND;
    }
    fclose(file_pointer);
    return SUCCESS;
}

int count_blocks() {
    FILE* file_pointer;
    char* block_line = NULL;
    ssize_t read;
    size_t block_len = 0;
    int block_counter = 0;
    file_pointer = fopen("resources/navigator.txt", "r");
    if (file_pointer == NULL) {
        return FILE_NOT_FOUND;
    }
    while ((read = getline(&block_line, &block_len, file_pointer)) != -1) {
        block_counter++;
    }
    return block_counter;
}

//char*** download_navigator() {
//    FILE* file_pointer;
//    char* line = NULL;
//    ssize_t read;
//    size_t len = 0;
//    file_pointer = fopen("resources/navigator.txt", "r");
//    if (file_pointer == NULL) {
//        return FILE_NOT_FOUND;
//    }
//    int block_counter = count_blocks();
//    char navigation_data[block_counter][2][4096];
//    int current_block_number = 0;
//    while ((read = getline(&line, &len, file_pointer)) != -1) {
//        printf("Retrieved line of size: %zu\n", sizeof(line));
//        printf("%s", line);
//        char new_line[4096];
//        strcpy(new_line, line);
//        printf("Line updated. Current size is: %zu\n", sizeof(new_line));
//        printf("%s", new_line);
//        char **tokens;
//        char split_data[2][4096];
//        tokens = str_split(new_line, ' ');
//        if (tokens) {
//          int i;
//          for (i = 0; *(tokens + i); i++) {
//              printf("split_data=[%s]\n", *(tokens + i));
//              strcpy(split_data[i], *(tokens + i));
//              free(*(tokens + i));
//          }
//          printf("\n");
//          free(tokens);
//          strcpy(navigation_data[current_block_number][0], split_data[0]);
//          strcpy(navigation_data[current_block_number][1], split_data[1]);
//          printf("-----Data preparing. Step %d-----\n", i);
//          printf("Key: '%s'\n", navigation_data[current_block_number][0]);
//          printf("Value: '%s'\n", navigation_data[current_block_number][1]);
//        }
//        current_block_number++;
//    }
//    return ***navigation_data;
//    fclose(file_pointer);
//    if (line) {
//        free(line);
//    }
//    return SUCCESS;
//
//}

int write_to_navigator(char path_to_item[], int offset) {
    FILE* file_pointer;
    file_pointer = fopen("resources/navigator.txt", "a+");
    if(file_pointer == NULL) {
        return FILE_NOT_FOUND;
    }
    fprintf(file_pointer,"%s %d\n",path_to_item, offset);
    fclose(file_pointer);
    return SUCCESS;
}