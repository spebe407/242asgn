#ifndef MYLIB_H_
#define MYLIB_H_ 
#include <stdlib.h>
#include <stdio.h>
void *emalloc(size_t s);
int getword(char *s, int limit, FILE *stream);
#endif