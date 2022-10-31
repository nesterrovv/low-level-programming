#include <stdio.h>
#include <assert.h>
#include "../include/structures.h"
#include "../include/hashset.h"

int main(int argc, char *argv[]) {
    if (argc == 0) {
        printf("Filepath is not defined. You should define it as a command line argument.\n");
        return 1;
    }
    char* filepath = argv[1];
    char *foo = "foo";
    char *missing = "missing";
    hashset_t set = hashset_create();
    if (set == NULL) {
        fprintf(stderr, "failed to create hashset instance\n");
        abort();
    }
    hashset_add(set, foo);
    assert(hashset_is_member(set, foo) == 1);
    assert(hashset_is_member(set, missing) == 0);
    int a = hashset_is_member(set, foo);
    printf("%d\n", a);
    int b = hashset_is_member(set, missing);
    printf("%d\n", b);
}