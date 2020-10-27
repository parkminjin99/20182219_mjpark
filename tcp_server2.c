#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>       //for linux
#include <arpa/inet.h>    //for linux
#include <sys/socket.h>

void error_handling(char* msg) // error_handling 함수 
{
  fputs(msg,stderr); 
  fputc('\n', stderr);
  exit(1); 
}

int main(int argc, char* argv[])
{
  int serv_sock, clnt_sock, fd, n;
  struct sockaddr_in serv_addr;   
  struct sockaddr_in clnt_addr;   
  socklen_t clnt_addr_size;   
  char buf[1024];   
  if(argc != 2)         
  {
    fprintf(stderr, "Usage: %s <port>\n", argv[0]); 
    exit(1);
  }
  serv_sock = socket(PF_INET, SOCK_STREAM, 0);
  if(serv_sock == -1) // 소켓 생성 실패하는 경우 
    error_handling("socket() error");
  
  memset(&serv_addr,0,sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;   
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(atoi(argv[1]));
  if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
    error_handling("bind() error"); 
  
  if(listen(serv_sock, 5) == -1) 
    error_handling("listen() error");
  
  clnt_addr_size = sizeof(clnt_addr);
  clnt_sock = accept(serv_sock,(struct sockaddr*)&clnt_addr, &clnt_addr_size); 
  if(clnt_sock == -1)
    error_handling("accept() error");
  int clnt_addr_ip = ntohl(clnt_addr.sin_addr.s_addr);
  int clnt_addr_port = ntohs(clnt_addr.sin_port);
  printf("[%d.", (clnt_addr_ip>>24)&0xff);
  printf("%d.", (clnt_addr_ip>>16)&0xff);
  printf("%d.", (clnt_addr_ip>>8)&0xff);
  printf("%d:", (clnt_addr_ip)&0xff);
  printf("%d] connected\n", clnt_addr_port);
  
  char filename[100];
  n = read(clnt_sock, &filename, sizeof(filename));
  if(n == -1)
    error_handling("read() error");
  fd = open(filename, O_RDONLY, S_IRWXU);
  if(fd == -1)
    error_handling("open() error");
  int filesize = lseek(fd, (off_t)0, SEEK_END); // 파일 크기 
  lseek(fd,(off_t)0,SEEK_SET);
  printf("Sending %d bytes of a file\n", filesize);
  write(clnt_sock, &filesize, sizeof(int)); // 파일 크기 전송 

  while((n = read(fd, buf, sizeof(buf))) > 0) 
  {
    for (int i = 0; i < n; i++) 
    {
      if(buf[i] >= 'a' && buf[i] <= 'z')
        buf[i] = buf[i] -'a' + 'A';
      else if(buf[i] >= 'A' && buf[i] <= 'Z')
        buf[i] = buf[i] - 'A' + 'a';
    }
    if(write(clnt_sock, buf, n) != n)
      error_handling("write() error");
  } 
  printf("Sending completed\n");
  
  close(clnt_sock); 
  close(serv_sock);
  return 0;
} 
  
  
  
  
