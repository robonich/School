#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>


#define N 256

int main(int argc, char **argv)
{
  char *filename = argv[1];
  int i;
  int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if(fd == -1) {perror("open"); exit(1);}
  unsigned char data[N];
  for(i = 0; i <= N-1; i++){
    data[i] = (unsigned char) i;
  }
  int is_wrote = 0;
  is_wrote = write(fd, data, N);
  if(is_wrote == -1) {perror("write");exit(1);}
  close(fd);
  return 0;
 }
 
