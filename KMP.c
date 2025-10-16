#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void NextArray(char *const x);

int KMP(char a[],char *const);

int *next;

int main(){
    char a[17]="ABCDDCBAABABCDCD",b[100],*x;
    int result;
    printf("Input a character string.\n");
    scanf("%s",b);
    x=b;
    NextArray(x);
    result=KMP(a,x);
    printf("The location is %d.\n",result);
    free(next);
    return 0;
}

void NextArray(char *x){
    int LEN=strlen(x);
    next=(int*)malloc(LEN*sizeof(int));
    next[0]=-1,next[1]=0;
    int front=0,last=1;
    for(;last<LEN-1;last++){
        if(x[last]==x[front]){
            next[last+1]=next[last]+1;
            front++;
            last++;
        }
        else{
            next[last+1]=0;
            front=0;
            last++;
        }
    }
}

int KMP(char a[],char *x){
    int i=0,j=0;
    while(i<strlen(a)&&j<strlen(x)){
        if(a[i]==x[j]){
            i++;
            j++;
        }
        else if(a[i]!=x[j]&&j!=0){
            j=next[j];
        }
        else{
            i++;
        }
    }
    if(j==strlen(x)){
        return i-j;
    }
    else{
        return -1;
    }
}