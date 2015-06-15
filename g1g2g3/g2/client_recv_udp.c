#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#define N 1000

int main(int argc, char *argv[]){
  int s = socket(PF_INET, SOCK_DGRAM, 0);//UDPならSOCK_DGRAM
  if(s == -1){perror("socket"); exit(1);}

  char *ip = argv[1];
  char *port = argv[2];

  struct sockaddr_in addr;
 
  addr.sin_family = AF_INET; /* これは IPv4 のアドレスです */
  //addr.sin_addr.s_addr = inet_addr(ip); /* IP アドレスは ... です */
  int inet = inet_aton(ip, &addr.sin_addr);//上の強いバージョン
  if(inet == 0){perror("inet_aton"); exit(1);}
  addr.sin_port = htons(atoi(port)); /* ポートは ... です */
  //int ret = connect(s, (struct sockaddr *)&addr, sizeof(addr)); /* 遂に connect *///TCPに接続するためのもの
  //if(ret == -1){perror("connect"); exit(1);}

  unsigned char data2[N];
  int i;
  int flag = 1;

  int n_send = sendto(s, "c", 1, 0, (const struct sockaddr *)&addr, sizeof(addr));
  if(n_send == -1) {perror("n_send"); exit(1);}

  
    
  struct sockaddr_in from_addr;
  socklen_t addr_size = sizeof(from_addr);


  shutdown(s, SHUT_WR);

  while(1){
    int n_recv = recvfrom(s, data2, N, 0, (struct sockaddr *)&from_addr, &addr_size);
    if(n_recv == -1){perror("n_recv"); exit(1);};
    if(n_recv == 0) break;
    for(i = 0; i < 1000; i++){
      flag = flag && (data2[i] == 1);
    }
    if (flag == 1) break;
    int n_write = write(1, data2, N);
    if(n_write == -1){perror("n_write"); exit(1);};
    if(n_write == 0) break;
  }

  return 0;
}
