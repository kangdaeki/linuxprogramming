#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#define MAXPATH 1024

int main(int argc, char *argv[])
{
	if (3!=argc)
	{
		fprintf(stderr,"usage: %s path1 path2\n",argv[0]);
		return 1;
	}
	char buf1[MAXPATH]={};
	if (NULL==realpath(argv[1],buf1))
	{
		fprintf(stderr,"realpath error for %s\n",argv[1]);
		return 1;
	}
	char buf2[MAXPATH]={};
	if (NULL==realpath(argv[2],buf2))
	{
		fprintf(stderr,"realpath error for %s\n",argv[2]);
		return 1;
	}
	if (0==strncmp(buf1, buf2, MAXPATH))
		fprintf(stdout,"Yes: %s and %s are connected by soft link.\n",argv[1], argv[2]);
	else 
		fprintf(stdout,"No: %s and %s are not connected by soft link.\n",argv[1], argv[2]);
	return 0;
}
