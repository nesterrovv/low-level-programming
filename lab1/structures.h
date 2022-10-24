//
// Created by Ivan Nesterov on 10/18/2022.
//

#ifndef LOW_LEVEL_PROGRAMMING_STRUCTURES_H
#define LOW_LEVEL_PROGRAMMING_STRUCTURES_H

#endif //LOW_LEVEL_PROGRAMMING_STRUCTURES_H

#include <stdint.h>

#define DATA_TYPE_BIT_SIZE  2
#define DATA_BIT_SIZE_FOR_BOOLEAN 1
#define REQUEST_BIT_SIZE 2

typedef struct {
    /*
     * Field for storing data type. Size of this field is 2 bits, where
     * 00 - boolean variables
     * 01 - integer numbers
     * 10 - numbers with floating points
     * 11 - strings
     */
    unsigned int data_type : DATA_TYPE_BIT_SIZE;
} data_type;

typedef struct {
    /*
     * Child element of root element in my document tree for storing
     * abstract info about data's blocks, where:
     */
    data_type data_type;                // root element for defining data type
    uint64_t hash_of_current_block;     // hash for unambiguous definition of this data block
    uint64_t hash_of_next_block;        // hash for unambiguous definition of continuation of this data block
    // (for case when data have not been stored in one data block)
} data;

typedef struct {
    /*
     * Child element of root element in my document tree for storing
     * integer numbers, where:
     */
    data data;       // abstract data block description
    int32_t data_cell;      // cell for clear data, 32-bit signed number (like in tech task).
    // May be extended via using hash to next data block (optional).
} Integer;

typedef struct {
    /*
     * Child element of root element in my document tree for storing
     * numbers with floating points, where:
     */
    data data;   // abstract data block description
    double data_cell;   // cell for clear data, 64-bit signed floating numbers
} floating_number;

typedef struct {
    /*
     * Child element of root element in my document tree for storing
     * boolean data type, where:
     */
    data data;                       // abstract data block description
    int data_cell : DATA_BIT_SIZE_FOR_BOOLEAN;  // cell for clear data, 1-bit number (0 == false, 1 == true)
} boolean;

typedef struct {
    /*
     * Child element of root element in my document tree for storing
     * strings, where:
     */
    data data;   // abstract data block description
    char data_cell[];   // cell for clear data, char[] array
} string;

typedef struct {
    /*
     * Info about request. 00 - create new block, 01 - read existing block
     * 10 - update existing block, 11 - delete existing block
     * Data for manipulation will be requested additionally
     */
    unsigned int request_type : REQUEST_BIT_SIZE;
} request;