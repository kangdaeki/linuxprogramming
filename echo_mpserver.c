#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>

void error_handling(char *mesg)
{
	fprintf(stderr, "PANIC: %s.\n", mesg);
	exit(1);
}

void read_childproc(int sig)
{
	int status=0;
	pid_t pid=waitpid(-1, &status, WNOHANG);
	fprintf(stderr, "Removed proc id: %d\n", pid);
}

#define BUF_SIZE 1024

int	main(int argc, char *argv[])
{
	if (2!=argc) {
		fprintf(stderr, "USAGE: %s <port>\n", argv[0]);
		return 1;
	}
	struct sigaction act;
	act.sa_handler=read_childproc;
	sigemptyset(&act.sa_mask);
	act.sa_flags=0;
	int state=sigaction(SIGCHLD, &act, 0);
	// socket()
	int serv_sock=socket(PF_INET, SOCK_STREAM, 0);
	if (-1==serv_sock) error_handling("cannot socker()");
	// bind()
	struct sockaddr_in serv_adr, clnt_adr;
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(atoi(argv[1]));
	if (-1==bind(serv_sock,(struct sockaddr*)&serv_adr,sizeof(serv_adr))) error_handling("cannot bind()");
	// listen()
	if (-1==listen(serv_sock,5)) error_handling("cannot listen()");
	while (1)
	{
		socklen_t clnt_adr_sz=sizeof(clnt_adr);
		// accept()
		int clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
		if (-1==clnt_sock) continue;
		else fprintf(stdout, "New client connected...\n");
		pid_t pid=fork();
		if (0==pid) {
			close(serv_sock);
			int str_len=0;
			char buf[BUF_SIZE];
			// read() / write()
			while (0!=(str_len=read(clnt_sock, buf, BUF_SIZE))) write(clnt_sock, buf, str_len);
			close(clnt_sock);
			fprintf(stdout, "Client disconnected...\n");
			return 0;
		}
		else close(clnt_sock);
	}
	// close()
	close(serv_sock);
	return 0;
}

