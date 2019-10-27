#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <omp.h>
#include "searchLib.h"

typedef struct coordinate
{
    int x;
    int y;
}coordinate;

int resTop=-1;
char *results[500];
int results_len[500];

coordinate pop(coordinate *coordinateArr, int *top);
void push(coordinate coordinateArr[], int *top, coordinate coor);
const char *strReturn(char game_grid[4][4], coordinate str_coor_stack[], int top);
void doSearch(char game_grid[4][4], dictionary *pdictionary, coordinate coor, coordinate str_coor_stack[], int top, int my_rank,int thread_count);
_Bool InsideStack(coordinate coor, coordinate str_coor_stack[]);
_Bool notInsideResults(char current[]);
int lenOfStack(coordinate str_coor_stack[]);


int main(int argc, char *argv[])
{       
    int thread_count ;
    clock_t start = clock();
    clock_t end;
    dictionary *pdictionary;    
    FILE * fp;
    char line [32];
    char maxstr[32];
    
    char game_grid[4][4]={ //mimlenmek
        {'l','o','o','n'},
        {'m','l','d','s'},
        {'s','e','f','r'},
        {'y','m','a','o'},
    };
    unsigned int max=0;
    int i =0,j =0, k=0;
    coordinate str_coor_stack[32], coor_def = {-1,-1};    
    coordinate str_coor_stack_def[32];    
    
    pdictionary = createDictionary();
    
    fp = fopen("sozluk1.txt", "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

   //#pragma omp parallel 
     while (fscanf(fp, "%s", line)!=EOF){
      //  int my_rank = omp_get_thread_num();
      //  thread_count = omp_get_num_threads();
      //  printf("%d    %d thread is working.\n", my_rank,thread_count);
             //printf("b %p   %d\n", fp,size);
//        fscanf(fp, "%s", line);
//        if(max<strlen(line))
//        {   max=strlen(line);
//            strcpy(maxstr,line);
//        }
         insertWord(pdictionary, line) ;
//             ->size++;
//         pdictionary->words[pdictionary->size]= (char *) malloc(strlen(line) * sizeof(char));
//         strcpy(dictionary[pdictionary->size], line);   
//                         
             //printf("aaa  %s  \n", line);
    }
    fclose(fp);
    //printf("%d is max line of length.\n", max);    
    end = clock();
    printf("|||||||||||||||||||||||||||||||| time passed end of dict is %f\n", (double)(end - start)/(CLOCKS_PER_SEC*3)); 
    start = clock();

    coordinate coor;
              
       for(k=0;k<16;k++){  str_coor_stack_def[k] = coor_def; }   
  #pragma omp parallel for default(none) private(i,j,coor,coor_def,str_coor_stack) shared(game_grid,thread_count, pdictionary, str_coor_stack_def) schedule(static,1)
   for (i=0;i<4;i++){
       int top = -1;
       thread_count = omp_get_num_threads();     
       int my_rank = omp_get_thread_num();
       memcpy(&str_coor_stack, &str_coor_stack_def, sizeof(str_coor_stack_def));
       int j,k;
           for (j=0;j<4;j++){
               coor.x = i; coor.y = j;
               printf("%d ......  %d   ******************* %d\n", coor.x,coor.y,my_rank);        
               doSearch(game_grid, pdictionary, coor, str_coor_stack, top, my_rank, thread_count);  
           }  
    }
    end = clock();
    int max_len_index =0;
      printf("dadadada here   are the results  \n");  
    for(i=0;i<resTop+1;i++){ 
         if(results_len[i]>results_len[max_len_index]) max_len_index = i;
          printf(" result no:%d is :  %s \tlen: %d\n",i+1,results[i],results_len[i]);  
    }
 printf("|||||||||||||||||||||||||||||||| time passed end of algorithm is %f \n", (double)(end - start)/(CLOCKS_PER_SEC*3)); 
      printf(" The Longest Word is:   %s is :\t   len: %d\n",results[max_len_index],results_len[max_len_index]);  
     // printf("dadadada clock per sec %f \n",(double)CLOCKS_PER_SEC);  
    
    free(pdictionary);
    exit(EXIT_SUCCESS);
}

//unsigned position or Length reached can be replace lenOfStack

void doSearch(char game_grid[4][4], dictionary *pdictionary,  coordinate coor, coordinate str_coor_stack[], int top,  int my_rank,int thread_count){  
    if((coor.x<0 || coor.x>3)  ||  (coor.y<0 || coor.y>3)) return;
    if(InsideStack(coor, str_coor_stack) ) return;    
    push(str_coor_stack, &top, coor);
   //printf("----------------------------- %c----%d , %d   by %d\n", game_grid[str_coor_stack[top].x][str_coor_stack[top].y], coor.x,coor.y, my_rank);
    char currentStr[32];
    int stackLength = lenOfStack(str_coor_stack);
    if(stackLength>=3  &&  stackLength<17){
     strcpy( currentStr, strReturn(game_grid, str_coor_stack, top));
   // if(my_rank == 2)
   // printf("     %d   word is %s the top stack in numthread              %d out of %d\n",top, currentStr, my_rank, thread_count);
   // printf("                                              %s     is the current word\n",currentStr);
//    printf("%d thread done. %d- %s   ++++\n", my_rank,stackLength,currentStr); 
        int searchResult = searchWord(pdictionary, currentStr);
//    printf("%d thread done. %d- %s   %d\n", my_rank,stackLength,currentStr,searchResult); 
        if(searchResult==1){
            if(notInsideResults(currentStr)){
                #pragma omp atomic                                
                    resTop++;
                results[resTop]=(char *) malloc(sizeof(char) *strlen(currentStr));
                strcpy(results[resTop] , currentStr);
                //printf("//////////  %d  /%s------------- is found lalalaaaa  by %d\n",resTop,results[resTop], my_rank);              
                results_len[resTop] = strlen(currentStr);
            }    
        }
        else if(searchResult==-1) return;
        
    }
        
                           
    coordinate  c1 = {coor.x+1, coor.y},     c2 = {coor.x, coor.y+1},
                c3 = {coor.x+1, coor.y+1}, c4 = {coor.x-1, coor.y-1},
                c5 = {coor.x-1, coor.y},     c6 = {coor.x, coor.y-1},                    
                c7 = {coor.x+1, coor.y-1}, c8 = {coor.x-1, coor.y+1};
        doSearch(game_grid, pdictionary, c1, str_coor_stack, top, my_rank, thread_count);
        doSearch(game_grid, pdictionary, c2, str_coor_stack, top, my_rank, thread_count);
        doSearch(game_grid, pdictionary, c3, str_coor_stack, top, my_rank, thread_count);
        doSearch(game_grid, pdictionary, c4, str_coor_stack, top, my_rank, thread_count);
        doSearch(game_grid, pdictionary, c5, str_coor_stack, top, my_rank, thread_count);
        doSearch(game_grid, pdictionary, c6, str_coor_stack, top, my_rank, thread_count);
        doSearch(game_grid, pdictionary, c7, str_coor_stack, top, my_rank, thread_count);
        doSearch(game_grid, pdictionary, c8, str_coor_stack, top, my_rank, thread_count);
       
    pop(str_coor_stack, &top);
}
_Bool InsideStack(coordinate coor, coordinate str_coor_stack[]){
    int iter=0;
    while(!(str_coor_stack[iter].x==-1 ||  str_coor_stack[iter].y==-1)){
    if(coor.x == str_coor_stack[iter].x  &&  coor.y == str_coor_stack[iter].y ) 
        return true;       
    iter++;
    }     
    return false;
}
_Bool notInsideResults(char currentStr[]){
    int i=0;           
    if(resTop > -1){
     for(i=0;i<resTop+1;i++){
         if(strcmp(currentStr,results[i])==0)
            return false;
     }
    }
    return true;
}

int lenOfStack(coordinate str_coor_stack[]){
    int iter=0;  
    while(!(str_coor_stack[iter].x==-1 || str_coor_stack[iter].y==-1)){   
     iter++;
    } 
    return iter;
}

const char *strReturn(char game_grid[4][4], coordinate str_coor_stack[], int top){
int i = 0;
    // printf("return string here %d index   top:%d    %d - %d\n",i,top, str_coor_stack[i-1].x ,str_coor_stack[i-1].y); 
char *s =(char*) malloc (sizeof (char) * 32);
    for(i; i<top+1; i++){
       s[i] = game_grid [str_coor_stack[i].x ][str_coor_stack[i].y];
    }
    s[i] = '\0'; 
    return s;
}

 coordinate pop(coordinate *coordinateArr, int *top){     
     coordinate c={-1,-1}; 
     if(*top == -1){
         printf("//////////is empty 666666666\n"); 
         return c;
     }
     coordinateArr[*top]=c;
     return coordinateArr[*top--];
 }

void push(coordinate coordinateArr[], int *top, coordinate coor){    
    coordinate c={-1,-1};
    if (*top == 31 || (coor.x == c.x  && coor.y == c.y))
    {
        printf("//////////is hamdooooooooooooo %d  at x %d y %d \n",*top,  coor.x,  coor.y); 
        
        return;}
    *top+=1;
    coordinateArr[*top] = coor;    
    return;
}

