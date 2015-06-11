#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define N 100

int main(int argc, char **argv){
  int fd = open(argv[1], O_RDONLY);
  if(fd == -1){perror("open"); exit(1);}
  

  int all_2bytes = 0;
  short data[N];
  while(1){
    int n, i;
    n = read(fd, data, 2*N);
    if(n == -1){perror("read"); exit(1);}
    if(n == 0) break;
    for(i = 0; i < n/2.0; i++, all_2bytes++){ 
      printf("%d %d\n", all_2bytes, data[i]);
    }
  }
  
  close(fd);
  return 0;
}
