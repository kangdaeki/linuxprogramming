#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#define BUF_LEN 1024

int main(int argc, char* argv[])
{
	int fd[2]={};
	if (-1==pipe(fd))
	{
		fprintf(stderr,"[PANIC] fail to call pipe()\n");
		exit(-1);
	}
	
	pid_t pid=fork();
	if (0<pid) // parent
	{
		close(fd[0]); // parent is not going to read from the pipe
		dup2(fd[1],1);
		execlp("ls","ls",(char *)NULL);
		return 0;
	}
	else if (0==pid) // child
	{
		close(fd[1]); // child is not going to write to the pipe
		char msg2[BUF_LEN]={};
		sleep(1);
		printf("[child] %d\n",getpid());
		read(fd[0],msg2,BUF_LEN);
		printf("[child] %s\n",msg2);
		exit(1);
	}
	else // error in fork()
	{
		fprintf(stderr,"[PANIC] fail to call fork()\n");
		exit(-1);
	}
	return 0;
}