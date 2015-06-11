#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>

#define N 100

int main(int argc, char** argv){
  short data[N];
  short data2[N];
  int mabiki = atoi(argv[1]);
  int n;
  int i;
  int j;
  while(1){
    n = read(0, data, N);
    if(n == 0) break;
    if(n == -1){ perror("read"); exit(1);}
    for(i = 0; i < n/2; i++){
      if(i%mabiki == 0){
	data2[j] = data[i];
	j++;
      }
      write(1, data2, sizeof(short)*j);
      j = 0;
      
    }
  }
  return 0;
}
