#include "basic_file_manager.h"

enum status_of_read_operation read_file( FILE *file, void *buffer,size_t size) {
    size_t length = fread(buffer, size, 1, file);
    enum status_of_read_operation code = IS_READ_OK;
    if (length < 1) {
        code = IS_READ_INVALID;
    } else if (sizeof(buffer) < length) {
        code = IS_END_OF_FILE;
    }
    return code;
}

enum status_of_write_operation write_file(FILE *file, void *buffer, size_t size) {
    size_t length = fwrite(buffer, size, 1, file);
    enum status_of_write_operation code = IS_WRITE_OK;
    if (length < 1) {
        code = IS_WRONG_INTEGRITY;
    } else if (sizeof(buffer) < length) {
        code = IS_WRITE_INVALID;
    }
    return code;
}

static enum status_of_open_operation open_file(char *filename, FILE **file, char *open_descriptor){
    *file = fopen(filename, open_descriptor);
    enum status_of_open_operation code = IS_OPEN_OK;
    if (*file==NULL){
        code = IS_OPEN_FAILED;
    }
    return code;
}

enum status_of_open_operation open_existing_file(char *filename, FILE **file){
    return open_file(filename, file, "r+b");
}

enum status_of_open_operation open_parse_file(char *filename, FILE **file){
    return open_file(filename, file, "r");
}

enum status_of_open_operation create_new_file(char *filename, FILE **file){
    return open_file(filename, file, "w+b");
}

void close_file(FILE *file){
    fclose(file);
}

enum status_of_open_operation close_empty_file(char *filename, FILE **file){
    open_file(filename, file, "w");
    close_file(*file);
    return open_file(filename, file, "r+b");
}
