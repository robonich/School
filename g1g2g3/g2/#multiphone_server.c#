#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <assert.h>
#include <complex.h>
#include <math.h>
#include <string.h>
#include <unistd.h>

typedef short sample_t;

#define N 1000

void die(char *);
ssize_t read_n(int, ssize_t, void *);
ssize_t write_n(int, ssize_t, void *);
void sample_to_complex(sample_t *, complex double *, long);
void complex_to_sample(complex double *, sample_t *, long);
void fft_r(complex double *, complex double *, long, complex double);
void fft(complex double *, complex double *, long);
void ifft(complex double *, complex double *, long);
int pow2check(long);


int main(int argc, char *argv[]){

  long n = 1024;//2のn乗であればよい
  int low_f, high_f;
  sample_t * buf = calloc(sizeof(sample_t), n);
  complex double * X = calloc(sizeof(complex double), n);
  complex double * Y = calloc(sizeof(complex double), n);
  int b = 0;

  int s, ss;
  struct sockaddr_in serv_addr, client_addr;

  if(argc == 4){
    time_t t1;  
    time(&t1);

    low_f = atoi(argv[2]); high_f = atoi(argv[3]);
    
    ss = socket(PF_INET, SOCK_STREAM, 0);//UDPならSOCK_DGRAM
    if(ss == -1){perror("socket"); exit(1);}
    
    char *port = argv[1];

    serv_addr.sin_family = AF_INET; /* これは IPv4 のアドレスです */
    serv_addr.sin_port = htons(atoi(port)); /* ポートは ... です */
    serv_addr.sin_addr.s_addr = INADDR_ANY; /*どのIPアドレスでも待ち受けしたいです*/
    bind(ss, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    int l = listen(ss, 10);
    
    socklen_t len = sizeof(struct sockaddr_in);
   
    s = accept(ss, (struct sockaddr *)&client_addr, &len);
    if(s == -1){perror("accept"); exit(1);}

   
    unsigned char data2[N];

    time_t t2;
    time(&t2);
    int timers = difftime(t2, t1);
    //  printf("%d\n", timers);
    int bytes = timers*44100*2+2;
 
    while(bytes > 0){
      int n_read2 = read(0, data2, N);
      if(n_read2 == -1) {perror("n_read2"); exit(1);}
      bytes -= N;
    }
  }

  socklen_t client_addr_len;

  while(1){
    client_addr_len = sizeof(client_addr);
    int n_recv = recvfrom(s, buf, n*sizeof(sample_t), 0, (struct sockaddr *)&client_addr, &client_addr_len);//recvfromとかもあるよ
    if(n_recv == -1){perror("n_recv"); exit(1);};
    if(n_recv == 0) break;

    ssize_t n_read = read_n(0, n * sizeof(sample_t), buf);
    if (n_read == 0) break;
   
    int n_send = send(s, buf, n_read, 0);//s
    if(n_send == -1) {perror("n_send"); exit(1);}
    if(n_send == 0) break;
  }

  if(argc == 4) shutdown(ss, SHUT_WR);
  else if(argc == 5) shutdown(s, SHUT_WR);

  return 0;
}


void die(char * s) {
  perror(s); 
  exit(1);
}

/* fd から 必ず n バイト読み, bufへ書く.
   n バイト未満でEOFに達したら, 残りは0で埋める.
   fd から読み出されたバイト数を返す */
ssize_t read_n(int fd, ssize_t n, void * buf) {
  ssize_t re = 0;
  while (re < n) {
    ssize_t r = read(fd, buf + re, n - re);
    if (r == -1) die("read");
    if (r == 0) break;
    re += r;
  }
  memset(buf + re, 0, n - re);
  return re;
}

/* fdへ, bufからnバイト書く */
ssize_t write_n(int fd, ssize_t n, void * buf) {
  ssize_t wr = 0;
  while (wr < n) {
    ssize_t w = write(fd, buf + wr, n - wr);
    if (w == -1) die("write");
    wr += w;
  }
  return wr;
}

/* 標本(整数)を複素数へ変換 */
void sample_to_complex(sample_t * s, 
		       complex double * X, 
		       long n) {
  long i;
  for (i = 0; i < n; i++) X[i] = s[i];
}

/* 複素数を標本(整数)へ変換. 虚数部分は無視 */
void complex_to_sample(complex double * X, 
		       sample_t * s, 
		       long n) {
  long i;
  for (i = 0; i < n; i++) {
    s[i] = creal(X[i]);
  }
}

/* 高速(逆)フーリエ変換;
   w は1のn乗根.
   フーリエ変換の場合   偏角 -2 pi / n
   逆フーリエ変換の場合 偏角  2 pi / n
   xが入力でyが出力.
   xも破壊される
 */
void fft_r(complex double * x, 
	   complex double * y, 
	   long n, 
	   complex double w) {
  if (n == 1) { y[0] = x[0]; }
  else {
    complex double W = 1.0; 
    long i;
    for (i = 0; i < n/2; i++) {
      y[i]     =     (x[i] + x[i+n/2]); /* 偶数行 */
      y[i+n/2] = W * (x[i] - x[i+n/2]); /* 奇数行 */
      W *= w;
    }
    fft_r(y,     x,     n/2, w * w);
    fft_r(y+n/2, x+n/2, n/2, w * w);
    for (i = 0; i < n/2; i++) {
      y[2*i]   = x[i];
      y[2*i+1] = x[i+n/2];
    }
  }
}

void fft(complex double * x, 
	 complex double * y, 
	 long n) {
  long i;
  double arg = 2.0 * M_PI / n;
  complex double w = cos(arg) - 1.0j * sin(arg);
  fft_r(x, y, n, w);
  for (i = 0; i < n; i++) y[i] /= n;
}

void ifft(complex double * y, 
	  complex double * x, 
	  long n) {
  double arg = 2.0 * M_PI / n;
  complex double w = cos(arg) + 1.0j * sin(arg);
  fft_r(y, x, n, w);
}

int pow2check(long nn) {
  long n = nn;
  while (n > 1) {
    if (n % 2) return 0;
    n = n / 2;
  }
  return 1;
}
