#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include "mylib.h"

void *emalloc(size_t s) {
    void *result = malloc(s);
    if(NULL == result) {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(EXIT_FAILURE);
    }

    return result;
}

int getword(char *s, int limit, FILE *stream) {
    int c;
    char *w = s;
    assert(limit > 0 && s != NULL && stream != NULL);
    /* skip to the start of the word */
    while (!isalnum(c = getc(stream)) && EOF != c);
    
    if (EOF == c) {
        return EOF;
    } else if (--limit > 0) { /* reduce limit by 1 to allow for the \0 */
        *w++ = tolower(c);
    }

    while (--limit > 0) {
        if (isalnum(c = getc(stream))) {
            *w++ = tolower(c);
        } else if ('\'' == c) {
            limit++;
        } else {
            break;
        }
    }
    *w = '\0';
    
    return w - s;
}

int is_prime(int x){
    int i = 2;
    for(i = 2;  i < x; i++){
        if(x % i == 0){
            return 0;
        }
    }
    return 1;
}

int next_prime(int n) {
    int x = n;
    if(is_prime(x)) {
        return x;
    }
    
    return next_prime(x + 1);
}
