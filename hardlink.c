#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#define MAXPATH 1024

int main(int argc, char *argv[])
{
	if (3!=argc)
	{
		fprintf(stderr,"usage: %s path1 path2\n",argv[0]);
		return 1;
	}
	struct stat finfo1;
	if (-1==lstat(argv[1],&finfo1))
	{
		fprintf(stderr,"Cannot stat %s\n",argv[1]);
		return 1;
	}
	struct stat finfo2;
	if (-1==lstat(argv[2],&finfo2))
	{
		fprintf(stderr,"Cannot stat %s\n",argv[2]);
		return 1;
	}
	if (finfo1.st_ino==finfo2.st_ino)
		fprintf(stdout,"Yes: %s and %s are connected by hard link.\n",argv[1], argv[2]);
	else 
		fprintf(stdout,"No: %s and %s are not connected by hard link.\n",argv[1], argv[2]);
	return 0;
}

