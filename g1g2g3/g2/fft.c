/* 
 * fft.c
 * 使い方
 *   ./fft n
 * 
 * 以下を繰り返す:
 *   標準入力から, 16 bit integerをn個読む
 *   FFTする
 *   逆FFTする
 *   標準出力へ出す
 *
 * したがって「ほぼ何もしない」フィルタになる
 * 
 */
#include <assert.h>
#include <complex.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

typedef short sample_t;

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

int pow2check(long N) {
  long n = N;
  while (n > 1) {
    if (n % 2) return 0;
    n = n / 2;
  }
  return 1;
}

void print_complex(FILE * wp, 
		   complex double * Y, long n) {
  long i;
  for (i = 0; i < n; i++) {
    fprintf(wp, "%ld %f %f %f %f\n", 
	    i, 
	    creal(Y[i]), cimag(Y[i]),
	    cabs(Y[i]), atan2(cimag(Y[i]), creal(Y[i])));
  }
}


int main(int argc, char ** argv) {
  (void)argc;
  int i = 0;
  long n = 1024;
  if (!pow2check(n)) {
    fprintf(stderr, "error : n (%ld) not a power of two\n", n);
    exit(1);
  }
  int wp1 = open("bill-gates.raw", O_RDONLY);
  if (wp1 == -1) die("open1");
  int wp2 = open("breakshot.raw", O_RDONLY);
  if (wp2 == -1) die("open2");

  sample_t * buf1 = calloc(sizeof(sample_t), n);
  sample_t * buf2 = calloc(sizeof(sample_t), n);
  complex double * X = calloc(sizeof(complex double), n);
  complex double * Y = calloc(sizeof(complex double), n);
  complex double * Z = calloc(sizeof(complex double), n);
  complex double * W = calloc(sizeof(complex double), n);
  while (1) {
    /* ファイルからn個標本を読む */
    ssize_t m1 = read_n(wp1, n * sizeof(sample_t), buf1);
    if (m1 == 0) break;
    ssize_t m2 = read_n(wp2, n * sizeof(sample_t), buf2);
    if (m2 == 0) break;
    
    /* 複素数の配列に変換 */
    sample_to_complex(buf1, X, n);
    sample_to_complex(buf2, Z, n);

    /* FFT -> Y */
    fft(X, Y, n);
    fft(Z, W, n);
    int b;
    for(b = 0; b < n; b++){
      Z[b] = W[b] + Y[b];
    }    
  
    /* IFFT -> Z */
    ifft(Z, X, n);
  /* 標本の配列に変換 */
    complex_to_sample(X, buf1, n);
    /* 標準出力へ出力 */
    write_n(1, m1, buf1);
  }
  close(wp1);
  close(wp2);
  return 0;
}
