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

long count_size_of_file(FILE* file) {
    fseek(file, 0L, SEEK_END);
    long file_size = ftell(file); // size of file saved here
    fseek(file, 0L, SEEK_SET);
    return file_size;
}

bool is_chunk_exists(FILE* file, int offset_of_chunk) {
    long file_size = count_size_of_file(file);
    long byte_offset = (offset_of_chunk - 1) * 1152;
    return byte_offset <= file_size - sizeof(integer) && byte_offset % sizeof(integer) == 0;
}

int read_chunk_integer(long offset, integer* destination) {
    FILE* file = fopen("resources/persons.txt", "rb");
    fseek(file, 0L, SEEK_SET);
    int result = 0;
    if (is_chunk_exists(file, offset)) {
        long necessary_offset = (offset - 1) * sizeof(integer);
        fseek(file, necessary_offset, SEEK_SET);
        result = fread(destination, sizeof(integer), 1, file);
        fseek(file, 0L, SEEK_SET);
    }
    fclose(file);
    return result;
}

int read_chunk_floating_number(long offset, floating_number* destination) {
    FILE* file = fopen("resources/persons.txt", "rb");
    fseek(file, 0L, SEEK_SET);
    int result = 0;
    if (is_chunk_exists(file, offset)) {
        long necessary_offset = (offset - 1) * sizeof(floating_number);
        fseek(file, necessary_offset, SEEK_SET);
        result = fread(destination, sizeof(floating_number), 1, file);
        fseek(file, 0L, SEEK_SET);
    }
    fclose(file);
    return result;
}

int read_chunk_string(long offset, string* destination) {
    FILE* file = fopen("resources/persons.txt", "rb");
    fseek(file, 0L, SEEK_SET);
    int result = 0;
    if (is_chunk_exists(file, offset)) {
        long necessary_offset = (offset - 1) * sizeof(string);
        fseek(file, necessary_offset, SEEK_SET);
        result = fread(destination, sizeof(string), 1, file);
        fseek(file, 0L, SEEK_SET);
    }
    fclose(file);
    return result;
}

int read_chunk_boolean(long offset, boolean* destination) {
    FILE* file = fopen("resources/persons.txt", "rb");
    fseek(file, 0L, SEEK_SET);
    int result = 0;
    if (is_chunk_exists(file, offset)) {
        long necessary_offset = (offset - 1) * sizeof(boolean);
        fseek(file, necessary_offset, SEEK_SET);
        result = fread(destination, sizeof(boolean), 1, file);
        fseek(file, 0L, SEEK_SET);
    }
    fclose(file);
    return result;
}

int clear_navigator() {
    FILE* file_pointer;
    file_pointer = fopen("resources/navigator.txt", "w");
    if (file_pointer == NULL) {
        return FILE_NOT_FOUND;
    }
    fclose(file_pointer);
    return SUCCESS;
}
//
//long countSize() {
//    FILE* file_pointer = fopen("resources/navigator.txt", "r");
//    fseek(file_pointer, OL, SEEK_END);
//    long size = ftell(file_pointer);
//    return size;
//}

int count_blocks() {
    FILE* file_pointer;
    char* block_line = NULL;
    ssize_t read;
    size_t block_len = 0;
    int block_counter = 0;
    printf("process...\n");
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
    printf("METHOD started as a function\n");
    FILE *file_pointer;
    file_pointer = fopen("resources/dheap.txt", "a+");
    if (file_pointer == NULL) {
        printf("FILE CREATING ERROR\n");
        printf("%d\n", errno);
        return FILE_NOT_FOUND;
    }
    printf("inside of metohd\n");
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
    printf("%s\n", "part done");
    strcpy(current_data.pathToThisNode, necessaryPath);
    printf("%s\n", "part done");
    for (int i = 0; i < 10; i++) {
        current_data.children[i] = 7777;
    }
    printf("Type: %d\n", some_type->type);
    switch (some_type->type) {
        //current_data.pathToThisNode = necessaryPath;
        case my_int:;
            // TODO check process of writing integer type structure;
            current_data.data_type = 1;
            integer this_integer;
            this_integer.data_cell = some_type -> i;
            printf("for writing:\n");
            printf("path: %s, value: %d\n", this_integer.pathToThisNode, this_integer.data_cell);
            int r1 = fwrite(&this_integer, sizeof(integer), 1, file_pointer);
            int r2 = fwrite(&this_integer, sizeof(integer), 1, file_pointer);
            printf("r1: %d\n", r1);
            printf("r2: %d\n", r2);
            printf("DONE! Trying to read: \n");
            fclose(file_pointer);
            FILE* for_read = fopen("resources/dheap.txt", "r");
            integer read_data;
            while (fread(&read_data, sizeof(integer), 1, for_read)) {
                printf("trying...\n");
                printf("path: %s; value: %d\n", read_data.pathToThisNode, read_data.data_cell);
            }
            fclose(for_read);
            // FINISH OF STRUCTURE CREATING
            // SERIALIZING STRUCTURE TO FILE
            printf("Total Size of structure: %zu\n", sizeof(integer));
//            fprintf(file_pointer, "%d ", this_integer.data.data_type.data_type);
//            fprintf(file_pointer, "%s ", this_integer.data.pathToThisNode);
//            fprintf(file_pointer, "%llu ", this_integer.data.offset_of_current_block);
//            fprintf(file_pointer, "%llu ", this_integer.data.offset_of_previous_block);
//            fprintf(file_pointer, "%llu ", this_integer.data.offset_of_next_block);
//            fprintf(file_pointer, "%llu ", this_integer.data.offset_to_parent_node);
            for (int i = 0; i < 100; i++) {
//                fprintf(file_pointer,"%llu ", this_integer.data.children[i]);
            }
//            fprintf(file_pointer, "%d ", this_integer.data.isDirectory);
//            fprintf(file_pointer, "%d\n", this_integer.data_cell);
            fwrite(&this_integer, sizeof(integer), 1, file_pointer);
            break;
        case my_float:;
            // TODO check process of writing floating_type type structure;
            current_data.data_type = 2;
            floating_number this_floating_number;
            this_floating_number.data_cell = some_type -> f;
            // FINISH OF STRUCTURE CREATING
            // SERIALIZING STRUCTURE TO FILE
            fprintf(file_pointer, "%d ", this_floating_number.data_type);
            fprintf(file_pointer, "%s ", this_floating_number.pathToThisNode);
            fprintf(file_pointer, "%llu ", this_floating_number.offset_of_current_block);
            fprintf(file_pointer, "%llu ", this_floating_number.offset_of_previous_block);
            fprintf(file_pointer, "%llu ", this_floating_number.offset_of_next_block);
            fprintf(file_pointer, "%llu ", this_floating_number.offset_to_parent_node);
            for (int i = 0; i < 65536; i++) {
                fprintf(file_pointer,"%llu ", this_floating_number.children[i]);
            }
            fprintf(file_pointer, "%d ", this_floating_number.isDirectory);
            fprintf(file_pointer, "%f\n", this_floating_number.data_cell);
            break;
        case my_string:;
            // TODO check process of writing integer type structure;
            current_data.data_type = 3;
            string this_string;
            strcpy(this_string.data_cell, some_type -> s);
            //this_string.data_cell = some_type -> s;
            // FINISH OF STRUCTURE CREATING
            // SERIALIZING STRUCTURE TO FILE
//            fprintf(file_pointer, "%d ", this_string.data.data_type.data_type);
//            fprintf(file_pointer, "%s ", this_string.data.pathToThisNode);
//            fprintf(file_pointer, "%llu ", this_string.data.offset_of_current_block);
//            fprintf(file_pointer, "%llu ", this_string.data.offset_of_previous_block);
//            fprintf(file_pointer, "%llu ", this_string.data.offset_of_next_block);
//            fprintf(file_pointer, "%llu ", this_string.data.offset_to_parent_node);
            for (int i = 0; i < 65536; i++) {
                //fprintf(file_pointer,"%llu ", this_string.data.children[i]);
            }
//            fprintf(file_pointer, "%d ", this_string.data.isDirectory);
//            fprintf(file_pointer, "%s\n", this_string.data_cell);
            break;
        case my_boolean:;
            // TODO check process of writing integer type structure;
            current_data.data_type = 0;
            boolean this_boolean;
            this_boolean.data_cell = some_type -> b;
            // FINISH OF STRUCTURE CREATING
            // SERIALIZING STRUCTURE TO FILE
//            fprintf(file_pointer, "%d ", this_boolean.data.data_type.data_type);
//            fprintf(file_pointer, "%s ", this_boolean.data.pathToThisNode);
//            fprintf(file_pointer, "%llu ", this_boolean.data.offset_of_current_block);
//            fprintf(file_pointer, "%llu ", this_boolean.data.offset_of_previous_block);
//            fprintf(file_pointer, "%llu ", this_boolean.data.offset_of_next_block);
//            fprintf(file_pointer, "%llu ", this_boolean.data.offset_to_parent_node);
            for (int i = 0; i < 65536; i++) {
//                fprintf(file_pointer,"%llu ", this_boolean.data.children[i]);
            }
//            fprintf(file_pointer, "%d ", this_boolean.data.isDirectory);
//            fprintf(file_pointer, "%d\n", this_boolean.data_cell);

            break;
    }
    printf("method ended as a function.\n");
    return 0;
}

int delete_from_data_heap(int offset) {
    FILE *file_pointer;
    file_pointer = fopen("resources/data_heap.txt", "a+");
    if (file_pointer == NULL) {
        return FILE_NOT_FOUND;
    }
    fseek(file_pointer, offset, 0);

}
