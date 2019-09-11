#include <stdlib.h>
#include <malloc.h>
int main(){
    int size;
    scanf("%dsize = ", &size);
    int *a = (int*) malloc (size *sizeof(int));
    int i,k;
    for (i = 0; i<size; i++)
        {
            printf("a[%d] = ", i);
            scanf("%d", &a[i]);
        }
    for (i = 0; i<size; i++)
    printf("%d ", a[i]);
    free(a);
return 0;
}