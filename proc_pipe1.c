#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define BUF_LEN 1024

int main(int argc, char* argv[])
{
	int p[2]={};
	if (-1==pipe(p))
	{
		fprintf(stderr,"[PANIC] fail to call pipe()\n");
		exit(-1);
	}
	
	pid_t pid=fork();
	if (0<pid) // parent
	{
		close(p[0]);
		dup2(p[1],1);
		execlp("ls","ls","-al",(char *)NULL);
		fprintf(stderr,"[parent] fail to call exec()\n");
		return 0;
	}
	else if (0==pid) // child
	{
		close(p[1]);
		dup2(p[0],0);
		execlp("wc","wc",(char *)NULL);
		fprintf(stderr,"[child] fail to call exec()\n");
	}
	else // error in fork()
	{
		fprintf(stderr,"[PANIC] fail to call fork()\n");
		exit(-1);
	}
	return 0;
}