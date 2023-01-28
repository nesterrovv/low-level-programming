#ifndef STRING_TOOLS_H
#define STRING_TOOLS_H

#include <stdbool.h>
#include "add_command.h"
#include "basic_interface.h"

char *concat(const char *s1, const char *s2);
bool isNumeric(const char *str);
size_t split(char *str, char c, char ***arr);
void parse_file(FILE *to, FILE *from);

#endif //STRING_TOOLS_H
