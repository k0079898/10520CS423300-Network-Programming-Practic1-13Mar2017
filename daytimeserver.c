#include <stdio.h>
#include <stdlib.h>
#include <string.h>     
#include <sys/types.h>  
#include <sys/socket.h> 
#include <arpa/inet.h>  
#include <netinet/in.h> 
#include <unistd.h>     
#include <time.h>       

#define PORT 8765
#define MAX_SIZE 2048
#define MAX_CONNECTION 5


int main() {

  int svr_fd;                   
  struct sockaddr_in svr_addr;  

  int cli_fd;                   
  struct sockaddr_in cli_addr;  
  socklen_t addr_len;           

  time_t ticks;                 
  int write_bytes;              
  char buf[MAX_SIZE];           

  int read_bytes;               

  svr_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (svr_fd < 0) {
    perror("Create socket failed.");
    exit(1);
  }

  bzero(&svr_addr, sizeof(svr_addr));
  svr_addr.sin_family = AF_INET;
  svr_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  svr_addr.sin_port = htons(PORT);

  if (bind(svr_fd, (struct sockaddr*)&svr_addr , sizeof(svr_addr)) < 0) {
    perror("Bind socket failed.");
    exit(1);
  }

  if (listen(svr_fd, MAX_CONNECTION) < 0) {
    perror("Listen socket failed.");
    exit(1);
  }


  printf("Waiting for new connection...\n");

  addr_len = sizeof(struct sockaddr_in);

  while(1) {
    cli_fd = accept(svr_fd, (struct sockaddr*)&cli_addr, (socklen_t*)&addr_len);

    if (cli_fd < 0) {
      perror("Accept failed");
      exit(1);
    }

    printf("Connection from %s, port %d\n", inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));

    ticks = time(NULL);
    snprintf(buf, sizeof(buf), "%.24s\r\n", ctime(&ticks));

    write_bytes = write(cli_fd, buf, strlen(buf));
    if(write_bytes < 0) {
      perror("Write Failed");
      exit(1);
    }

    close(cli_fd);
  }

  close(svr_fd);
  return 0;
}
