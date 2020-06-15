#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>

void error_handling(char *message)
{
	fprintf(stderr, "%s\n", message);
	exit(1);
}

#define BUF_SIZE 1024

int main(int argc, char *argv[])
{
	if (2!=argc)
	{
		fprintf(stdout, "Usage: %s <port>\n", argv[0]);
		return 1;
	}
	int serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (-1==serv_sock) error_handling("cannot socket()");
	struct sockaddr_in serv_adr;
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(atoi(argv[1]));
	
	if (-1==bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))) error_handling("cannot bind()");
	if (-1==listen(serv_sock, 5)) error_handling("cannot listen()");
	
	fd_set reads;
	FD_ZERO(&reads);
	FD_SET(serv_sock, &reads);
	int fd_max=serv_sock;
	
	while (1)
	{
		fd_set cpy_reads=reads;
		struct timeval timeout;
		timeout.tv_sec = 5;
		timeout.tv_usec = 0;
		
		int  fd_num;
		if (-1==(fd_num=select(fd_max+1,&cpy_reads,0,0,&timeout)))  break;
		if (0==fd_num) continue;
		
		int  i;
		for (i=0;i<fd_max+1;i++)
		{
			if (FD_ISSET(i,&cpy_reads))
			{
				if (i==serv_sock)
				{
					struct sockaddr_in clnt_adr;
					socklen_t adr_sz=sizeof(clnt_adr);
					int clnt_sock = accept(serv_sock,(struct sockaddr*)&clnt_adr,&adr_sz);
					FD_SET(clnt_sock,&reads);
					if (fd_max<clnt_sock) fd_max=clnt_sock;
					fprintf(stdout, "Connected client: %d\n", clnt_sock);
				}
				else
				{
					char buf[BUF_SIZE];
					int  str_len = read(i,buf, BUF_SIZE);
					if (0==str_len)
					{
						FD_CLR(i,&reads);
						close(i);
						fprintf(stdout, "Closed client: %d\n", i);
					}
					else
					{
						write(i,buf,str_len);
					}
				}
			}
		}
		
	}
	close(serv_sock);
	return 0;
}

