#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>


int main()
{
    pid_t pid;
    int status,i;
    if(fork() == 0)
    {
        printf("This is the child process pid = %d\n", getpid());
        exit(5);
    }
    else
    {
        sleep(1);
        printf("this is parent process wait for child\n");
        pid = wait(&status);
        i = WEXITSTATUS(status);
        printf("child pid = %d exit status = %d\n", pid, i);
    }

}
