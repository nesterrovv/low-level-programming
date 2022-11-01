#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include "../include/structures.h"
#include "../include/hashset.h"
#include "../include//hashset_itr.h"

// function for deleting unnecessary space symbols at the beginning and/or ending of line
void trim (char *dest, char *src);
// function to casting content of all line to upper case
void to_upper_case(char* temp);

// argv[1] - filepath to data file (expected of it)
int main(int argc, char *argv[]) {
    if (argc == 0) {
        printf("Filepath is not defined. You should define it as a command line argument.\n");
        return 1;
    }
    char* filepath = argv[1];
    // user greeting
    fprintf(stdout, "\"Big data\" manager has been successfully started!\n");
    fprintf(stdout, "Which data manipulation do you wish to run?\n");
    fprintf(stdout, "Variants:\n");
    fprintf(stdout, "  1. READING;\n");
    fprintf(stdout, "  2. DELETING;\n");
    fprintf(stdout, "  3. WRITING;\n");
    fprintf(stdout, "  4. EDITING.\n");
    fprintf(stdout,"You should type number of operation or it's name here.\n");
    char enter_before_trim[1024];
    char enter_after_trim [1024];
    printf ("Type a string : ");
    fgets (enter_before_trim, 1024, stdin);
    trim(enter_after_trim, enter_before_trim);
    to_upper_case(enter_after_trim);
    printf("Your input: \'%s\'.\n", enter_after_trim);
    switch (enter_after_trim) {
        case "READING":
        case "1":
            // TODO: add read function call
            break;
        case "DELETING":
        case "2":
            // TODO: add delete function call
            break;
        case "WRITING":
        case "3"
            // TODO: add write function call
            break;
        case "EDITING":
        case "4"
            // TODO add edit function call
            break;
        default:
            printf("%s", "Incorrect enter. Please, try again.")
            break;
    }
    // --- testing of hashset implementation ---
//    char *foo = "foo";
//    char *missing = "missing";
//    hashset_t set = hashset_create();
//    if (set == NULL) {
//        fprintf(stderr, "failed to create hashset instance\n");
//        abort();
//    }
//    hashset_add(set, foo);
//    assert(hashset_is_member(set, foo) == 1);
//    assert(hashset_is_member(set, missing) == 0);
//    int a = hashset_is_member(set, foo);
//    printf("%d\n", a);
//    int b = hashset_is_member(set, missing);
//    printf("%d\n", b);
}

void trim (char *dest, char *src)
{
    if (!src || !dest)
        return;

    int len = strlen (src);

    if (!len) {
        *dest = '\0';
        return;
    }
    char *ptr = src + len - 1;

    // remove trailing whitespace
    while (ptr > src) {
        if (!isspace (*ptr))
            break;
        ptr--;
    }

    ptr++;

    char *q;
    // remove leading whitespace
    for (q = src; (q < ptr && isspace (*q)); q++)
        ;

    while (q < ptr)
        *dest++ = *q++;

    *dest = '\0';
}

void to_upper_case(char* temp) {
    char * name;
    name = strtok(temp,":");

    // Convert to upper case
    char *s = name;
    while (*s) {
        *s = toupper((unsigned char) *s);
        s++;
    }

}