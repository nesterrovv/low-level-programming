//
// Created by Ivan Nesterov on 10/18/2022.
//

#ifndef LOW_LEVEL_PROGRAMMING_STRUCTURES_H
#define LOW_LEVEL_PROGRAMMING_STRUCTURES_H

#include <stdint.h>
#include <stdint.h>
#include <stdbool.h>

#define DATA_TYPE_BIT_SIZE  2
#define DATA_BIT_SIZE_FOR_BOOLEAN 1
#define REQUEST_BIT_SIZE 2
#define PATH_SIZE 260

struct {
    /*
     * Field for storing data type, where
     * 0 - boolean variables
     * 1 - integer numbers
     * 2 - numbers with floating points
     * 3 - strings
     */
    unsigned int data_type;
} data_type;

typedef struct {
    /*
     * Child element of root element in my document tree for storing
     * abstract info about data's blocks, where:
     */
    /*
     * Field for storing data type, where
     * 0 - folder
     * 1 - integer numbers
     * 2 - numbers with floating points
     * 3 - strings
     * 4 - boolean
     */
    unsigned int data_type;
    char pathToThisNode[1024];          // unix-like path to this node in document tree
    uint64_t offset_of_current_block;   // hash for unambiguous definition of this data block
    uint64_t offset_of_previous_block;  // hash for unambiguous definition of previous part of this data block
    uint64_t offset_of_next_block;      // hash for unambiguous definition of continuation of this data block
                                        // (for case when data have not been stored in one data block)
    uint64_t offset_to_parent_node;
    uint64_t children[10];           // array of offsets to children nodes
    bool isDirectory;
} data;

typedef struct {
    /*
     * Child element of root element in my document tree for storing
     * integer numbers, where:
     */
    unsigned int data_type;
    char pathToThisNode[1024];          // unix-like path to this node in document tree
    uint64_t offset_of_current_block;   // hash for unambiguous definition of this data block
    uint64_t offset_of_previous_block;  // hash for unambiguous definition of previous part of this data block
    uint64_t offset_of_next_block;      // hash for unambiguous definition of continuation of this data block
    // (for case when data have not been stored in one data block)
    uint64_t offset_to_parent_node;
    uint64_t children[10];           // array of offsets to children nodes
    bool isDirectory;
    int32_t data_cell;      // cell for clear data, 32-bit signed number (like in tech task).
    // May be extended via using hash to next data block (optional).
} integer;

typedef struct {
    unsigned int data_type;
    char pathToThisNode[1024];          // unix-like path to this node in document tree
    uint64_t offset_of_current_block;   // hash for unambiguous definition of this data block
    uint64_t offset_of_previous_block;  // hash for unambiguous definition of previous part of this data block
    uint64_t offset_of_next_block;      // hash for unambiguous definition of continuation of this data block
    // (for case when data have not been stored in one data block)
    uint64_t offset_to_parent_node;
    uint64_t children[10];           // array of offsets to children nodes
    bool isDirectory;
    float data_cell;   // cell for clear data, 64-bit signed floating numbers
} floating_number;

typedef struct {
    unsigned int data_type;
    char pathToThisNode[1024];          // unix-like path to this node in document tree
    uint64_t offset_of_current_block;   // hash for unambiguous definition of this data block
    uint64_t offset_of_previous_block;  // hash for unambiguous definition of previous part of this data block
    uint64_t offset_of_next_block;      // hash for unambiguous definition of continuation of this data block
    // (for case when data have not been stored in one data block)
    uint64_t offset_to_parent_node;
    uint64_t children[10];           // array of offsets to children nodes
    bool isDirectory;
    bool data_cell : DATA_BIT_SIZE_FOR_BOOLEAN;  // cell for clear data, 1-bit number (0 == false, 1 == true)
} boolean;

typedef struct {
    unsigned int data_type;
    char pathToThisNode[1024];          // unix-like path to this node in document tree
    uint64_t offset_of_current_block;   // hash for unambiguous definition of this data block
    uint64_t offset_of_previous_block;  // hash for unambiguous definition of previous part of this data block
    uint64_t offset_of_next_block;      // hash for unambiguous definition of continuation of this data block
    // (for case when data have not been stored in one data block)
    uint64_t offset_to_parent_node;
    uint64_t children[10];           // array of offsets to children nodes
    bool isDirectory;
    char data_cell[4];   // cell for clear data, char[] array
} string;

enum type { my_int, my_float, my_boolean,  my_string };

struct some_type {
    enum type type;
    union {
        uint32_t            i;  // when integer
        double              f;  // when double
        char*               s;  // when string
        bool                b;  // when boolean
    };
};

//typedef struct {
//    /*
//     * Info about request. 00 - create new block, 01 - read existing block
//     * 10 - update existing block, 11 - delete existing block
//     * Data for manipulation will be requested additionally
//     */
//    unsigned int request_type : REQUEST_BIT_SIZE;
//    char path[PATH_SIZE];
//    char user_data[]; // necessary for CREATE, UPDATE operations, empty for READ and DELETE
//} request;

#endif //LOW_LEVEL_PROGRAMMING_STRUCTURES_H