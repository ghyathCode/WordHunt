#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <omp.h>
#include <stdbool.h>

#define SPELL_CHECKER

#ifdef SPELL_CHECKER

typedef struct DICTIONARY
{
    int size ;   
    char *words[16];
}dictionary;


dictionary* createDictionary(void);

void insertWord(dictionary *pdictionary, int size, char *str);

_Bool searchWord(dictionary *pdictionary, char *target);

_Bool searchWord(dictionary *pdictionary, char *target)
{

    /* error condition check */
    if (pdictionary == NULL || target == NULL)
    {
        fprintf(stderr, " -- invalid pointer to string or hashtable -- \n");
        return false;
    }
    int curr_bottom= 0;
    int mid;
    int curr_top = pdictionary->size - 1;
    
    //search a part of the word before ...

    while(curr_bottom <= curr_top){
        mid = (curr_bottom + curr_top)/2;
        if (strcmp(pdictionary->words[mid], target) == 0){
            printf("%s found at location %d.\n", target, mid+1);
            return true;
        } else if (strcmp(pdictionary->words[mid], target) > 0){
            curr_top    = mid - 1;
        } else if (strcmp(pdictionary->words[mid], target) < 0){
            curr_bottom = mid + 1;
        }
    }
    return false;

}

void insertWord(dictionary *pdictionary, int size, char *str)
{   
    if (pdictionary == NULL || str == NULL)
    {
        return;
    }
    
    //    #pragma omp critical {
    
    printf("from ins %d   \n" ,pdictionary->size);
    pdictionary->words[pdictionary->size]= (char *) malloc(strlen(str) * sizeof(char));
    pdictionary->words[pdictionary->size] = str;
    printf("from insert func : %s %p %d\n", pdictionary->words[pdictionary->size], pdictionary->words[pdictionary->size] ,pdictionary->size );
    
    //printf("from insert func : %s %p %d\n", pdictionary->words[x], pdictionary->words[x] ,x );
    // }
    return;

}

dictionary* createDictionary()
{
    int i;

    dictionary *pdictionary = (dictionary *)malloc(sizeof(dictionary));

    if (pdictionary == NULL)
    {
        fprintf(stderr, "memory allocation failed for dictionary...\n");
    }

    pdictionary->size = 0;

    return pdictionary;
}



#endif