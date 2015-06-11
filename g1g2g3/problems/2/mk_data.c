#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define N 256

int main(int argc, char **argv)
{
  char *filename = argv[1];
  int i;
  int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);  
  unsigned char data[N];
  for(i = 0; i <= N-1; i++){
    data[i] = (unsigned char) i;
  }
  write(fd, data, N);
  close(fd);
  return 0;
 }
 
