#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <getopt.h>
#include <time.h>
#include "htable.h"
#include "mylib.h"


int main(int argc, char* argv[]) {
    char *filename;
    hashing_t hashing_method = LINEAR_P;
    int table_size = 113;

    bool e_flag = false;
    bool p_flag = false;
    bool s_flag = false;
    bool h_flag = false;

    char option;
    while((option = getopt(argc, argv, "c:")) != EOF) {
        switch(option) {
            case 'c':
                filename = optarg;
                break;
            /* case 'd': */
            /*     hashing_method = DOUBLE_H; */
            /* case 'e': */
            /*     /\*Display hash table on stderr*\/ */
            /*     e_flag = true; */
            /* case 'p':  */
            /*     /\*Print stats instead of printing freqs & words*\/ */
            /*     p_flag = true; */
            /* case 's': */
            /*     /\*Display up to the given value of stats snapshots */
            /*     s_flag = true; */
            /* case 't': */
            /*     table_size = atoi(optarg); */
            default: 
                printf("Unknown input: -%c %s", option, optarg);
                return 1;
        }
    }

    htable dictionary = htable_new(table_size, hashing_method);
    char word[256];

    clock_t start, end;
    double fillTime;
    double searchTime;
    int unknownWords = 0;
    
    start = clock();
    while(getword(word, sizeof(word), stdin) != EOF) {
        htable_insert(dictionary, word);
    }
    end = clock();
    fillTime  = ((double) (end - start)) / CLOCKS_PER_SEC;
    
    
    if(filename != NULL) {
        FILE* filePointer = fopen(filename, "r");

        start = clock();
        while(getword(word, sizeof(word), filePointer) != EOF) {
            int result = htable_search(dictionary, word);
            if(result == 0) {
                fprintf(stdout, "%s\n", word);
                unknownWords++;
            }
        }
        
        end = clock();
        searchTime = ((double) (end - start)) / CLOCKS_PER_SEC;
        
        fprintf(stderr, "Fill time     : %lf\n", fillTime);
        fprintf(stderr, "Search time   : %lf\n", searchTime);
        fprintf(stderr, "Unknown words = %d\n", unknownWords);
    }
}
