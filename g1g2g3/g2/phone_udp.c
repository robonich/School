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
#include <string.h>


#define N 1000

int main(int argc, char *argv[]){

  int s;
  struct sockaddr_in serv_addr, client_addr;
  unsigned char data[N];
    
  struct sockaddr_in from_addr;
  socklen_t from_addr_len;
  
    
  if(argc == 2){
    /*time_t t1;  
    time(&t1);
    */
    s = socket(PF_INET, SOCK_DGRAM, 0);//UDPならSOCK_DGRAM
    if(s == -1){perror("socket"); exit(1);}
    
    char *port = argv[1];

    serv_addr.sin_family = PF_INET; /* これは IPv4 のアドレスです */
    serv_addr.sin_port = htons(atoi(port)); /* ポートは ... です */
    serv_addr.sin_addr.s_addr = INADDR_ANY; /*どのIPアドレスでも待ち受けしたいです*/
    bind(s, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

     while(1){
    
    from_addr_len = sizeof(from_addr);

    int n_recv = recvfrom(s , data, N, 0, (struct sockaddr *)&from_addr, &from_addr_len);//recvfromとかもあるよ
    if(n_recv == -1){perror("n_recv"); exit(1);};
    //if(n_recv == 0) break;
    
    int n_write = write(1, data, n_recv);//sendtoとかもあるよ
    if(n_write == -1){perror("n_send"); exit(1);}
    //if(n_write == 0) break;
    
    int n_read = read(0, data, N);
    if(n_read == -1) {perror("n_read"); exit(1);}
    //if(n_read == 0) break;
    
    //(struct sockaddr *)&to, sizeof(to)
    int n_send = sendto(s, data, n_read, 0, (const struct sockaddr *)&from_addr/*(argc==2) ? (const struct sockaddr *)&serv_addr : (const struct sockaddr *)&client_addr*/, from_addr_len/*(argc==2) ? sizeof(serv_addr) : sizeof(client_addr)*/);//s
    if(n_send == -1) {perror("n_send"); exit(1);}
    //if(n_send == 0) break;

  }
    //int l = listen(ss, 10);
    
    // socklen_t client_addr_len = sizeof(client_addr);
    
    //s = accept(ss, (struct sockaddr *)&client_addr, &client_addr_len);
    //if(s == -1){perror("accept"); exit(1);}
    
    /*unsigned char data2[N];

    time_t t2;
    time(&t2);
    int timers = difftime(t2, t1) + 2;
    int bytes = timers*44100*2;
 
    while(bytes > 0){
      int n_read2 = read(0, data2, N);
      if(n_read2 == -1) {perror("n_read2"); exit(1);}
      if(n_read2 == 0) break;
      bytes -= N;
    }*/
  }
  else if(argc == 3){
    s = socket(PF_INET, SOCK_DGRAM, 0);//UDPならSOCK_DGRAM
    if(s == -1){perror("socket"); exit(1);}

    char *ip = argv[1];
    char *port = argv[2];

    client_addr.sin_family = PF_INET; /* これは IPv4 のアドレスです */
    //client_addr.sin_addr.s_addr = inet_addr(ip); /* IP アドレスは ... です */
    int inet = inet_aton(ip, &client_addr.sin_addr);//上の強いバージョン
    if(inet == 0){perror("inet_aton"); exit(1);}
    client_addr.sin_port = htons(atoi(port)); /* ポートは ... です */
    bind(s, (struct sockaddr *)&client_addr, sizeof(client_addr));

    //int send_to2 = sendto(s, "start", 6, 0, (const struct sockaddr *)&from_addr, sizeof(from_addr));

    memcpy(&from_addr, &client_addr, sizeof(client_addr));

    while(1){
      
       from_addr_len = sizeof(from_addr);

              
       int n_read = read(0, data, N);
       if(n_read == -1) {perror("n_read"); exit(1);}
       //if(n_read == 0) break;
       
       //(struct sockaddr *)&to, sizeof(to)
       int n_send = sendto(s, data, n_read, 0, (const struct sockaddr *)&from_addr/*(argc==2) ? (const struct sockaddr *)&serv_addr : (const struct sockaddr *)&client_addr*/, from_addr_len/*(argc==2) ? sizeof(serv_addr) : sizeof(client_addr)*/);//s
       if(n_send == -1) {perror("n_send"); exit(1);}
       //if(n_send == 0) break;
       
       int n_recv = recvfrom(s , data, N, 0, (struct sockaddr *)&from_addr, &from_addr_len);//recvfromとかもあるよ
       if(n_recv == -1){perror("n_recv"); exit(1);};
       //if(n_recv == 0) break;
       
       int n_write = write(1, data, n_recv);//sendtoとかもあるよ
       if(n_write == -1){perror("n_send"); exit(1);}
       //if(n_write == 0) break;
       
    }
  }
  
  shutdown(s, SHUT_WR);

  return 0;
}
