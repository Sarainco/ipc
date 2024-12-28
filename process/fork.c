#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<math.h>


int main()
{
    pid_t child;
    int status;

    printf("THIS WILL DEMOSTRATE HOW TO GET CHILD STATUS\n");

    if(child = fork() == -1)
    {
        printf("Fork Error: %s\n", strerror(errno));
        exit(1);
    }
    else if(child == 0)
    {
        int i;
        printf("i am child : %d\n", getpid());
        for(i=0; i<1000000;i++) sin(i);
        i = 5;
        printf("I EXIT WITH %d\n", i);
        exit(i);
    }

}
