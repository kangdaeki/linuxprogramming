#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>


void error_handling(char * mesg)
{
	fprintf(stderr, "%s\n",mesg);
	exit(1);
}

#define BUF_SIZE 1024

int main(int argc, char *argv[])
{
	if (2!=argc) {
		fprintf(stdout, "USAGE: %s <port>\n",argv[0]);
		exit(1);
	}
	int serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (-1==serv_sock) error_handling("cannot socket()");
	struct sockaddr_in serv_adr;
	memset(&serv_adr,0,sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(atoi(argv[1]));
	if (-1==bind(serv_sock,(struct sockaddr*)&serv_adr,sizeof(serv_adr))) 
		error_handling("cannot bind()");
	if (-1==listen(serv_sock, 5)) error_handling("cannot listen()");
	int count=0;
	while (1) {
		struct sockaddr_in clnt_adr;
		socklen_t clnt_adr_sz=0;
		int clnt_sock = accept(serv_sock,(struct sockaddr*)&clnt_adr,&clnt_adr_sz);
		if (-1==clnt_sock) error_handling("cannot accept()");
		else {
			count++;
			fprintf(stdout, "Client connected (%d)\n", count);
		}
		int str_len=0;
		char message[BUF_SIZE]={};
		while (0!=(str_len=read(clnt_sock,message,BUF_SIZE))) 
			write(clnt_sock,message,str_len);
		close(clnt_sock);
	}
	close(serv_sock);
	return 0;
}
