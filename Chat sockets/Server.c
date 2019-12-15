#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include<sys/mman.h>
#include<fcntl.h>
#include<sys/ipc.h>


int* descriptors;
int nclients;
char* str;
int sockfd;

void* While(void* mySocket) {
    int newsockfd = *(int*)mySocket;
    char line[1000];
    int n;
    while ((n = read(newsockfd, line, 100)) > 0) {
            strcpy(str, line);
            str = &str[strlen(line)];
            //printf("%s", line);
            for (int i = 0; i < nclients; ++i) {
            if (descriptors[i] != newsockfd) {
                if ((n = write(descriptors[i], line,
                    strlen(line) + 1)) < 0) {
                    perror(NULL);
                    //close(sockfd);
                    close(newsockfd);
                    exit(1);
                }
            }
        }
    }
}

int main()
{
    int fd = open("test.txt", O_RDWR, 0666);
    ftruncate(fd, 10000);
    str = (char*)mmap(NULL, 10000, PROT_READ | PROT_WRITE | O_APPEND, MAP_SHARED, fd, 0);
    close(fd);
    nclients = 0;
    descriptors = malloc(5 * sizeof(int));
    int newsockfd;  
    int clilen;     
    int n;
    char line[1000]; 
    struct sockaddr_in servaddr, cliaddr; 
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror(NULL);
        exit(1);
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(51000);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    if (bind(sockfd, (struct sockaddr *) &servaddr,
        sizeof(servaddr)) < 0) {
        perror(NULL);
        close(sockfd);
        exit(1);
    }

    if (listen(sockfd, 100) < 0) {
        perror(NULL);
        close(sockfd);
        exit(1);
    }

    while (1) {
        pthread_t thid;
    
        clilen = sizeof(cliaddr);
        

        if ((newsockfd = accept(sockfd,
            (struct sockaddr *) &cliaddr, &clilen)) < 0) {
            perror(NULL);
            close(sockfd);
            exit(1);
        }
        ++nclients;
        descriptors[nclients-1] = newsockfd;
    
        pthread_create(&thid, (pthread_attr_t *)NULL, While, &newsockfd);

    }
}




