#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <omp.h>

typedef struct coordinate
{
    int x;
    int y;
}coordinate;

int resTop=-1;
char *results[32];
int results_len[100];

coordinate pop(coordinate *coordinateArr, int *top);
void push(coordinate coordinateArr[], int *top, coordinate coor);
const char *strReturn(char game_grid[4][4], coordinate str_coor_stack[], int top);
void doSearch(char game_grid[4][4], char *dictionary[],int size, coordinate coor, coordinate str_coor_stack[], int top, int my_rank,int thread_count);
_Bool InsideStack(coordinate coor, coordinate str_coor_stack[]);
_Bool notInsideResults(char current[]);
int lenOfStack(coordinate str_coor_stack[]);


int searchWord(char *dictionary[],int size, char *target);


int main(int argc, char *argv[])
{       
    int thread_count ;
    clock_t start = clock();
    clock_t end;
    char *dictionary[50000];
    FILE * fp;
    char line [32];
    char maxstr[32];
    
    char game_grid[4][4]={
        {'z','r','c','u'},
        {'o','n','y','u'},
        {'n','l','b','k'},
        {'k','a','m','a'},
    };
    size_t len = 0;
    unsigned int max=0;
    ssize_t read;
    int i =0,j =0, k=0;
    coordinate str_coor_stack[32], coor_def = {-1,-1};    
    coordinate str_coor_stack_def[32];    
    
    int size =-1;
    
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
        if(max<strlen(line))
        {   max=strlen(line);
            strcpy(maxstr,line);
        }
         size++;
         dictionary[size]= (char *) malloc(strlen(line) * sizeof(char));
         strcpy(dictionary[size], line);   
                         
             //printf("a %s   %p   %d\n", dictionary[size], dictionary[size],size);
    }
    fclose(fp);
    //printf("%d is max line of length.\n", max);    
    end = clock();
    printf("|||||||||||||||||||||||||||%f is time passed end of dict\n", (double)(end - start)/(CLOCKS_PER_SEC*3)); 
    start = clock();

    coordinate coor;
              
       for(k=0;k<16;k++){  str_coor_stack_def[k] = coor_def; }   
  #pragma omp parallel for default(none) private(i,j,coor,str_coor_stack) shared(game_grid,thread_count, dictionary,size, coor_def,str_coor_stack_def) schedule(static,1)
   for (i=0;i<4;i++){
       int top = -1;
       thread_count = omp_get_num_threads();     
       int my_rank = omp_get_thread_num();
       memcpy(&str_coor_stack, &str_coor_stack_def, sizeof(str_coor_stack_def));
       int j,k;
           for (j=0;j<4;j++){
               coor.x = j; coor.y = i;
               //printf("%d ......  %d   ***************************************\n", coor.x,coor.y);        
               doSearch(game_grid, dictionary, size, coor, str_coor_stack, top, my_rank, thread_count);  
           }  
    }
    end = clock();
    
      printf("dadadada here   are the results  \n");  
    for(i=0;i<resTop+1;i++){            
          printf(" result no:%d is :  %s \tlen: %d\n",i+1,results[i],results_len[i]);  
    }
 printf("||||||||||||||||||||%f is time passed end of algorithm\n", (double)(end - start)/(CLOCKS_PER_SEC*3)); 
     // printf("dadadada clock per sec %f \n",(double)CLOCKS_PER_SEC);  
    
    //free(dictionary);
    exit(EXIT_SUCCESS);
}

//unsigned position or Length reached can be replace lenOfStack

void doSearch(char game_grid[4][4], char *dictionary[], int size, coordinate coor, coordinate str_coor_stack[], int top,  int my_rank,int thread_count){     
    if((coor.x<0 || coor.x>3)  ||  (coor.y<0 || coor.y>3)) return;
    if(InsideStack(coor, str_coor_stack) ) return;    
    push(str_coor_stack, &top, coor);
   //printf("----------------------------- %c----%d , %d   by %d\n", game_grid[str_coor_stack[top].x][str_coor_stack[top].y], coor.x,coor.y, my_rank);
    char currentStr[32];
    int stackLength = lenOfStack(str_coor_stack);
    if(stackLength>=4  &&  stackLength<16){
     strcpy( currentStr, strReturn(game_grid, str_coor_stack, top));
   // if(my_rank == 2)
   // printf("     %d   word is %s the top stack in numthread              %d out of %d\n",top, currentStr, my_rank, thread_count);
   // printf("                                              %s     is the current word\n",currentStr);
//     printf("%d thread done. %d- %s   %\n", my_rank,stackLength,currentStr); 
        if(searchWord(dictionary, size,currentStr)==1){
            if(notInsideResults(currentStr)){
                #pragma omp atomic                                
                    resTop++;
                results[resTop]=(char *) malloc(sizeof(char) *strlen(currentStr));
                strcpy(results[resTop] , currentStr);
                //printf("//////////  %d  /%s------------- is found lalalaaaa  by %d\n",resTop,results[resTop], my_rank);              
                results_len[resTop] = strlen(currentStr);
            }    
        }
        else if(searchWord(dictionary, size,currentStr)==-1) return;
        
    }
        
                           
    coordinate  c1 = {coor.x+1, coor.y},     c2 = {coor.x, coor.y+1},
                c3 = {coor.x+1, coor.y+1}, c4 = {coor.x-1, coor.y-1},
                c5 = {coor.x-1, coor.y},     c6 = {coor.x, coor.y-1},                    
                c7 = {coor.x+1, coor.y-1}, c8 = {coor.x-1, coor.y+1};
        doSearch(game_grid, dictionary, size, c1, str_coor_stack, top, my_rank, thread_count);
        doSearch(game_grid, dictionary, size, c2, str_coor_stack, top, my_rank, thread_count);
        doSearch(game_grid, dictionary, size, c3, str_coor_stack, top, my_rank, thread_count);
        doSearch(game_grid, dictionary, size, c4, str_coor_stack, top, my_rank, thread_count);
        doSearch(game_grid, dictionary, size, c5, str_coor_stack, top, my_rank, thread_count);
        doSearch(game_grid, dictionary, size, c6, str_coor_stack, top, my_rank, thread_count);
        doSearch(game_grid, dictionary, size, c7, str_coor_stack, top, my_rank, thread_count);
        doSearch(game_grid, dictionary, size, c8, str_coor_stack, top, my_rank, thread_count);
       
    pop(str_coor_stack, &top);
}
_Bool InsideStack(coordinate coor, coordinate str_coor_stack[]){
    int iter=0;
    while(!(str_coor_stack[iter].x==-1 &&  str_coor_stack[iter].y==-1)){
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


int searchWord(char *dictionary[],int size, char *target)
{

    /* error condition check */
    if (dictionary == NULL || target == NULL)
    {
        fprintf(stderr, " -- invalid pointer to string or hashtable -- \n");
        return false;
    }
    int curr_bottom= 0;
    int mid;
    int curr_top = size - 1;
    
    //search a part of the word before ...

    while(curr_bottom <= curr_top){
        mid = (curr_bottom + curr_top)/2;
        if (strcmp(dictionary[mid], target) == 0){
           // printf("%s found at location %d.\n", target, mid+1);
            return 1;
        } else if (strcmp(dictionary[mid], target) > 0){
            curr_top    = mid - 1;
        } else if (strcmp(dictionary[mid], target) < 0){
            curr_bottom = mid + 1;
        }
    }
    if(strlen(target) == 4){
        char exist_str[10];
        strncpy(exist_str, dictionary[mid], 3);
           // printf(" %s dictionary[mid],     %s target /////// \n",dictionary[mid],target);
        if(strcmp(dictionary[mid],target)==0) 
            return 0;
        else {
             return -1;
        }
           
    }
    
    return 0;
}

