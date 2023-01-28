#ifndef LAB1_FILE_MANAGER_H
#define LAB1_FILE_MANAGER_H

#include <stdio.h>
#include <inttypes.h>

enum status_of_read_operation {
    IS_READ_OK = 0,
    IS_END_OF_FILE,
    IS_READ_INVALID
};

enum status_of_write_operation {
    IS_WRITE_OK = 0,
    IS_WRONG_INTEGRITY,
    IS_WRITE_INVALID
};

enum status_of_open_operation {
    IS_OPEN_OK = 0,
    IS_OPEN_FAILED
};

enum status_of_open_operation open_parse_file(char *filename, FILE **file);
enum status_of_read_operation read_file(FILE *file, void *buffer, size_t size);
enum status_of_write_operation write_file(FILE *file, void *buffer, size_t size);
enum status_of_open_operation open_existing_file(char *filename, FILE **file);
enum status_of_open_operation create_new_file(char *filename, FILE **file);
enum status_of_open_operation close_empty_file(char *filename, FILE **file);
void close_file(FILE *file);

#endif //LAB1_FILE_MANAGER_H