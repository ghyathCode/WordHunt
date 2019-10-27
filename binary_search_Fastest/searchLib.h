#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef HASH   
#define HASH

#define SPELL_CHECKER

#ifdef SPELL_CHECKER
typedef struct DICTIONARY
{
    int size ;   
    char *words[60000];
}dictionary;


dictionary* createDictionary(void);

void insertWord(dictionary *pdictionary, char *str);

int searchWord(dictionary *pdictionary, char *target); // can return index of the word in the dictionary

#endif
#endif 
