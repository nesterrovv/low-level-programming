#ifndef UPDATE_COMMAND_H
#define UPDATE_COMMAND_H

#include "string_tools.h"

size_t update_item(FILE *f, char **str, size_t pattern_size, const uint32_t *pattern_types, char **pattern_names, size_t fields_count);

#endif //UPDATE_COMMAND_H