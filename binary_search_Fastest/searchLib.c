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
    char *words[60000];
}dictionary;


dictionary* createDictionary(void);

void insertWord(dictionary *pdictionary, char *str);

int searchWord(dictionary *pdictionary, char *target);



int searchWord(dictionary *pdictionary, char *target)
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
            //printf(" ffffffffffffat location %d.\n",mid);
        mid = (curr_bottom + curr_top)/2;
        if (strcmp(pdictionary->words[mid], target) == 0){
           // printf("%s found at location %d.\n", target, mid+1);
            return 1;
        } else if (strcmp(pdictionary->words[mid], target) > 0){
            curr_top    = mid - 1;
        } else if (strcmp(pdictionary->words[mid], target) < 0){
            curr_bottom = mid + 1;
        }
    }
    if(strlen(target) == 4){
        char exist_str[10];
        strncpy(exist_str, pdictionary->words[mid+1], strlen(target));
        if(strcmp(exist_str,target)!=0) 
        {
           //printf("[mid-1]:%s\t\t\t, [mid]:%s,\t\t\t [mid+1]:%s\t\t trgt:,\t%s  \n",pdictionary->words[mid-1],pdictionary->words[mid],pdictionary->words[mid+1],target);
            
        return -1;
        } 
        else return 0;
        //else if (!strcmp("uuuu",target)) printf ("found");
              
    }
    
    return 0;
}


void insertWord(dictionary *pdictionary, char *str)
{   
    if (pdictionary == NULL || str == NULL)
    {
        return;
    }
    
    //    #pragma omp critical {
     //printf("from ins %d   \n" ,pdictionary->size);
     pdictionary->size++; 
     pdictionary->words[pdictionary->size]= (char *) malloc(strlen(str) * sizeof(char));
     strcpy(pdictionary->words[pdictionary->size], str);
     //printf("from insert func : %s %p %d\n", pdictionary->words[pdictionary->size], pdictionary->words[pdictionary->size] ,pdictionary->size );
    
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

    pdictionary->size = -1;

    return pdictionary;
}



#endif