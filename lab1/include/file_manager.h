//
// Created by Ivan Nesterov on 11/17/2022.
//

#ifndef LOW_LEVEL_PROGRAMMING_FILE_MANAGER_H
#define LOW_LEVEL_PROGRAMMING_FILE_MANAGER_H

long count_size_of_file(FILE* file);
bool is_chunk_exists(FILE* file, int offset_of_chunk);
int read_chunk_integer(long offset, integer* destination);
int read_chunk_floating_number(long offset, floating_number* destination);
int read_chunk_string(long offset, string* destination);
int read_chunk_boolean(long offset, boolean* destination);
int read_item_path(long offset, char* destination);
int write_to_navigator(char pathToFile[], int offset);
int clear_navigator();
int download_navigator();
int count_blocks();
int write_to_data_heap(struct some_type* some_type, char* necessaryPath);

#endif //LOW_LEVEL_PROGRAMMING_FILE_MANAGER_H
