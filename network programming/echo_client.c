#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

void error_handling(char *mesg)
{
	fprintf(stderr, "PANIC: %s.\n", mesg);
	exit(1);
}

#define BUF_SIZE 1024

int main(int argc, char *argv[])
{
	if (3!=argc) {
		fprintf(stderr, "USAGE: %s <IP> <port>\n", argv[0]);
		return 1;
	}
	// socket()
	int sock=socket(PF_INET, SOCK_STREAM, 0);
	if (-1==sock) error_handling("cannot socket()");
	// connect()
	struct sockaddr_in serv_adr;
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_adr.sin_port=htons(atoi(argv[2]));
	if (-1==connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))) error_handling("cannot connect()");
	else fprintf(stdout, "Connected ...\n");
	while (1)
	{
		fprintf(stdout, "Input message(Q to quit):");
		char message[BUF_SIZE];
		fgets(message, BUF_SIZE, stdin);
		if (!strcmp(message,"q\n") || !strcmp(message,"Q\n")) break;
		// read() / write()
		int str_len = write(sock, message, strlen(message));
		int recv_len=0;
		while (recv_len<str_len) {
			int recv_cnt=read(sock, &message[recv_len], BUF_SIZE-1);
			if (-1==recv_cnt) error_handling("cannot read()");
			recv_len+=recv_cnt;
		}
		message[recv_len]=0;
		fprintf(stdout, "Message from the server: %s", message);
	}
	// close()
	close(sock);
	return 0;
}
