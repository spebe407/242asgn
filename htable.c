#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "htable.h"
#include "mylib.h"

struct htablerec {
    int capacity;
    int num_keys;
    int* frequencies;
    char** keys;
    int* stats;
    hashing_t method;
};

void htable_free(htable h) {
    for(int i = 0; i < h->capacity; i++) {
        if(h->keys[i] != NULL) { 
            free(h->keys[i]);
        }
    } 

    free(h->frequencies);
    free(h->keys);
    free(h);
}



int htable_insert(htable h, char *str) {
    int collisions = 0;
    unsigned int key = htable_word_to_int(str);
    unsigned int index = key % h->capacity;
    unsigned int step = (h->method == LINEAR_P) ? 1 : htable_step(h, key);

    while(h->keys[index] != NULL && strcmp(str, h->keys[index]) != 0 && index != h->capacity) {
        index = (index + step) % h->capacity;
        collisions++;
    }

    if(h->keys[index] == NULL) {
        h->keys[index] = emalloc((strlen(str) + 1) * sizeof(char));
        strcpy(h->keys[index], str);
        h->frequencies[index] = 1;
        h->num_keys++;
        h->stats[h->num_keys] = collisions;
        return 1;    
    } else {
        h->frequencies[index]++;
        return h->frequencies[index];
    }

    if(collisions == h->capacity) {
        return 0;
    }
}

htable htable_new(int capacity, hashing_t h_type) {
    htable result = emalloc(sizeof *result);
    result->capacity = capacity;
    result->num_keys = 0;
    result->method = h_type;
    result->frequencies = emalloc(result->capacity * sizeof(result->frequencies[0]));
    result->keys = emalloc(result->capacity * sizeof(result->keys[0]));
    result->stats = emalloc(result->capacity * sizeof(result->stats[0]));

    for(int i = 0; i < result->capacity; i++) {
        result->frequencies[i] = 0;
        result->keys[i] = NULL;
    }

    return result;
}

void htable_print(htable h, FILE *stream) {
    for(int i = 0; i < h->capacity; i++) {
        if(h->keys[i] != NULL) {
            printf("%d %s\n", h->frequencies[i], h->keys[i]);
        }
    }
}

int htable_search(htable h, char *str) {
    int collisions = 0;
    unsigned int key = htable_word_to_int(str);
    unsigned int index = key % h->capacity;
    unsigned int step = htable_step(h, key);

    while(h->keys[index] != NULL && strcmp(str, h->keys[index]) != 0 && index != h->capacity) {
        index = (index + step) % h->capacity;
        collisions++;
    }   

    if(collisions == h->capacity) {
        return 0;
    }

    return h->frequencies[index];
}

static unsigned int htable_word_to_int(char *word) {
    unsigned int result = 0;
    
    while (*word != '\0') {
        result = (*word++ + 31 * result);   
    }

    return result;
}

static unsigned int htable_step(htable h, unsigned int i_key) {
    return 1 + (i_key % (h->capacity - 1));
}

/*int main(void) {
    htable h = htable_new(18143);
    char word[256];
    
    int insertions = 0;
    while (getword(word, sizeof word, stdin) != EOF) {
        htable_insert(h, word);

        insertions++;
    }
    
    printf("Unique Keys: %d\n", h->num_keys);
    int f = htable_search(h, "the");
    printf("the: %d", f);
    htable_free(h);
    
    return EXIT_SUCCESS;
}*/