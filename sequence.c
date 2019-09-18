#include <stdlib.h>
#include <malloc.h>
#include <string.h>

void GenerateString(int n, char* string){
    //string[0] = 'a';
    int k;
    char *stringcp = (char*) malloc ((n*n) *sizeof(char));

    int i;
    
    for (i = 1; i < n + 1; ++i){
        //printf("in\n");        
        //printf("copy\n");
        strcpy(stringcp,string);
        //printf("stringcp = %s\n", stringcp);
        string[(1<<(i-1)) - 1] = 'a' + i - 1;
        //printf("string.1 =  %s\n", string);
        strcat(string, stringcp);
        //printf("string.2 =  %s\n", string);
        //printf("out\n");  
    }
    printf("%s\n", string);
    free(stringcp);

     
}

int main(){
    int n;
    scanf("%d", &n);
    char *string = (char*) malloc ((n*n) *sizeof(char));
    GenerateString(n,string);
    free(string);
return 0;
}