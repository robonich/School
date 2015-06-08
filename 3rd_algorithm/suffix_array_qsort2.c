#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#define LONGEST 1000000

typedef struct answer{
  long strlength;
  long forward_num;
}Answer;

void Qsort2(char *data[LONGEST], long, long);

void Qsort2(char *data[LONGEST], long leftside, long rightside){
  long pivot, i, j;
  char *temp;
  i = leftside; j = rightside;
  pivot = leftside;
  temp = data[pivot];
  while(i < j){
    while(strcmp(data[j], temp) >= 0 && i < j) j--;
    if(i != j){
      data[i] = data[j];
      i++;
    }
    while(strcmp(data[i], temp) <= 0 && i < j) i++;
    if(i != j){
      data[j] = data[i];
      j--;
    }
  }
  data[i] = temp;
  pivot = i;
  if(leftside < pivot) Qsort2(data, leftside, pivot-1);
  if(rightside > pivot) Qsort2(data, pivot+1, rightside);
}

int main(void){
  char *data[LONGEST];
  data[0] = (char *)malloc(sizeof(char)*(LONGEST+1));//*(LONGEST+2)/2);//'\0'を入れる空間も作る
  long i = 0;
  FILE *fp;
  if((fp = fopen("2015.txt", "r")) == NULL) {perror("fopen"); exit(1);}
  while((data[0][i] = fgetc(fp)) != EOF){
    if(data[0][i] == ' ' || data[0][i] == '\n'){}
    else i++;
  }
  data[0][LONGEST] = '\0';

  for(i = 0; i < LONGEST; i++) data[i] = data[0] + i;

  Qsort2(data, 0, LONGEST - 1);

  long j;
  
  Answer ans;
  ans.strlength = 1;//2以上は絶対ある
  ans.forward_num = 0;

  for(i = 0; i < LONGEST - 1; i++){
    j = 0;
    while(data[i][j] != '\0' || data[i+1][j] != '\0'){
      if(data[i][j] != data[i+1][j]) break;
      j++;
    }
    if(j > ans.strlength){ans.strlength = j; ans.forward_num = i;}
  }
    
  printf("the longest string is\n");
  for(i = 0; i < ans.strlength; i++){
    printf("%c",data[ans.forward_num][i]);
  } 
  printf("\n");
   
  fclose(fp);
  return 0;
}
