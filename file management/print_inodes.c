#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	if (1>=argc) 
	{
		fprintf(stdout, "usage: %s files \n",argv[0]);
		return 1;
	}
	struct stat finfo;
	for (int i=1;i<argc;i++)
	{
		if (-1==stat(argv[i],&finfo))
		{
			fprintf(stderr, "Cannot stat %s\n",argv[i]);
			return 1;
		}
		fprintf(stdout, "Inode # of %s: %d\n",argv[i],finfo.st_ino);
	}
	return 0;
}

