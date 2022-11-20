//
// Created by Ivan Nesterov on 11/17/2022.
//

#ifndef LOW_LEVEL_PROGRAMMING_FILE_MANAGER_H
#define LOW_LEVEL_PROGRAMMING_FILE_MANAGER_H

int write_to_navigator(char pathToFile[], int offset);
int clear_navigator();
int download_navigator();
int count_blocks();

#endif //LOW_LEVEL_PROGRAMMING_FILE_MANAGER_H
