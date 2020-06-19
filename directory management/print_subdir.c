#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#define MAXPATH 2048

void print_dir(char *dirname)
{
	DIR *dirp=opendir(dirname);
	if (NULL==dirp) return;
	struct dirent *dentry;
	while (NULL!=(dentry=readdir(dirp)))
	{
		if ( (0!=dentry->d_ino) && (DT_DIR==dentry->d_type))
		{
			if (0==strcmp(".",dentry->d_name)) continue;
			if (0==strcmp("..",dentry->d_name)) continue;
			fprintf(stdout, "%s/%s\n",dirname,dentry->d_name);
			char path[MAXPATH]={};
			sprintf(path,"%s/%s",dirname,dentry->d_name);
			print_dir(path);
		}
	}
}

int main(int argc, char *argv[])
{
	if (2!=argc)
	{
		fprintf(stderr,"usage: %s <dir name>\n", argv[0]);
		return 1;
	}
	print_dir(argv[1]);
	return 0;
}
