#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char *message)
{
	fprintf(stderr, "%s\n", message);
	exit(1);
}

int main(int argc, char *argv[])
{
	if (2!=argc) {
		fprintf(stderr, "USAGE: %s <port>\n",argv[0]);
		return 1;
	}
	// socket
	int serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (-1==serv_sock) error_handling("server socket() error!");
	// bind
	struct sockaddr_in serv_addr;
	memset(&serv_addr,0,sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_addr.sin_port=htons(atoi(argv[1]));
	if (-1==bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) error_handling("bind() error!");
	// listen
	if (-1==listen(serv_sock, 5)) error_handling("listen() error!");
	// accept
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size = sizeof(clnt_addr);
	int clnt_sock=accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
	if (-1==clnt_sock) error_handling("accept() error!");
	// read or write
	char message[]="Hello World!";
	write(clnt_sock,message,sizeof(message));
	// close
	close(clnt_sock);
	close(serv_sock);

	return 0;
}

