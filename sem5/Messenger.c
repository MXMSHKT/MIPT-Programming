#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>

void Dia(char *name01, char *name10, int fd){
    int frk = fork();
    while(1){
        if (frk == 0){
            fd = open(name10, O_WRONLY);
            char buf[100];
            memset(buf, 0, 100);
            fgets(buf, 100, stdin);
            write(fd, buf, 100);
            memset(buf, 0, 100);
            close(fd);
        }
        else{

            fd = open(name01, O_RDONLY);
            char buf[100];
            memset(buf, 0, 100);
            read(fd, buf, 100);
            printf("%s", buf);
            memset(buf, 0, 100);
            close(fd);
        }
    }
    kill(getpid(), SIGKILL);
}      

int main(int argc, char** argv){
    int fd;
    char name01[]="aaaa.fifo";
    char name10[] = "aaa.fifo";
    (void)umask(0);
    char *n1 = "1";
    char *n0 = "0";
    if (strcmp(argv[1], n0) == 0){            
        Dia(name10, name01, fd);
    }
    
    if (strcmp(argv[1], n1) == 0){
        Dia(name01, name10, fd);
    }
return 0;
}