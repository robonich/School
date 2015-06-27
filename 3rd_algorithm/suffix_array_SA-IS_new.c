#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#define LONGEST 1000000

#ifndef bool/* Boolean が定義されていなかったら */
#define bool int
#endif

#ifndef true/* TRUE が定義されていなかったら */
#define true 1
#endif

#ifndef false/* FALSE が定義されていなかったら */
#define false 0
#endif

unsigned char mask[]={0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};//unsigned char は1ビットつまり8バイトのメモリをもつ。そのときこの様に書いてやることで
/*#define tget(i) ( (t[(i)/8]&mask[(i)%8]) ? 1 : 0 )
#define tset(i, b) (t[(i)/8]=(b)?(mask[(i)%8]|t[(i)/8]):((~mask[(i)%8])&t[(i)/8]))
//~mask[i]の論理積をとっているということは、iバイト目だけゼロにするという意味。
#define chr(i) (cs==sizeof(int)?((int*)s)[i]:((unsigned char *)s)[i])
#define isLMS(i) (i>0 && tget(i) && !tget(i-1))
*/
bool tget(int i, unsigned char *t){
  return t[i/8]&mask[i%8] ? 1 : 0;
}

void tset(int i, bool b, unsigned char *t){
  t[i/8] = (b) ? (mask[i%8]|t[i/8]):((~mask[i%8])&t[i/8]);
}

bool isLMS(int i, unsigned char *t){
  return i > 0 && tget(i, t) && !tget(i-1, t);
}

typedef struct answer{
  int strlength;
  int forward_num;
}Answer;

void getBuckets(unsigned char *s, int *bkt, int n, int K, bool end);
void induceSAl(unsigned char *t, int *SA, unsigned char *s, int *bkt, int n, int K, bool end);
void induceSAl(unsigned char *t, int *SA, unsigned char *s, int *bkt, int n, int K, bool end);
void induceSAs(unsigned char *t, int *SA, unsigned char *s, int *bkt, int n, int K, bool end);
void SA_IS(unsigned char *s, int *SA, int n, int K);


int main(int argc, char *argv[]){
  unsigned char *origin = malloc(sizeof(unsigned char)*(LONGEST+1));//'\0'を入れる空間も作る '\0'があるとlast_numがつくりやすい
  int flag;
  int i = 0;
  
  char *filename = argv[1];

  FILE *fp;
  if((fp = fopen(filename, "r")) == NULL) {perror("fopen"); exit(1);}
  while((flag = fgetc(fp)) != EOF){
    if(flag == ' ' || flag == '\n'){}
    else{
      origin[i] = flag;
      i++;
    }
  }
  
  printf("origin ok");

  int longest = i;

  origin[longest] = '\0';

  int *SA = malloc(sizeof(int)*longest+1);

  SA_IS(origin, SA, longest+1, 5);//0~9だからK=10

  printf("\nSA=");
  for(i = 0; i < longest; i++) printf("%d", SA[i]);
  printf("\n");
  printf("suffix\n");
  for(i = 0; i < longest; i++) printf("%s\n", origin+SA[i]);
  printf("\n");

  int j;
  
  Answer ans;
  ans.strlength = 1;//2以上は絶対ある
  ans.forward_num = 0;

  for(i = 0; i < longest - 1; i++){
    j = 0;
    while((origin+SA[i])[j] != '\0' || (origin+SA[i+1])[j] != '\0'){
      if((origin+SA[i])[j] != (origin+SA[i+1])[j]) break;
      j++;
    }
    if(j > ans.strlength){ans.strlength = j; ans.forward_num = i;}
  }
  
  printf("the longest string is\n");
  for(i = 0; i < ans.strlength; i++){
    printf("%c", (origin+SA[ans.forward_num])[i]);
  } 
  printf("\n");

  free(origin);
  fclose(fp);
  return 0;
}



// find the start or end of each bucket
//Kは文字の種類かな
//csはint型なのかchar型なのかを選ぶものだからcs = 1で設定？
//end = 1ならそのバケットの最後の位置を、end = 0ならそのバケットの最初の位置をbktに保存する。
void getBuckets(unsigned char *s, int *bkt, int n, int K, bool end){
  printf("getbuckets in");
  int i, sum=0;
  // clear all buckets
  for(i=0; i<=K; i++) bkt[i]=0;//0~9までの10このバケットと\0の１個のバケット合わせて11このバケット(K=9)
  // compute the size of each bucket
  for(i=0; i<n; i++) bkt[(int)*(s+i)-(int)'0']++;//数字と対応させているバケットなのでこのように記述する
  for(i=0; i<=K; i++){
    sum+=bkt[i];
    bkt[i]=end ? sum : sum-bkt[i];
  }
  printf("getbuckets out");
}

// compute SAl
void induceSAl(unsigned char *t, int *SA, unsigned char *s, int *bkt, int n, int K, bool end){
  printf("induceSAl in");
  int i, j;
  // find starts of buckets
  getBuckets(s, bkt, n, K, end);
  for(i=0; i<n; i++){//前から見ていく
    j=SA[i]-1;
    if(j>=0 && !tget(j, t)) SA[bkt[(int)*(s+j)-(int)'0']++]=j;//tget=1となるのがS, tget=0となるのがLだから!tget==1はLだったらという意味
  }
  printf("induceSAl out");
}

// compute SAs
void induceSAs(unsigned char *t, int *SA, unsigned char *s, int *bkt, int n, int K, bool end){
  printf("induceSAs in");
  int i, j;
  // find ends of buckets
  getBuckets(s, bkt, n, K, end);
  for(i=n-1; i>=0; i--){//後ろから見ていく
    j=SA[i]-1;
    if(j>=0 && tget(j, t)) SA[--bkt[(int)*(s+j)-(int)'0']]=j;//Sだったら
  }
  printf("induceSAs out");
}

// find the suffix array SA of s[0..n-1] in {1..K}ˆn
// require s[n-1]=0 (the sentinel!), n>=2
// use a working space (excluding s and SA) of
// at most 2.25n+O(1) for a constant alphabet

void SA_IS(unsigned char *s, int *SA, int n, int K){
  // LS-type array in bits
  printf("SA_IS in");
  unsigned char *t = malloc(sizeof(unsigned char)*(n/8+1));
  // classify the type of each character
  // the sentinel must be in s1, important!!!　sentinel is \0 n-1が\0ということはnは\0を含んだ数？
  //tset(int i, bool b, unsigned char *t)
  tset(n-2, 0, t);//bが0なら？tsetはsかlかを決めるもののはず
  tset(n-1, 1, t);//bが1なら？
  for(i=n-3; i>=0; i--) tset(i, (int)*(s+i)<(int)*(s+i+1) || (int)*(s+i)<(int)*(s+i+1) && tget(i+1, t)==1 ? 1 : 0, t);

  // stage 1: reduce the problem by at least 1/2
  // sort all the S-substrings
  // bucket array
  int *bkt = (int *)malloc(sizeof(int)*(K+1));//K+1つまり\0の場所も作るということ
  // find ends of buckets
  getBuckets(s, bkt, n, K, true);//trueなら末尾を探す
  for(i=0; i<n; i++) SA[i]=-1;
  for(i=1; i<n; i++) if(isLMS(i, t)) SA[--bkt[(int)*(s+i)-(int)'0']]=i;
					
  induceSAl(t, SA, s, bkt, n, K, false);//前から探す
  induceSAs(t, SA, s, bkt, n, K, true);//後ろから探す
  free(bkt);
  
  // compact all the sorted substrings into
  // the first n1 items of SA
  // 2*n1 must be not larger than n (proveable)
  int n1=0;
  for(i=0; i<n; i++) if(isLMS(SA[i], t)) SA[n1++]=SA[i];
  
  // find the lexicographic names of substrings
  // init the name array buffer
  for(i=n1; i<n; i++) SA[i]=-1;
  int name=0, prev=-1;
  for(i=0; i<n1; i++){
    int pos=SA[i]; bool diff=false;
    int d;
    for(d=0; d<n; d++)
      if(prev==-1 || (int)*(s+pos+d)!=(int)*(s+prev+d) || tget(pos+d, t)!=tget(prev+d, t)){
	diff=true;
	break;
      }
      else if(d>0 && (isLMS(pos+d, t) || isLMS(prev+d, t))) break;
    
    if(diff){
      name++;
      prev=pos;
    }
    pos=(pos%2==0)?pos/2:(pos-1)/2;
    SA[n1+pos]=name-1;
  }
  for(i=n-1, j=n-1; i>=n1; i--) if(SA[i]>=0) SA[j--]=SA[i];
  // stage 2: solve the reduced problem
  // recurse if names are not yet unique
  int *SA1=SA, *s1=SA+n-n1;
  if(name<n1) SA_IS((unsigned char*)s1, SA1, n1, name-1);
  else// generate the suffix array of s1 directly
    for(i=0; i<n1; i++) SA1[s1[i]] = i;
  
  // stage 3: induce the result for
  // the original problem
  // bucket array
  bkt = (int *)malloc(sizeof(int)*(K+1));
  // put all the LMS characters into their buckets
  // find ends of buckets
  getBuckets(s, bkt, n, K, true);
  for(i=1, j=0; i<n; i++)
    if(isLMS(i, t)) s1[j++]=i; // get p1
  // get index in s
  for(i=0; i<n1; i++) SA1[i]=s1[SA1[i]];
  // init SA[n1..n-1]
  for(i=n1; i<n; i++) SA[i]=-1;
  for(i=n1-1; i>=0; i--){
    j=SA[i]; SA[i]=-1;
    SA[--bkt[(int)*(s+j)-(int)'0']]=j;
  }
  induceSAl(t, SA, s, bkt, n, K, false);
  induceSAs(t, SA, s, bkt, n, K, true);
  free(bkt);
  free(t);
  printf("SA_IS out");  
}


