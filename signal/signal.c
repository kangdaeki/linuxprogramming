#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void timeout(int sig)
{
	if (SIGALRM==sig) fprintf(stdout, "Time Out!\n");
	alarm(2);
}

void keycontrol(int sig)
{
	if (SIGINT==sig) fprintf(stdout, "CTRL+C pressed!\n");
}

int main(int argc, char *argv[])
{
	signal(SIGALRM, timeout);
	signal(SIGINT, keycontrol);
	alarm(2);
	for (int i=0;i<3;i++) {
		fprintf(stdout, "Wait...\n");
		sleep(100);
	}
	return 0;
}

