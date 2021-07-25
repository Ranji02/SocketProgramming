#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>

void error(const char *msg)
{
	perror(msg);
	exit(0);
}

//argc - total number of parameters
//argv - parameters
//argv[0] - file name


int main(int argc, char *argv[])
{
	if (argc<2)
	{
		printf("Port number not provided\n");
		exit(0);
	}
	int sockfd, newsockfd, portno;
	char buffer[500];
	struct sockaddr_in serv_addr, cli_addr;
	
	// socklen_t is sys/socket.h data type 32 bit data type
	socklen_t clilen;
	
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd<0)
	{
		error("Error opening Socket");
	}
	bzero((char*) &serv_addr,sizeof(serv_addr));
	portno=atoi(argv[1]);
	
	/* Internet address family */
	serv_addr.sin_family = AF_INET;
	
	/* Any incoming interface */
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	
	/* Local port */
	serv_addr.sin_port = htons(portno);
	
	if(bind(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr))<0)
		error("Binding Failed");
	
	listen(sockfd, 1);
	clilen = sizeof(cli_addr);
	
	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	if(newsockfd < 0)
		error("Error on Accept");
	 
	bzero(buffer,500);
	int n = recv(newsockfd, buffer, 500,0);
	if(n<0)
	{
		error("Error on reading");
		exit(0);
	}
		
	printf("Message from Client: %s\n\n",buffer);
	printf("Message sent: %s\n",buffer);
	n = send(newsockfd,buffer,strlen(buffer),0);
	if(n<0)
	{
		error("Error on writing\n");
		exit(0);
	}
		
	close(newsockfd);
	close(sockfd);
	return 0;
	
}