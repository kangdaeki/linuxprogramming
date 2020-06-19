// page 220, problem 1
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    int x=0;
    fprintf(stdout, "Enter x: ");
    fscanf(stdin, "%d", &x);
    int y=0;
    fprintf(stdout, "Enter y: ");
    fscanf(stdin, " %d", &y);

    pid_t pid=fork();
    if (0<pid)
    {
        long unsigned int prod=1;
        for (int i=0;i<y;i++) prod*=x;
        int status=0;
        waitpid(pid,&status,0);
        fprintf(stdout, "Total=%lu\n", prod+(status>>8));
        return 0;
    }
    else if (0==pid)
    {
        int sum=0;
        for (int i=x;i<=y;i++) sum+=i;
        return sum;
    }
    else
    {
        fprintf(stderr, "Cannot fork");
        return 1;        
    }
}
