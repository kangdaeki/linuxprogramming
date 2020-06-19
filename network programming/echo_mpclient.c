#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30

void error_handling(char *mesg) 
{
	fprintf(stderr,"%s\n",mesg);
	exit(1);
}

void read_routine(int sock, char *buf)
{
	while (1)
	{
		int len = read(sock, buf, BUF_SIZE);
		if (0==len) return;
		buf[len]=0;
		fprintf(stdout, "Message from the server: %s", buf);
	}
}

void write_routine(int sock, char *buf)
{
	while (1)
	{
		fgets(buf,BUF_SIZE,stdin);
		if ( (0==strcmp(buf,"q\n")) || (0==strcmp(buf,"Q\n")) )
		{
			shutdown(sock, SHUT_WR);
		}
		write(sock,buf,strlen(buf));
	}
}

int main(int argc, char *argv[])
{
	if (3!=argc)
	{
		fprintf(stderr, "Usage: %s <IP> <port>", argv[0]);
		return 1;
	}
	
	int sock;
	sock=socket(PF_INET, SOCK_STREAM, 0);
	
	struct sockaddr_in serv_adr;
	memset(&serv_adr,0,sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_adr.sin_port = htons(atoi(argv[2]));
	
	if (-1==connect(sock,(struct sockaddr *)&serv_adr,sizeof(serv_adr))) error_handling("connect() error!");
	
	char buf[BUF_SIZE];
	pid_t pid = fork();
	if (0==pid) /* child */ write_routine(sock,buf); else /* parent */ read_routine(sock,buf);
	close(sock);
	return 0;
}


