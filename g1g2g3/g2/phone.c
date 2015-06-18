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


#define N 1000

int main(int argc, char *argv[]){

  int s, ss;
  struct sockaddr_in serv_addr, client_addr;

  if(argc == 2){
    time_t t1;  
    time(&t1);
    
    ss = socket(PF_INET, SOCK_STREAM, 0);//UDPならSOCK_DGRAM
    if(ss == -1){perror("socket"); exit(1);}
    
    char *port = argv[1];

    serv_addr.sin_family = AF_INET; /* これは IPv4 のアドレスです */
    serv_addr.sin_port = htons(atoi(port)); /* ポートは ... です */
    serv_addr.sin_addr.s_addr = INADDR_ANY; /*どのIPアドレスでも待ち受けしたいです*/
    bind(ss, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    int l = listen(ss, 10);
  
    
    socklen_t len = sizeof(struct sockaddr_in);
    printf("t1\n");
    s = accept(ss, (struct sockaddr *)&client_addr, &len);
    if(s == -1){perror("accept"); exit(1);}
    printf("t2\n");
    
    unsigned char data2[N];

    time_t t2;
    time(&t2);
    int timers = difftime(t2, t1) + 2;
    printf("%d\n", timers);
    int bytes = timers*44100*2;
 
    while(bytes > 0){
      int n_read2 = read(0, data2, N);
      if(n_read2 == -1) {perror("n_read2"); exit(1);}
      bytes -= N;
    }
  }
  else if(argc == 3){
    s = socket(PF_INET, SOCK_STREAM, 0);//UDPならSOCK_DGRAM
    if(s == -1){perror("socket"); exit(1);}

    char *ip = argv[1];
    char *port = argv[2];

    client_addr.sin_family = AF_INET; /* これは IPv4 のアドレスです */
    //client_addr.sin_addr.s_addr = inet_addr(ip); /* IP アドレスは ... です */
    int inet = inet_aton(ip, &client_addr.sin_addr);//上の強いバージョン
    if(inet == 0){perror("inet_aton"); exit(1);}
    client_addr.sin_port = htons(atoi(port)); /* ポートは ... です */
    int ret = connect(s, (struct sockaddr *)&client_addr, sizeof(client_addr)); /* 遂に connect */
    if(ret == -1){perror("connect"); exit(1);}
  }
  
  unsigned char data[N];

  while(1){
    int n_read = read(0, data, N);
    if(n_read == -1) {perror("n_read"); exit(1);}
    if(n_read == 0) break;
    int n_send = sendto(s, data, n_read, 0, (const struct sockaddr *)&client_addr, sizeof(client_addr));//s
    if(n_send == -1) {perror("n_send"); exit(1);}
    if(n_send == 0) break;

    int n_recv = recv(s, data, N, 0);//recvfromとかもあるよ
    if(n_recv == -1){perror("n_recv"); exit(1);};
    if(n_recv == 0) break;
    
    int n_write = write(1, data, n_recv);//sendtoとかもあるよ
    if(n_write == -1){perror("n_send"); exit(1);}
    if(n_write == 0) break;
  }

  if(argc == 2) shutdown(ss, SHUT_WR);
  else if(argc == 3) shutdown(s, SHUT_WR);

  return 0;
}
