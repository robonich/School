#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#define LONGEST 1000000
#define N 100

#include <stdio.h>

#ifndef bool         /* Boolean が定義されていなかったら */
#define bool int
#endif

#ifndef true            /* TRUE が定義されていなかったら */
#define true 1
#endif

#ifndef false           /* FALSE が定義されていなかったら */
#define false 0
#endif


typedef struct answer{
  long strlength;
  long forward_num;
}Answer;

void Qsort2(long longest, char *data[longest], long, long);

/*int stringcmp(char *, char *);

int stringcmp(char *a, char *b){//suffixが等しくなることはないので等しい場合は考えない
  int diff = 0;
  int i = 0;
  
  while(!(diff = *(unsigned char *)(a+i) - *(unsigned char*)(b+i)) && *(b+i)) i++;
  if(diff < 0) diff = -1;
  else if(diff > 0) diff = 1;
  return diff;
  /*  
  for(i = 0; (diff = a[i] - b[i]) == 0 && a[i] != '\0' && b[i] != '\0'; i++);
  if(diff < 0 || a[i] == '\0') return -1;
  if(diff > 0 || b[i] == '\0') return 1;

}
*/

void Qsort2(long longest, char *data[longest], long leftside, long rightside){
  long pivot, i, j;
  char *temp;
  i = leftside; j = rightside;
  pivot = leftside;
  temp = data[pivot];
  while(i < j){
    while(strncmp(data[j], temp, N) >= 0 && i < j) j--;
    if(i != j){
      data[i] = data[j];
      i++;
    }
    while(strncmp(data[i], temp, N) <= 0 && i < j) i++;
    if(i != j){
      data[j] = data[i];
      j--;
    }
  }
  data[i] = temp;
  pivot = i;
  //printf("%ld\n", counter);
  //counter++;
  if(leftside < pivot) Qsort2(longest, data, leftside, pivot-1);
  if(rightside > pivot) Qsort2(longest, data, pivot+1, rightside);
}

int main(void){
  char *origin = malloc(sizeof(char)*(LONGEST+1));//'\0'を入れる空間も作る
  long i = 0;
  long longest;
  FILE *fp;
  if((fp = fopen("2015.txt", "r")) == NULL) {perror("fopen"); exit(1);}
  while((origin[i] = fgetc(fp)) != EOF){
    if(origin[i] == ' ' || origin[i] == '\n'){}
    else i++;
  }
  
  longest = i;

  origin[longest] = '\0';

  char *data[longest];

  for(i = 0; i < longest; i++) data[i] = origin + i;

  Qsort2(longest, data, 0, longest - 1);

  long j;
  
  Answer ans;
  ans.strlength = 1;//2以上は絶対ある
  ans.forward_num = 0;

  for(i = 0; i < longest - 1; i++){
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
   
  free(origin);
  fclose(fp);
  return 0;
}
