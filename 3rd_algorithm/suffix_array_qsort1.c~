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


int med3(char *data[LONGEST], long, long, long);
int compare_string(char*, char*);
void Qsort(char *data[LONGEST], long, long);

int med3(char *data[LONGEST], long x, long y, long z){//data[x], data[y], data[z]のなかの中間値を返す
  if(strcmp(data[x], data[y]) < 0)
    if(strcmp(data[y], data[z]) < 0) return y;
    else if(strcmp(data[z], data[x]) < 0) return x;
    else return z;
  else if(strcmp(data[z], data[y]) < 0) return y;
  else if(strcmp(data[x], data[z]) < 0) return x;
  else return z;
} 

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


void Qsort(char *data[LONGEST], long leftside, long rightside){
  if(leftside < rightside){
    char *temp;
    long i = leftside, j = rightside;
    long pivot = med3(data, i, (i+j)/2, j);
    while(1){
      while(strcmp(data[i], data[pivot]) < 0) i++;
      while(strcmp(data[pivot], data[j]) < 0) j--;
      if(i >= j) break;
      temp = data[i]; data[i] = data[j]; data[j] = temp;
      i++; j--;
    }
    Qsort(data, leftside, i - 1);
    Qsort(data, j + 1, rightside);
  }
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

  /*printf("before Qsort\n");
  for(i = LONGEST - 3; i < LONGEST; i++){
    printf("%ld:%s\n", i, data[i]);
    }
  */
  Qsort2(data, 0, LONGEST - 1);

  /*printf("after Qsort\n");

  for(i = 0; i < 20; i++){
    printf("%ld:%s\n", i, data[i]);
    }
  */
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
  
  //printf("forward_num = %ld, strlength = %ld", ans.forward_num, ans.strlength);
  
  printf("the longest string is\n");
  for(i = 0; i < ans.strlength; i++){
    printf("%c",data[ans.forward_num][i]);
  } 
  printf("\n");
  
  
  fclose(fp);
  // free(data[0]);
  return 0;
}
