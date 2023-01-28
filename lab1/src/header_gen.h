#ifndef HEADER_GENERATOR_H
#include "utils.h"
#include "structures.h"
#include "basic_file_manager.h"
#include <stdlib.h>

void gen_empty_tree_header(char **pattern, uint32_t *types,size_t pattern_size, size_t *key_sizes, struct header_of_tree *header);

#endif //HEADER_GENERATOR_H
