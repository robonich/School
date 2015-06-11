#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#define LONGEST 1000000

int main(void){
  char data[LONGEST];
  int i = 0, j, k;
  FILE *fp;
  if((fp = fopen("2015.txt", "r")) == NULL) {perror("fopen"); exit(1);}
  while((data[i] = fgetc(fp)) != EOF){
    if(data[i] == ' ' || data[i] == '\n') data[i] = '\0';
    else i++;
  }
  //printf("%s\n", data);
    
  int flag = 0;
  int syukai = 1;//何周目か
    
  while(flag == 0){
    for(j = 0; j < syukai; j++){//何文字かを決める
      for(k = 1; k < syukai + 1; k++){
	for(i = 0; i < LONGEST - syukai; i++){
	  if(data[j+i] != data[j+k+i]) break;
	}
	if(i == LONGEST - syukai) flag = 1;
      }
      if(flag == 1){
	for(k = j; k < LONGEST - syukai + j; k++){
	  printf("%c", data[k]);
	}
	printf("\n");
	break;
      }
    }
    printf("syukai = %d\n", syukai);
    syukai++;
  }
  fclose(fp);
  return 0;
}
