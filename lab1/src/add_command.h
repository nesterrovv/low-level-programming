#ifndef ADD_COMMAND_H
#define ADD_COMMAND_H

#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>
#include <time.h>
#include "string_tools.h"

size_t add_input_item(FILE *f, char **str, size_t pattern_size, const uint32_t *pattern_types, char **pattern_names);

#endif //ADD_COMMAND_H
