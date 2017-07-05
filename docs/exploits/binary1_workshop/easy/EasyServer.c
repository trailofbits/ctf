#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


#define PORTNO 12346

int h=0,b=0,p=0;

#define BUFFER_SIZE 512

int checksec(FILE* f){
  FILE* key;
  char buf[1024];
  
  if(h&b&p){
    key=fopen("easy_key","r");
    fread(buf,1024,1,key);
    fprintf(f,"%s",buf);
    fclose(key);
    return 0;
  }
  return 1;
}


void hekers(FILE* f){
  volatile int zeroWeekend;  
  char buf[32];

  fprintf(f,"So you want to be an 31337 Hax0r?\n");
  fgets(buf,40,f);
  switch(strcmp("y3$\n",buf)){
  case 0:
    fprintf(f,"First you must get power\n");
    break;
  default:
    fprintf(f,"Well then go away\n");
    break;
  }
  if(zeroWeekend==0xcafebabe){
    h=1;    
  }
  return;
} 

void batmenss(FILE* f){
  volatile int batsignet; 
  char buf[32];

  fprintf(f,"So you want to be the batman?\n");
  fgets(buf,40,f);
  switch(strcmp("YESSSSSSS\n",buf)){
  case 0:
    fprintf(f,"First you must get rich\n");
    break;
  default:
    fprintf(f,"Well then go away\n");
    break;
  }
  
  if(batsignet==0x12345678){
    b=1;
  }
  
  return;
} 

void pokemans(FILE* f){
  volatile int pikachy;
  char buf[32];
  
  fprintf(f,"So you want to be the best there ever was?\n");
  fgets(buf,40,f);
  switch(strcmp("catchemall\n",buf)){
  case 0:
    fprintf(f,"First you must get respect\n\n");
    break;
  default:
    fprintf(f,"Well then go away\n");
    break;
  }
  
  if(pikachy==0xfa75beef){
    p=1;    
  }
  return;
} 





void readInput(int sock){
  int msg;
  char choice[4];
  char buffer[BUFFER_SIZE];
  FILE* fptr = fdopen(sock, "r+");
  char* prompt="Do you want to be a?\n"
               "1.) Pokemon Master\n"
               "2.) Elite Hacker\n"
               "3.) The Batman\n";



  while(checksec(fptr)){
    fprintf(fptr,"%s",prompt);
    fgets(choice,4,fptr);
    switch(choice[0]){
    case '1':
      pokemans(fptr);
      break;
    case '2':
      hekers(fptr);
      break;
    case '3':
      batmenss(fptr);
      break;
    default:
      fprintf(fptr,"\nThat is not one of the choices\n");
      fflush(fptr);
    }  
  }
  
  fprintf(fptr, "%s", buffer);
  fflush(fptr);
  fclose(fptr); 
  return;
}

int main(int argc, char *argv[])
{
  char buffer[BUFFER_SIZE];
  int sockfd, newsockfd, portno, pid;
  socklen_t clilen;
  struct sockaddr_in serv_addr, cli_addr;
  /*
  if (argc < 2) {
    fprintf(stderr,"ERROR, no port provided\n");
    exit(1);
  }
  */
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0){
    perror("ERROR opening socket");
    exit(1);
  }
  bzero((char *) &serv_addr, sizeof(serv_addr));
  //portno = atoi(argv[1]);
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr("0.0.0.0");
  serv_addr.sin_port = htons(PORTNO);
  if (bind(sockfd, (struct sockaddr *) &serv_addr,
	   sizeof(serv_addr)) < 0){

    perror("ERROR on binding");
    exit(1);
  }
  listen(sockfd,5);
  clilen = sizeof(cli_addr);
  while (1) {
    newsockfd = accept(sockfd,
		       (struct sockaddr *) &cli_addr, &clilen);
    if (newsockfd < 0)
      perror("ERROR on accept");
    pid = fork();
    if (pid < 0)
      perror("ERROR on fork");
    if (pid == 0)  {
      close(sockfd);
      readInput(newsockfd);
      return;
    }
    else close(newsockfd);
    waitpid(-1,NULL,WNOHANG);
  } /* end of while */
  close(sockfd);
  return 0; /* we never get here */
}
