#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#define LONGEST 20

int main(void){
  char *data[LONGEST];
  data[0] = (char *)malloc(sizeof(char)*(LONGEST+1));//*(LONGEST+2)/2);//'\0'を入れる空間も作る
  int i = 0, j;
  /*for(i = 1; i < LONGEST; i++){
    data[i] = data[0] + i*(LONGEST+1) - i*(i+1)/2;
    }*/
  FILE *fp;
  if((fp = fopen("veryshort.txt", "r")) == NULL) {perror("fopen"); exit(1);}
  while((data[0][i] = fgetc(fp)) != EOF){
    if(data[0][i] == ' ' || data[0][i] == '\n'){}
    else{
      i++;
      //printf("%d:%s\n", i, data[0]);
      /* data[i] = data[0] + i*(LONGEST+1) - i*(i+1)/2 + 1;
      for(j = 1; j < i + 1; j++){
	data[j][i-j] = data[0][i];
	printf("%d:%s\n", j, data[i]);
	}*/
    }
  }
  data[0][LONGEST] = '\0';
  for(i = 0; i < LONGEST; i++){
    data[i] = data[0] + i;
  }
  for(i = LONGEST - 3; i < LONGEST; i++){
    printf("%d:%s\n", i, data[i]);
  }
  printf("%c\n", data[LONGEST-1][0]);
  
  //  qsort(data, LONGEST, );

  fclose(fp);
  free(data[0]);
  return 0;
}
