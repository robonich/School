#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#define LONGEST 646

int counter = 0;

int med3(char *data[LONGEST], int, int, int);
int compare_string(char*, char*);
void Qsort(char *data[LONGEST], int, int);

int med3_atol(char *data[LONGEST], long, long, long);
int compare_string_atol(char*, char*);
void Qsort_atol(char *data[LONGEST], long, long);

int med3_atol(char *data[LONGEST], long x, long y, long z){
  if(compare_string_atol(data[x], data[y]) < 0)
    if(compare_string_atol(data[y], data[z]) < 0) return y;
    else if(compare_string_atol(data[z], data[x]) < 0) return x;
    else return z;
  else if(compare_string_atol(data[z], data[y]) < 0) return y;
  else if(compare_string_atol(data[x], data[z]) < 0) return x;
  else return z;
}

int med3(char *data[LONGEST], int x, int y, int z){//data[x], data[y], data[z]のなかの中間値を返す
  if(compare_string(data[x], data[y]) < 0)
    if(compare_string(data[y], data[z]) < 0) return y;
    else if(compare_string(data[z], data[x]) < 0) return x;
    else return z;
  else if(compare_string(data[z], data[y]) < 0) return y;
  else if(compare_string(data[x], data[z]) < 0) return x;
  else return z;
}

int compare_string_atol(char *a, char *b){//aがbより先にあれば負、同じなら0、後なら正
  if(atol(a) < atol(b)) return -1;
  if(atol(a) == atol(b)) return 0;
  if(atol(a) > atol(b)) return 1;
}
  
int compare_string(char *a, char *b){//aがbより先にあれば負、同じなら0、後なら正
  int i = 0;
  while(a[i] != '\0' || b[i] != '\0'){
    if(a[i] < b[i]) {return -1; break;}
    if(a[i] == b[i]) i++;
    if(a[i] > b[i]) {return 1; break;}
  }
  if(a[i] == '\0' && b[i] == '\0') return 0;
  else if(a[i] == '\0') return -1;
  else if(b[i] == '\0') return 1;
} 

void Qsort_atol(char *data[LONGEST], long leftside, long rightside){
  if(leftside < rightside){
    char *temp;
    long i = leftside, j = rightside;
    long pivot = med3_atol(data, i, (i+j)/2, j);
    while(1){
      while(compare_string_atol(data[i], data[pivot]) < 0) i++;
      while(compare_string_atol(data[pivot], data[j]) < 0) j--;
      if(i >= j) break;
      temp = data[i]; data[i] = data[j]; data[j] = temp;
      i++; j--;
    }
    counter++;
    printf("%d\n", counter);
    Qsort_atol(data, leftside, i - 1);
    Qsort_atol(data, j + 1, rightside);
  }
}

void Qsort(char *data[LONGEST], int leftside, int rightside){
  if(leftside < rightside){
    char *temp;
    int i = leftside, j = rightside;
    int pivot = med3(data, i, (i+j)/2, j);
    while(1){
      while(compare_string(data[i], data[pivot]) < 0) i++;
      while(compare_string(data[pivot], data[j]) < 0) j--;
      if(i >= j) break;
      temp = data[i]; data[i] = data[j]; data[j] = temp;
      i++; j--;
    }
    counter++;
    printf("%d\n", counter);
    Qsort(data, leftside, i - 1);
    Qsort(data, j + 1, rightside);
  }
}

int main(void){
  char *data[LONGEST];
  data[0] = (char *)malloc(sizeof(char)*(LONGEST+1));//*(LONGEST+2)/2);//'\0'を入れる空間も作る
  long i = 0;
  FILE *fp;
  if((fp = fopen("short.txt", "r")) == NULL) {perror("fopen"); exit(1);}
  while((data[0][i] = fgetc(fp)) != EOF){
    if(data[0][i] == ' ' || data[0][i] == '\n'){}
    else i++;
  }
  data[0][LONGEST] = '\0';

  for(i = 0; i < LONGEST; i++) data[i] = data[0] + i;

  printf("before Qsort\n");
  for(i = LONGEST - 3; i < LONGEST; i++){
    printf("%ld:%s\n", i, data[i]);
  }

  Qsort_atol(data, 0, LONGEST - 1);

  printf("after Qsort\n");

  for(i = LONGEST - 3; i < LONGEST; i++){
    printf("%ld:%s\n", i, data[i]);
  }
  
  fclose(fp);
  // free(data[0]);
  return 0;
}
