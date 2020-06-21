#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>

void error_handling(char *buf)
{
	fprintf(stderr, "%s\n", buf);
	exit(1);
}

// BUF_SIZE is set to a small value on purpose.
#define BUF_SIZE 4
#define EPOLL_SIZE 50

int main(int argc, char *argv[])
{
	if (2!=argc) {
		fprintf(stdout, "USAGE: %s <port>\n", argv[0]);
		return 1;
	}
	int serv_sock=socket(PF_INET, SOCK_STREAM, 0);
	if (-1==serv_sock) error_handling("cannot serv socket()");
	struct sockaddr_in serv_adr;
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(atoi(argv[1]));
	if (-1==bind(serv_sock,(struct sockaddr *)&serv_adr, sizeof(serv_adr))) error_handling("cannot bind()");
	if (-1==listen(serv_sock, 5)) error_handling("listen() error");

	int epfd=epoll_create(EPOLL_SIZE);
	if (-1==epfd) error_handling("cannot epoll_create()");
	else fprintf(stdout, "epoll created with epfd: %d\n",epfd);
	struct epoll_event *ep_events=(struct epoll_event *)malloc(sizeof(struct epoll_event *)*EPOLL_SIZE);
	struct epoll_event event;
	event.events=EPOLLIN;
	event.data.fd=serv_sock;
	epoll_ctl(epfd, EPOLL_CTL_ADD, serv_sock, &event);
	while (1) {
		int event_cnt=epoll_wait(epfd, ep_events, EPOLL_SIZE, -1);
		if (-1==event_cnt) {
			fprintf(stderr, "epoll_wait() error\n");
			break;
		}
		fprintf(stdout, "return epoll_wait()\n");
		for (int i=0;i<event_cnt;i++) {
			if (ep_events[i].data.fd==serv_sock) {
				struct sockaddr_in clnt_adr;
				socklen_t adr_sz=sizeof(clnt_adr);
				int clnt_sock=accept(serv_sock,(struct sockaddr *)&clnt_adr, &adr_sz);
				event.events=EPOLLIN;
				event.data.fd=clnt_sock;
				epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_sock, &event);
				fprintf(stdout, "Connected client: %d\n", clnt_sock);
			}
			else {
				char buf[BUF_SIZE]={};
				int str_len=read(ep_events[i].data.fd, buf, BUF_SIZE);
				if (0==str_len) {
					epoll_ctl(epfd, EPOLL_CTL_DEL, ep_events[i].data.fd, NULL);
					close(ep_events[i].data.fd);
					fprintf(stdout, "Closed client: %d\n", ep_events[i].data.fd);
				}
				else write(ep_events[i].data.fd, buf, str_len); // echo
			}
		}
	}

	free(ep_events);
	close(epfd);
	close(serv_sock);
	return 0;
}

