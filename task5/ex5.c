#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#define N 100

typedef struct Data{
    int time;
    char **command;
} Data;

int main(){
    int num;
    FILE *file = fopen("input.txt","r");
    fscanf(file, "%d", &num);

    Data *data = (Data *) calloc(num, sizeof(Data));

    for(int i = 0 ; i < num; i++){
        int g = 2;
        data[i].command = (char**) calloc(N, sizeof(char*));

        fscanf(file, "%d", &data[i].time);
        data[i].command[0] = (char*) calloc(N, sizeof(char));
        data[i].command[1] = (char*) calloc(N, sizeof(char));
        strcpy(data[i].command[0], "timeout");
        strcpy(data[i].command[1], "5");
        
        for(;;){
            
            data[i].command[g] = (char*)calloc(N, sizeof(char));
            for(int j = 0;;j++){
             
                fscanf(file, "%c", &data[i].command[g][j]);
                

                if (data[i].command[g][j] == ' '){
                    data[i].command[g][j] = '\0';
                    g++;
                    break;
                }
                
                if (data[i].command[g][j] == '\n'){
                    data[i].command[g][j] = '\0';
                    data[i].command[g + 1] = (char *)0;
                    break;
                }
            }
                break;
        }
    }

    for (int i = 0; i < num; i++){
        pid_t pid = fork();
        if (pid == 0){
            sleep(data[i].time);
            execvp(data[i].command[0], data[i].command); 
        }
    }

    int k = 0; 
    int status = 0;
    for(;;){
        pid_t pid_ = waitpid(-1, &status, 0);

        if ((status >> 8) & 255){
            printf("Process %d was killed after timeout\n", pid_);
        } else {
            printf("OK\n");
        }

        k++;

        if(k == num)
            break;
    }
    return 0;
}

