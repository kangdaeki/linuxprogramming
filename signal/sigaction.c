#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void timeout(int sig)
{
	if (SIGALRM==sig) fprintf(stdout, "Time Out!\n");
	alarm(2);
}

int main(int argc, char *argv[])
{
	struct sigaction act;
	act.sa_handler=timeout;
	sigemptyset(&act.sa_mask);
	act.sa_flags=0;
	sigaction(SIGALRM, &act,0);
	alarm(2);
	for (int i=0;i<3;i++) {
		fprintf(stdout, "Wait...\n");
		sleep(100);
	}
	return 0;
}

