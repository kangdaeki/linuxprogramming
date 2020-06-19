#include <stdio.h>
#include <string.h>

#define LINLEN 1024

int main(int argc, char *argv[])
{
    if (2!=argc)
    {
        fprintf(stderr, "Usage: %s <file>\n",argv[0]);
        return 1;
    }
    FILE *fp=fopen(argv[1],"rt");
	if (NULL==fp) {
        fprintf(stderr, "Cannot open %s\n", argv[1]);
        return 1;
	}
	char buf[LINLEN]={};
	while(fgets(buf, LINLEN, fp) != NULL)
    {
        while ('\n'==buf[strlen(buf)-1]) buf[strlen(buf)-1]=0;
		for (int i=strlen(buf)-1; i>=0 ; i--) fprintf(stdout, "%c", buf[i]);
		fprintf(stdout, "\n");
    }
    fclose(fp);
    return 0;
}

