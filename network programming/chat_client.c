// sudo apt install manpages-posix
// sudo apt install manpages-posix-dev
// gcc -o chat_client chat_client.c -lpthread
#define _REENTRANT
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

void error_handling(char *mesg)
{
	fprintf(stderr, "PANIC: %s.\n", mesg);
	exit(1);
}

#define NAME_SIZE 1024
char name[NAME_SIZE]="[DEFAULT]";
#define BUF_SIZE 1024
char msg[BUF_SIZE]={};

void *send_msg(void *arg)
{
	int sock=*((int*)arg);
	char name_msg[NAME_SIZE+BUF_SIZE]={};
	while (1) {
		fgets(msg, BUF_SIZE, stdin);
		if (!strcmp(msg,"q\n")||!strcmp(msg,"Q\n")) {
			close(sock);
			exit(1);
		}
		sprintf(name_msg, "%s %s", name, msg);
		write(sock, name_msg, strlen(name_msg));
	}
	return NULL;
}

void *recv_msg(void *arg)
{
	int sock=*((int*)arg);
	char name_msg[NAME_SIZE+BUF_SIZE]={};
	while (1) {
		int str_len=read(sock, name_msg, NAME_SIZE+BUF_SIZE-1);
		if (-1==str_len) return (void*)-1;
		name_msg[str_len]=0;
		fputs(name_msg,stdout);
	}
	return NULL;
}

int main(int argc, char *argv[])
{
	if (4!=argc) {
		fprintf(stdout, "USAGE: %s <IP> <port> <name>\n", argv[0]);
		return 1;
	}
	sprintf(name, "[%s]", argv[3]);
	int sock=socket(PF_INET, SOCK_STREAM, 0);
	if (-1==sock) error_handling("cannot server socket()");
	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_addr.sin_port=htons(atoi(argv[2]));
	if (-1==connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) error_handling("cannot connect()");
	pthread_t snd_thread, rcv_thread;
	pthread_create(&snd_thread, NULL, send_msg, (void*)&sock);
	pthread_create(&rcv_thread, NULL, recv_msg, (void*)&sock);
	void *thread_return;
	pthread_join(snd_thread,&thread_return);
	pthread_join(rcv_thread,&thread_return);
	close(sock);
	return 0;
}

