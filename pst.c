#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>

#define LINELEN 1024
#define MAXPROC 32768

int pid[MAXPROC]={};
int ppid[MAXPROC]={};
int count=0;

void showtree(int cid, int depth, int depth_arr[])
{
    for (int i=0;i<depth;i++) 
        if (i==depth-1) fprintf(stdout,"+----"); 
        else if (1==depth_arr[i]) fprintf(stdout,"|    "); else fprintf(stdout,"     "); 
    fprintf(stdout,"%d\n",cid);
    int no_children=0;
    for (int i=0;i<count;i++) if (cid==ppid[i]) no_children++;
    int printed_children=0;
    for (int i=0;i<count;i++)
        if (cid==ppid[i]) 
        {
            if (printed_children<no_children-1) depth_arr[depth]=1; else depth_arr[depth]=-1;
            showtree(pid[i], depth+1, depth_arr);
            printed_children++;
        }
}

int mother_work(int p[])
{
	close(p[1]);
    int status;
    wait(&status); // wait for the child
//    int flags=fcntl(p[0],F_GETFL,0);
//    flags|=O_NONBLOCK;
//    fcntl(p[0],F_SETFL,flags); // non-blocking fgets
    FILE *fp=fdopen(p[0],"rt");

    char line[LINELEN]={};
    fgets(line,LINELEN,fp); // skip the first line
    while (fgets(line,LINELEN,fp)) 
    {
        char *wp=strtok(line," \n\r\t");
        wp=strtok(NULL," \n\r\t");
        pid[count]=atoi(wp);
        wp=strtok(NULL," \n\r\t");
        ppid[count]=atoi(wp);
        count++;
    }
	shutdown(p[0],SHUT_WR);
	fclose(fp);
    int depth_arr[MAXPROC]={};
    showtree(1,0,depth_arr);
    return 0;
}

int child_work(int p[])
{
    close(p[0]);
    dup2(p[1],1);
    execl("/bin/ps","ps","-ef",(char*)0);
    return 0;
}

int main()
{
    int p[2];
    if (-1==pipe(p))
    {
        fprintf(stderr, "Fail to call pipe()\n");
        exit(1);
    }
    pid_t pid=fork();
    if (0<pid) return mother_work(p);
    else if (0==pid) return child_work(p);
    else
    {
        fprintf(stderr, "Fail to call fork()\n");
        exit(1);
    }
    return 0;
}
