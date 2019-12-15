#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

char* requiredFile;

int Search_file(int currentLevel, char* directory)
{
    DIR* dir = opendir(directory);
    if(dir == NULL)
    {
        printf("Could not open the directory\n");
        return 2;
    }
    char path[1024];
    struct dirent* fileInfo;
    while ((fileInfo = readdir(dir)) != NULL)
    {
        if(strcmp(fileInfo->d_name, "..") == 0 || strcmp(fileInfo->d_name, ".") == 0)
        {
            continue;
        }
        
        if(fileInfo->d_type != DT_REG && fileInfo->d_type != DT_DIR)
        {
            continue;
        }
        
        if(fileInfo->d_type == DT_DIR && currentLevel - 1 > 0)
        {
            sprintf(path, "%s/%s", directory, fileInfo->d_name);
            if(Search_file(currentLevel - 1, path) == 1)
            {
                return 1;
            }
        }
        
        if(strcmp(fileInfo->d_name, requiredFile) == 0)
        {
            return 1;
        }
    }
    return 0;
    closedir(dir);
}

int main(int argc, char** argv)
{
    if(argc < 4)
    {
        printf("Input the directory, recursion level and file name\n");
        return 1;
    }
    int recLevel = atoi(argv[2]);
    if(recLevel < 1)
    {
        printf("Recursion level must be more than 0");
        return 2;
    }
    requiredFile = argv[3];
    int res = Search_file(recLevel, argv[1]);
    printf("%d\n", res);
    return 0;
}
