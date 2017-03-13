#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>  
#include <sys/socket.h> 
#include <arpa/inet.h>  
#include <netinet/in.h> 
#include <unistd.h>     
#include <dirent.h>     
#include <sys/stat.h>   

#define MAX_SIZE 2048

int main (int argc , char **argv) {
  int cli_fd;                   
  struct sockaddr_in svr_addr;  

  int read_bytes;               
  char buf[MAX_SIZE];           

  cli_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (cli_fd < 0) {
    perror("Create socket failed.");
    exit(1);
  }

  bzero(&svr_addr, sizeof(svr_addr));
  svr_addr.sin_family = AF_INET;
  svr_addr.sin_port = htons(atoi(argv[2]));
  if (inet_pton(AF_INET, argv[1], &svr_addr.sin_addr) <= 0) {
     perror("Address converting fail with wrong address argument");
     return 0;
  }
  if (connect(cli_fd, (struct sockaddr *)&svr_addr, sizeof(svr_addr)) < 0) {
    perror("Connect failed");
    exit(1);
  }

  read_bytes = read(cli_fd, buf, sizeof(buf));
  if (read_bytes < 0) {
    perror("Read failed");
    exit(1);
  }
  buf[read_bytes] = '\0';
  printf("%s\n", buf);
  close(cli_fd);

  return 0;
}
