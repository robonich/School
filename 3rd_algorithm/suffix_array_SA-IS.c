#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#define LONGEST 1000000//646//20

#define N 100

typedef struct answer{
  long strlength;
  long forward_num;
}Answer;

void Qsort2(long longest, char *data[longest], long, long);

//L、Sの等号はどうする？同じ長さのsuffixは存在しないので等号はいらないType L は　S1 > s2, Type S は S1 < S2　

void LMS_sort(long longest, char *bucket[longest+1], char *origin, long *num_last, long *num_of_S, char *LS){
  long i;
  bucket[0] = origin + longest;//'/0'ということ
  for(i = 1 ; i < longest; i++){//data[LONGEST-1] = '数字\0'というかたちで絶対LMSにならないから除外
    if(LS[i] == 'S' && LS[i-1] == 'L'){//LMSなら
      bucket[num_last[(origin+i)[0]-'0'] - num_of_S[(origin+i)[0]-'0']] = origin + i;
      num_of_S[(origin+i)[0] - '0']++;//増やす
    }
  }
  /* for(i = 0; i < 10; i++){
    Qsort2(longest, bucket, num_last[i] - num_of_S[i] + 1, num_last[i]);//LMSのバケットごとのソート
    }*/
}


//LMSのソートが終わったら「上から順番にSuffix Siを検索」「Si-1がL-typeならバケットの一番上の空きに挿入」でLtypeを挿入して、「下から順番にSuffix Siを検索」「Si-1がS-typeならバケットの一番下の空きに挿入」でStypeを挿入する（ただしLMSは除く）
void induced_sort(long longest, char *bucket[longest+1], char *origin, long *num_begin, long *num_last, long *num_of_L, long *num_of_S, char *LS){
  long i, j;
  bucket[num_begin[origin[longest-1]-'0']] = origin + longest - 1;
  num_of_L[origin[longest-1]-'0']++;
  for(i = 1; i < longest; i++){//bucket[i]に入っているsuffixの開始位置はlongest - strlen(bucket[i])であらわせる。-1してそれの一個下を見る。Lであれば上から空欄に入れていく（バケットはもちろん分ける）//またi = 0のときは絶対'\0'が入ってるから、それについてはループの外で行う。つまり開始位置longest-1のsuffixは絶対にLなので、先にいれておく。
    if(bucket[i] == '\0') continue;
    j = longest - strlen(bucket[i]) -1;//jは開始位置の一つ前
    if(j == -1 || j == longest - 1) continue;//開始位置が0つまりj=-1ならとばす。また開始位置がlongestつまり'\0',j=longest-1のときも飛ばす。
    if(LS[j] == 'L'){
      bucket[num_begin[(origin+j)[0]-'0'] + num_of_L[(origin+j)[0]-'0']] = origin+ j;
      num_of_L[(origin+j)[0]-'0']++;
    }
  }
  /*
  printf("Ltype sorted\n");
  for(i = 0; i < longest + 1; i++){
    printf("%ld:%s\n", i, bucket[i]);
  }
  */
  //Stype

  char num_of_S2[10] = {};

  for(i = longest; i >= 1; i--){
    if(bucket[i] == '\0') continue;
    j = longest - strlen(bucket[i]) -1;
    if(j == -1 || j == longest - 1) continue;
    if(LS[j] == 'S'){
      //if(LS[j-1] == 'L') continue;//LMSなら飛ばす
      bucket[num_last[(origin+j)[0]-'0'] - num_of_S2[(origin+j)[0]-'0']] = origin + j;

      num_of_S2[(origin+j)[0] - '0']++;
      //num_of_S[(origin+j)[0] - '0']++;//num_of_Sとかnum_of_Lをmainにつっこもう。
    }
  }
}

int main(int argc, char *argv[]){
  char *origin = malloc(sizeof(char)*(LONGEST+1));//'\0'を入れる空間も作る '\0'があるとlast_numがつくりやすい
  long appear_num[10] = {};//0~9の文字の先頭の出現回数
  long num_last[10];//各バケツの最後ただし'\0'は除く
  long num_begin[10];//各バケツの最初ただし'\0'は除く
  long num_of_S[10] = {};
  long num_of_L[10] = {};
  long i = 0;
  
  char *filename = argv[1];

  FILE *fp;
  if((fp = fopen(filename, "r")) == NULL) {perror("fopen"); exit(1);}
  while((origin[i] = fgetc(fp)) != EOF){
    if(origin[i] == ' ' || origin[i] == '\n'){}
    else{
      //data[i] = origin + i;//suffix作成完了
      appear_num[origin[i]-'0']++;//文字ごとの出現回数を記録
      //bucket[i] = '\0';//初期化（全部できるようになったらいらない）
      i++;
    }
  }

  long longest = i;

  origin[longest] = '\0';

  //printf("data loaded successfully");

  //printf("\n%c\n", (origin+10)[0]);

  char *LS = malloc(sizeof(char)*(longest+1));
  
  char *bucket[longest+1];

  bucket[longest] =  '\0';
  LS[longest] = 'S';

  for(i = longest - 1; i >= 0; i--){
    bucket[i] = '\0';
    LS[i] = (origin[i] == origin[i+1]) ? LS[i+1] : ((origin[i] < origin[i+1]) ? 'S' : 'L'); //等号の時はorigin+iのtypeはorigin+i+1と同じになる.なぜかというと、a[i] = a[i+1] < or > or = a[i+2]だから
  }

  //printf("\n%s\n%s", origin, LS);
	 
  for(i = 0; i < 10; i++){
    num_last[i] = ((i == 0) ? 0 : (num_last[i-1])) + appear_num[i];
    num_begin[i] = (appear_num[i] == 0 ? num_last[i] : num_last[i]-(appear_num[i]-1));
    //printf("%ld:%ld,%ld\n", i, num_begin[i], num_last[i]);
  }
  
  //printf("num_last, num_begin ok\n");

  LMS_sort(longest, bucket, origin, num_last, num_of_S, LS);
 
  /*  printf("LMS ok\n");
  
  printf("bucket\n");
  for(i = 0; i < longest+1; i++){
    printf("%ld:%s\n", i, bucket[i]);
  }
  */
  induced_sort(longest, bucket, origin, num_begin, num_last, num_of_L, num_of_S, LS);
  
   printf("induced ok\n");
  
 /* printf("bucket\n");
  for(i = 0; i < longest+1; i++){
    printf("%ld:%s\n", i, bucket[i]);
  }
*/	 

  long j;
  
  Answer ans;
  ans.strlength = 1;//2以上は絶対ある
  ans.forward_num = 0;

  for(i = 0; i < longest - 1; i++){
    j = 0;
    while(bucket[i][j] != '\0' || bucket[i+1][j] != '\0'){
      if(bucket[i][j] != bucket[i+1][j]) break;
      j++;
    }
    if(j > ans.strlength){ans.strlength = j; ans.forward_num = i;}
  }
  
  printf("the longest string is\n");
  for(i = 0; i < ans.strlength; i++){
    printf("%c", bucket[ans.forward_num][i]);
  } 
  printf("\n");

  free(origin);
  fclose(fp);
  return 0;
}


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
