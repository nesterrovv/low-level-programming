#ifndef FIND_BY_COMMAND_H
#define FIND_BY_COMMAND_H

#include <stdbool.h>
#include "basic_interface.h"
#include "string_tools.h"

void find_by(FILE *f, char **arr, size_t pattern_size, const uint32_t *pattern_types, char **pattern_names, size_t count);

#endif //FIND_BY_COMMAND_H
