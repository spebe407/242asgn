#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <getopt.h>
#include <time.h>
#include "htable.h"
#include "mylib.h"


int main(int argc, char* argv[]) {
    htable dictionary;
    char word[256];

    clock_t start, end;
    double fillTime = 0.0;
    double searchTime = 0.0;
    int unknownWords = 0;
                                           /*max path size on linux*/
    char *filename = emalloc(sizeof(char) * (4096 + 1)); 
    hashing_t hashing_method = LINEAR_P;
    int table_size = 113;

    bool c_flag = false;
    bool e_flag = false;
    bool p_flag = false;
    bool s_flag = false;
    bool h_flag = false;
    
    int snapshots = 0;
    int length = 0;
    char option;
    while((option = getopt(argc, argv, "c:deps:t:h")) != EOF) {
        switch(option) {
            case 'c':
                c_flag = true;
                filename = optarg;
                break;
            case 'd':
                hashing_method = DOUBLE_H; /*double check*/
                break;
            case 'e':
                e_flag = true;
                break;
            case 'p':
                p_flag = true;
                break;
            case 's':
                s_flag = true;
                snapshots = atoi(optarg);
                break;
            case 't':
                
                break;
            case 'h':
                h_flag = true;
                break;
            default: 
                printf("Unknown input: -%c %s", option, optarg);
                return 1;
        }
    }

    if(h_flag) {
        fprintf(stdout,
        "Usage: ./sample-asgn [OPTION]... <STDIN>\n\n"
        "Perform various operations using a hash table.  By default, words are\n"
        "read from stdin and added to the hash table, before being printed out\n"
        "alongside their frequencies to stdout.\n\n"

        "-c FILENAME  Check spelling of words in FILENAME using words\n"
        "             from stdin as dictionary.  Print unknown words to\n"
        "             stdout, timing info & count to stderr (ignore -p)\n"
        "-d           Use double hashing (linear probing is the default)\n"
        "-e           Display entire contents of hash table on stderr\n"
        "-p           Print stats info instead of frequencies & words\n" 
        "-s SNAPSHOTS Show SNAPSHOTS stats snapshots (if -p is used)\n" 
        "-t TABLESIZE Use the first prime >= TABLESIZE as htable size\n\n" 

        "-h           Display this message");
        return EXIT_SUCCESS;
    }
    
    dictionary = htable_new(table_size, hashing_method);

    start = clock();
    while(getword(word, sizeof(word), stdin) != EOF) {
        htable_insert(dictionary, word);
        length++;
    }
    end = clock();
    fillTime  = ((double) (end - start)) / CLOCKS_PER_SEC;

    if(e_flag) {
        htable_print_entire_table(dictionary);
    }

    if(p_flag && s_flag) {
                                       /*stderr?*/ 
        htable_print_stats(dictionary, stdout, snapshots);
    } else if(p_flag) {
        htable_print_stats(dictionary, stdout, 10);
    }
   
    if(c_flag) {
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

        
        fprintf(stderr, "Fill time     : %f\n", fillTime);
        fprintf(stderr, "Search time   : %f\n", searchTime);
        fprintf(stderr, "Unknown words = %d\n", unknownWords);
    }

    return EXIT_SUCCESS;
}
