#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
//#include <netinet/udp.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

#define N 100

void die(char*);

int main(int argc,char** argv){
  if(argc != 3)die("input");
  //argv[1] ip_address
  //argv[2] port number

  int sfd = socket(PF_INET,SOCK_STREAM,0);
  if(sfd == -1) die("socket");
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  //addr.sin_addr.s_addr = inet_addr(argv[1]);
  inet_aton(argv[1],&addr.sin_addr);
  addr.sin_port = htons(atoi(argv[2]));
  int ret = connect(sfd,(struct sockaddr*)&addr,sizeof(addr));
  if(ret == -1) die("connect");
  char* msg0 ="connect ok\n";
  write(2,msg0,15);

  FILE* fp;
  char* cmdline = "rec -t raw -b 16 -c 1 -e s -r 44100 -";
  fp = popen(cmdline,"r");
  unsigned char data[N];
  
  while(1){
    //int n = read(0,data,N);
    int n = fread(data,sizeof(char),N,fp);
    if(n== -1)die("read");
    if(n==0) break;
    if(send(sfd,data,n,0)==-1)die("send");
  
    n = recv(sfd,data,N,0);
    if(n == -1)die("recv");
    if(n==0) break;
    if(write(1,data,n)==-1)die("write");
  }

  char* msg1 = "The end\n";
  write(2,msg1,10);
  return 0;
}

void die(char* s){
  perror(s);
  exit(1);
}
