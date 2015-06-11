#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
 
#define SAMP_FREC 44100
#define AMPBIT 65536

int main(int argc, char **argv){
  int A = atoi(argv[1]);
  int f = atoi(argv[2]);
  int n = atoi(argv[3]);
  int i;
  short data[n];
  for(i = 0; i < n; i++){
    data[i] = (short)A*sin(2*M_PI*f*i/SAMP_FREC);
  }
  write(1, data, 2*n);
  return 0;
}
