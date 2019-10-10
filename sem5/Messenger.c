#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>

void ReadFifo(char* file){
    char output[100];
    int fd = open(file, O_RDONLY);
    read(fd, output, 100);
    printf("%s", output);
    close(fd);
}

void WriteFifo(char* file){
    char input[100];
    int fd = open(file, O_WRONLY);
    fgets(input, 100, stdin);
    write(fd, input, 100);
    close(fd);
}
  

int main(int argc, char** argv){
    int fd;
    char name01[]="aaaa.fifo";
    char name10[] = "aaa.fifo";
    (void)umask(0);
    char *n1 = "1";
    char *n0 = "0";
    mknod(name01, S_IFIFO | 0666, 0);
    mknod(name10, S_IFIFO | 0666, 0);

    if (strcmp(argv[1], n0) == 0){            
        pid_t frk = fork();
        for(;;){
        if (frk > 0)
            WriteFifo(name01);
        else
            ReadFifo(name10);
        }
    }
    
    if (strcmp(argv[1], n1) == 0){
        pid_t pid = fork();
        for(;;){
        if (pid > 0)
            WriteFifo(name10);
        else
            ReadFifo(name01);
        }
    }
    close(fd);
    return 0;
}
