#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUFF_LEN 2048

void error_handling(char *message)
{
	fprintf(stderr, "%s\n", message);
	exit(1);
}

int main(int argc, char *argv[])
{
	if (3!=argc) {
		fprintf(stderr, "USAGE: %s <IP> <port>\n", argv[0]);
		return 1;
	}
	// socket
	int sock=socket(PF_INET, SOCK_STREAM, 0);
	if (-1==sock) error_handling("socket() error");
	// connect
	struct sockaddr_in serv_addr;
	memset(&serv_addr,0,sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_addr.sin_port=htons(atoi(argv[2]));
	if (-1==connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) error_handling("connect() error!");
	// read or write
	char message[BUFF_LEN];
	int str_len = read(sock, message, sizeof(message)-1);
	fprintf(stdout, "Message from the server <%s>\n", message);
	// close
	close(sock);
	return 0;
}

