#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>       //for linux
#include <arpa/inet.h>    //for linux
#include <sys/socket.h>

void error_handling(char* msg)  
{
  fputs(msg,stderr); 
  fputc('\n', stderr);
  exit(1); 
}

int main(int argc, char* argv[])
{
  int sock, fd,n; 
  struct sockaddr_in serv_addr; 
  char buf[1024];
  if(argc != 3) 
  {
    fprintf(stderr,"%s <IP> <PORT>\n", argv[0]);
    exit(1); 
  }
  sock = socket(PF_INET, SOCK_STREAM, 0);
  if(sock == -1)
    error_handling("socket() error");
    
  memset(&serv_addr,0,sizeof(serv_addr));
  serv_addr.sin_family =  AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
  serv_addr.sin_port = htons(atoi(argv[2]));
  
  if(connect(sock,(struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
    error_handling("connect() error");
   
  char filename[100];
  printf("Input a file name:\n");
  scanf("%s", filename);
  write(sock, filename, sizeof(filename));
  
  int filesize;
  n = read(sock, &filesize, sizeof(filesize));
  if(n == -1)
    error_handling("read() error");
  printf("Receiving %d bytes of a file\n", filesize);
    
  fd = open("received_script.txt", O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);
  if(fd == -1)
    error_handling("open() error");
  
  while((n = read(sock, buf, sizeof(buf))) > 0) // n = 0이면 EOF(파일의 끝)
  {
    if(write(fd, buf, n) != n)
      error_handling("write() error");
  }  
  printf("Receiving completed\n"); 
  
  return 0;
}
