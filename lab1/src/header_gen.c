#include "header_gen.h"

static void generate_empty_tree_subheader(struct tree_subheader *subheader, size_t pattern_size) {
    subheader->pattern_size = (uint64_t) pattern_size;
    subheader->cur_id = NULL_VALUE;
    subheader->signature_of_ASCII = BIG_ENDIAN_SIGNATURE;
    subheader->root_offset = NULL_VALUE;
    subheader->sequence_second = NULL_VALUE;
    subheader->root_offset = NULL_VALUE;
}

static void copy_string(char *from, char *to, size_t size_from, size_t size_to){
    while(size_to-- && size_from--) *(to++) = *(from++);
}

static void generate_empty_pattern(struct key **key_pattern, char **pattern, uint32_t *types,size_t pattern_size, size_t *key_sizes) {
    struct key *pattern_key;
    size_t real_size;
    for (size_t iter = pattern_size; iter-- > 0; key_pattern++ && pattern++ && types++ && key_sizes++) {
        pattern_key = malloc_test(sizeof(struct key));
        real_size = (*key_sizes)/FILE_GRANULARITY*FILE_GRANULARITY + ( (*key_sizes)%FILE_GRANULARITY ? FILE_GRANULARITY : 0);
        char *appended_string = malloc_test(sizeof(char) * real_size);
        copy_string(*pattern, appended_string, *key_sizes, real_size);
        pattern_key->key_value = appended_string;
        pattern_key->header = malloc_test(sizeof(struct key_header));
        pattern_key->header->size = (uint32_t) real_size;
        pattern_key->header->type = *types;
        *key_pattern = pattern_key;
    }
}

void gen_empty_tree_header(char **pattern, uint32_t *types,size_t pattern_size, size_t *key_sizes, struct header_of_tree *header) {
    header->subheader = malloc_test(sizeof(struct tree_subheader));
    generate_empty_tree_subheader(header->subheader, pattern_size);

    header->pattern = malloc_test(sizeof(struct key*) * pattern_size);
    generate_empty_pattern(header->pattern, pattern, types, pattern_size, key_sizes);

    size_t array_size = get_info_array_size(header->subheader->pattern_size, header->subheader->cur_id);
    header->id_sequence = malloc_test(sizeof(uint64_t) * array_size);
    for(size_t iter = 0; iter < array_size; iter++){
        header->id_sequence[iter] = 0;
    }
}


