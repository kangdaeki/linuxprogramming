#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define LINELEN 1024

int get_no_words(char *line)
{
    for (int i=0;i<strlen(line);i++) if (('\t'==line[i])||('\r'==line[i])||('\n'==line[i])) line[i]=' ';
    int count=0;
    for (int i=0;i<strlen(line);i++) 
        if (0==i)
        {
            if (' '!=line[i]) count++;
        }
        else if ((' '==line[i-1])&&(' '!=line[i])) count++;
    return count;
}

int main(int argc, char *argv[])
{
    do
    {
        fprintf(stdout, "(myshell) $ ");
        char line[LINELEN]={};
        fgets(line, LINELEN, stdin);
        for (int i=strlen(line)-1;i>=0;i--) if ((' '==line[i])||('\t'==line[i])||('\r'==line[i])||('\n'==line[i])) line[i]=0; else break;
        if (0==strcmp("quit",line)) break;
        char *line_ptr=line;
        pid_t pid=fork();
        if (0==pid)
        {
            int nwords=get_no_words(line_ptr);
            char *(*words)[]=(char *(*)[])malloc((1+nwords)*sizeof(char *));
            int count=0;
            char *word=NULL;
            while (NULL!=(word=strsep(&line_ptr," ")))
            {
                if (strlen(word)>0)
                {
                    (*words)[count]=word;
                    count++;
                }
            }
            (*words)[count]=(char *)0;
            execvp((*words)[0],*words);
            fprintf(stderr, "Cannot exec %s\n",(*words)[0]);
            free(words);
            return 1;
        }
        else if (0>pid)
        {
            fprintf(stderr, "cannot fork\n");
            return 2;
        }
        int status=0;
        wait(&status);
    } while (1);
    return 0;
}