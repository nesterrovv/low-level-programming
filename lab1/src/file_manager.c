//
// Created by Ivan Nesterov on 11/17/2022.
//

#include <stdlib.h>
#include <stdio.h>
#define  _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include "../include/string_methods.h"
#include "../include/structures.h"

#define FILE_NOT_FOUND      1
#define SUCCESS             0

int available_offset = 0;

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

int write_to_data_heap(struct some_type* some_type, char* necessaryPath) {
    FILE *file_pointer;
    file_pointer = fopen("resources/data_heap.txt", "a+");
    if (file_pointer == NULL) {
        return FILE_NOT_FOUND;
    }
    // STRUCTURE CREATING
    data current_data;
    current_data.offset_to_parent_node = 0; // TODO: add finding an offset via entered path to current
    // TODO: add changing of children array of parent node
    // next two blocks are constant for integer/floating numbers/boolean types
    current_data.offset_of_previous_block = 0;
    current_data.offset_of_next_block = 0;
    current_data.offset_of_current_block = available_offset; // TODO: receive this value from array of 'free' offsets
    available_offset += 1;
    current_data.children;
    current_data.isDirectory = false;
    switch (some_type->type) {
        strcpy(current_data.pathToThisNode, necessaryPath);
        //current_data.pathToThisNode = necessaryPath;
        case my_int:;
            // TODO check process of writing integer type structure;
            current_data.data_type.data_type = 1;
            integer this_integer;
            this_integer.data = current_data;
            this_integer.data_cell = some_type -> i;
            // FINISH OF STRUCTURE CREATING
            // SERIALIZING STRUCTURE TO FILE
            fprintf(file_pointer, "%d ", this_integer.data.data_type.data_type);
            fprintf(file_pointer, "%s ", this_integer.data.pathToThisNode);
            fprintf(file_pointer, "%llu ", this_integer.data.offset_of_current_block);
            fprintf(file_pointer, "%llu ", this_integer.data.offset_of_previous_block);
            fprintf(file_pointer, "%llu ", this_integer.data.offset_of_next_block);
            fprintf(file_pointer, "%llu ", this_integer.data.offset_to_parent_node);
            for (int i = 0; i < 65536; i++) {
                fprintf(file_pointer,"%llu ", this_integer.data.children[i]);
            }
            fprintf(file_pointer, "%d ", this_integer.data.isDirectory);
            fprintf(file_pointer, "%d\n", this_integer.data_cell);
            break;
        case my_float:;
            // TODO check process of writing floating_type type structure;
            current_data.data_type.data_type = 2;
            floating_number this_floating_number;
            this_floating_number.data = current_data;
            this_floating_number.data_cell = some_type -> f;
            // FINISH OF STRUCTURE CREATING
            // SERIALIZING STRUCTURE TO FILE
            fprintf(file_pointer, "%d ", this_floating_number.data.data_type.data_type);
            fprintf(file_pointer, "%s ", this_floating_number.data.pathToThisNode);
            fprintf(file_pointer, "%llu ", this_floating_number.data.offset_of_current_block);
            fprintf(file_pointer, "%llu ", this_floating_number.data.offset_of_previous_block);
            fprintf(file_pointer, "%llu ", this_floating_number.data.offset_of_next_block);
            fprintf(file_pointer, "%llu ", this_floating_number.data.offset_to_parent_node);
            for (int i = 0; i < 65536; i++) {
                fprintf(file_pointer,"%llu ", this_floating_number.data.children[i]);
            }
            fprintf(file_pointer, "%d ", this_floating_number.data.isDirectory);
            fprintf(file_pointer, "%d\n", this_floating_number.data_cell);
            break;
        case my_string:;
            // TODO check process of writing integer type structure;
            current_data.data_type.data_type = 3;
            string this_string;
            this_string.data = current_data;
            this_string.data_cell = some_type -> s;
            // FINISH OF STRUCTURE CREATING
            // SERIALIZING STRUCTURE TO FILE
            fprintf(file_pointer, "%d ", this_string.data.data_type.data_type);
            fprintf(file_pointer, "%s ", this_string.data.pathToThisNode);
            fprintf(file_pointer, "%llu ", this_string.data.offset_of_current_block);
            fprintf(file_pointer, "%llu ", this_string.data.offset_of_previous_block);
            fprintf(file_pointer, "%llu ", this_string.data.offset_of_next_block);
            fprintf(file_pointer, "%llu ", this_string.data.offset_to_parent_node);
            for (int i = 0; i < 65536; i++) {
                fprintf(file_pointer,"%llu ", this_string.data.children[i]);
            }
            fprintf(file_pointer, "%d ", this_string.data.isDirectory);
            fprintf(file_pointer, "%d\n", this_string.data_cell);
            break;
        case my_boolean:;
            // TODO check process of writing integer type structure;
            current_data.data_type.data_type = 0;
            boolean this_boolean;
            this_boolean.data = current_data;
            this_boolean.data_cell = some_type -> b;
            // FINISH OF STRUCTURE CREATING
            // SERIALIZING STRUCTURE TO FILE
            fprintf(file_pointer, "%d ", this_boolean.data.data_type.data_type);
            fprintf(file_pointer, "%s ", this_boolean.data.pathToThisNode);
            fprintf(file_pointer, "%llu ", this_boolean.data.offset_of_current_block);
            fprintf(file_pointer, "%llu ", this_boolean.data.offset_of_previous_block);
            fprintf(file_pointer, "%llu ", this_boolean.data.offset_of_next_block);
            fprintf(file_pointer, "%llu ", this_boolean.data.offset_to_parent_node);
            for (int i = 0; i < 65536; i++) {
                fprintf(file_pointer,"%llu ", this_boolean.data.children[i]);
            }
            fprintf(file_pointer, "%d ", this_boolean.data.isDirectory);
            fprintf(file_pointer, "%d\n", this_boolean.data_cell);
            break;
        default:
            abort();
    }
}
