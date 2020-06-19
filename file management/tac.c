#include <stdio.h>
#include <string.h>

#define MAXLIN 10000
#define LINLEN 1024

int main(int argc, char *argv[])
{
    if (2!=argc)
    {
        fprintf(stderr, "Usage: %s <file>\n",argv[0]);
        return 1;
    }
    long int line_start[MAXLIN]={};
    int num_lines=1;
    FILE *fp=fopen(argv[1],"rt");
    int ch=fgetc(fp);
    while (EOF!=ch) 
    {
        if ('\n'==ch) 
        {
            line_start[num_lines]=ftell(fp);
            num_lines++;
        }
        ch=fgetc(fp);
    }
//    fprintf(stdout, "# of new line chars: %d\n",num_lines);
    char line[LINLEN]={};
    for (int i=num_lines-1;0<=i;i--)
    {
        fseek(fp,line_start[i],SEEK_SET);
        fgets(line,LINLEN,fp);
        if ('\n'==line[strlen(line)-1]) line[strlen(line)-1]=0;
        fprintf(stdout, "%s\n", line);
    }

    fclose(fp);
    return 0;
}