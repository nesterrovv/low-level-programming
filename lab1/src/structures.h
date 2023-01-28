#ifndef LLP_STRUCTURES_H
#define LLP_STRUCTURES_H

#include <stdio.h>
#include <inttypes.h>
#include "utils.h"

union header_tuple {
    struct {
        uint64_t next;
        uint64_t previous;
    };
    struct {
        uint64_t original;
        uint64_t buff;
    };
};

/**
 * Struct for file parameters
 * signature_of_ASCII - definition of little/big endian
 * curr_id      - sequence for checking current id
 * sequence_first  - sequence that increases after opening file
 * sequence_second - sequence that increases after closing file
 * pattern_size    - quantity of fields in template
 */
struct tree_subheader {
    uint64_t signature_of_ASCII;
    uint64_t cur_id;
    uint64_t root_offset;
    uint64_t sequence_first;
    uint64_t sequence_second;
    uint64_t pattern_size;
};

enum tree_subheader_parameter {
    PAR_ASCII_SIGNATURE = 0,
    PAR_ROOT_OFFSET,
    PAR_FIRST_SEQ,
    PAR_SECOND_SEQ,
    PAR_CURRENT_ID,
    PAR_PATTERN_SIZE
};

/**
 * For key template
 */
//#pragma pack(push, 4)
struct key_header {
    uint32_t size;
    uint32_t type;
};
struct key {
    struct key_header *header;
    char *key_value;
};
//#pragma pack(pop)

/**
 * Full header of the file where sequence_id is array of pointers to tuples
 */
struct header_of_tree {
    struct tree_subheader *subheader;
    struct key **pattern;
    uint64_t *id_sequence;
};


struct my_tuple {
    union header_tuple header;
    uint64_t *data;
};


struct document_tree {
    struct header_of_tree header;
    struct my_tuple *tuples;
};


#endif //LLP_STRUCTURES_H
