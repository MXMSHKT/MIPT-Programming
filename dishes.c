#include<sys/stat.h>
#include<sys/msg.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/sem.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include"split.h"
#include<string.h>
#include<errno.h>
#include<unistd.h>
#include<fcntl.h>
#include<signal.h>
#define NAMEFILE "washer.txt"
#define MAX_SIZE 100
#define TABLE_LIMIT 2


void File_Read(FILE *mf, char *string){
    mf = fopen(NAMEFILE, "r");
    char *estr;
    char str[MAX_SIZE] = {};
    do{
        estr = fgets(str, MAX_SIZE, mf);
        if(estr == NULL){
            break;
        }
        strcat(string, str);
    }while(estr);
    fclose(mf);
}

int main(){
    int semid1;
    int msgid;
    int len, maxlen;
    struct mymsgbuf{
        long mtype;
        char mtext[MAX_SIZE];
    } mybuf;
    char pathname[] = "a.c";
    key_t key;
    struct sembuf buf;
    if((key = ftok(pathname, 0)) < 0){
        exit(-1);
    }
    if((semid1 = semget(key, 1, 0666 | IPC_CREAT)) < 0){
        exit(-1);
    }
    msgid = msgget(key, 0666 | IPC_CREAT);
    int i;
    char string[MAX_SIZE] = {};
    int tokensCount = 0;
    char delimiters[2] = {':','\n'};
    FILE *mf;
    File_Read(mf, string);
    char **tokens;
    tokens = (char**)calloc(MAX_SIZE, sizeof(char*));
    for(i = 0; i < MAX_SIZE; i++){
        tokens[i] = (char*)calloc(MAX_SIZE, sizeof(char));
    }
    Split(string, delimiters, &tokens, &tokensCount);
    buf.sem_op = TABLE_LIMIT - 1;
    buf.sem_flg = 0;
    buf.sem_num = 0;
    semop(semid1, &buf, 1);
    for(i = 0; i < tokensCount; i += 2){
        sleep(atoi(tokens[i+1]));
        printf(" washed %s \n", tokens[i]);
        mybuf.mtype = 1;
        strcpy(mybuf.mtext, tokens[i]);
        msgsnd(msgid, &mybuf, sizeof(struct mymsgbuf) - sizeof(long), 0);
        printf(" send %s\n", mybuf.mtext);
        buf.sem_op = -1;
        buf.sem_flg = 0;
        buf.sem_num = 0;
        semop(semid1, &buf, 1);
    }
    mybuf.mtype = 228;
    len = 0;
    msgsnd(msgid, &mybuf, len, 0);
    free(tokens);
    return 0;
}
