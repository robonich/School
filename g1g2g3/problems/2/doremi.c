#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

#define DO 523 
#define DURATION 13230
#define SAMP_FREC 44100

int main(int argc, char** argv){
  int A = atoi(argv[1]);
  int n = atoi(argv[2]);
  short data[n*DURATION];
  int i, j;
  double frec;
  for(i = 0; i < n; i++){
    if(i%7 == 0) frec = DO * pow(2, 12*((int)i/7)/12.0);
    else if(i%7 == 1) frec = DO * pow(2, (12*((int)i/7) + 2)/12.0);
    else if(i%7 == 2) frec = DO * pow(2, (12*((int)i/7) + 4)/12.0);
    else if(i%7 == 3) frec = DO * pow(2, (12*((int)i/7) + 5)/12.0);
    else if(i%7 == 4) frec = DO * pow(2, (12*((int)i/7) + 7)/12.0);
    else if(i%7 == 5) frec = DO * pow(2, (12*((int)i/7) + 9)/12.0);
    else if(i%7 == 6) frec = DO * pow(2, (12*((int)i/7) + 11)/12.0);
    for(j = 0; j < DURATION; j++) data[DURATION*i + j] = (short)A*sin(2.0*M_PI*frec*(DURATION*i + j)/SAMP_FREC);
  }
  write(1, data, 2*n*DURATION);
  return 0;
}
