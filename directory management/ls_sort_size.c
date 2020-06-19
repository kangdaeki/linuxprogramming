#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#define MAXFNAME 1024
#define MAXFILES 1024

struct mydirent
{
  char d_name[MAXFNAME];
  blkcnt_t nblocks;
} my_dirents[MAXFILES];
int nfiles;

int cmpfunc(const void *a, const void *b)
{
  struct mydirent* da=(struct mydirent *)a;
  struct mydirent* db=(struct mydirent *)b;

  if (da->nblocks > db->nblocks) return 1;
  else if (da->nblocks < db->nblocks) return -1;
  else return 0;
}

int main(int argc, char *argv[])
{
  if (2!=argc)
  {
     fprintf(stderr, "usage: %s <path>\n", argv[0]);
     exit(1);
  }
  DIR *dirp=opendir(argv[1]);
  if (NULL==dirp) 
  {
     perror(argv[1]);
     exit(1);
  }

  struct dirent *dentry;
  struct stat finfo;
  nfiles=0;
  while (dentry=readdir(dirp))
    if (0!=dentry->d_ino)
    {
      stat(dentry->d_name, &finfo);
      fprintf(stdout, "%s, %ld\n", dentry->d_name, finfo.st_blocks);

      strncpy(my_dirents[nfiles].d_name, dentry->d_name, MAXFNAME);
      my_dirents[nfiles].nblocks=finfo.st_blocks;
      nfiles++;
    }

  qsort(my_dirents,nfiles,sizeof(struct mydirent),cmpfunc);

  fprintf(stdout, "\nSorted:\n");
  for (int i=0;i<nfiles;i++)
    fprintf(stdout, "%s, %ld\n", my_dirents[i].d_name, my_dirents[i].nblocks);
  closedir(dirp);
  return 0;
}

