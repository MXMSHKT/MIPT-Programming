#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void Split(char* str, char* delimeters);

int main(){
    char string[10000];
    printf("Enter string: ");
    fgets(string, 100, stdin);
    
    char delimeters[10000];
    printf("Enter delimeters: ");
    fgets(delimeters, 100, stdin);
    
    Split(string, delimeters);
}

void Split(char* str, char* delimeters){
    char *tok = strtok(str, delimeters);
    while (tok != NULL){
        printf("%s\n", tok);
        tok = strtok(NULL, delimeters);
    }
}
