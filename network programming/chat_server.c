// gcc -o chat_server chat_server.c -lpthread
#define _REENTRANT
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>

void error_handling(char *mesg)
{
	fprintf(stderr, "PANIC: %s.\n", mesg);
	exit(1);
}

int clnt_cnt=0;
pthread_mutex_t mutx;
#define MAX_CLNT 256
int clnt_socks[MAX_CLNT];

void send_msg(char *msg, int len)
{
	pthread_mutex_lock(&mutx);
	for (int i=0;i<clnt_cnt;i++) write(clnt_socks[i],msg,len);
	pthread_mutex_unlock(&mutx);
}

#define BUF_SIZE 1024

void *handle_clnt(void *arg)
{
	int clnt_sock=*((int*)arg);
	int str_len=0;
	char msg[BUF_SIZE]={};
	while (0!=(str_len=read(clnt_sock, msg, sizeof(msg)))) send_msg(msg, str_len);

	// remove disconnected client
	pthread_mutex_lock(&mutx);
	for (int i=0;i<clnt_cnt;i++) {
		if (clnt_sock==clnt_socks[i]) {
			for (int j=i;j<clnt_cnt-1;j++) clnt_socks[j]=clnt_socks[j+1];
			break;
		}
	}
	clnt_cnt--;
	pthread_mutex_unlock(&mutx);
	close(clnt_sock);
	return NULL;
}

int main(int argc, char *argv[])
{
	if (2!=argc) {
		fprintf(stdout, "USAGE: %s <port>\n", argv[0]);
		return 1;
	}
	int serv_sock=socket(PF_INET, SOCK_STREAM, 0);
	if (-1==serv_sock) error_handling("cannot server socket()");
	struct sockaddr_in serv_adr;
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(atoi(argv[1]));
	if (-1==bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))) error_handling("cannot bind()");
	if (-1==listen(serv_sock,5)) error_handling("cannot listen()");
	pthread_mutex_init(&mutx, NULL);
	while (1) {
		struct sockaddr_in clnt_adr;
		socklen_t clnt_adr_sz=sizeof(clnt_adr);
		int clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
		if (-1==clnt_sock) {
			fprintf(stderr, "cannot client sock()\n");
			continue;
		}
		pthread_mutex_lock(&mutx);
		clnt_socks[clnt_cnt]=clnt_sock;
		clnt_cnt++;
		pthread_mutex_unlock(&mutx);
		pthread_t t_id;
		pthread_create(&t_id, NULL, handle_clnt, (void *)&clnt_sock);
		pthread_detach(t_id);
		fprintf(stdout, "Connected client IP: %s\n", inet_ntoa(clnt_adr.sin_addr));
	}
	close(serv_sock);
	return 0;
}

