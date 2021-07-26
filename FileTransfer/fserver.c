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

int main(int argc, char *argv[])
{
	if (argc<2)
	{
		error("Port number not provided");
	}
	int sockfd, newsockfd, portno;
	char buffer[100],filepath[100];
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
	 
	printf("Waiting for client\n");
	bzero(filepath,100);
	
	if(recv(newsockfd, filepath, 100,0)<0)
	{
		error("Error on receiving");
	}
	printf("File to be transferred is %s\n",filepath);
	
	FILE *fp;
	fp = fopen(filepath,"r");
	if(fp==NULL)
		error("File not found");
	
	while(fgets(buffer,sizeof(buffer),fp)!=NULL)
	{
		if(send(newsockfd,buffer,100,0)<0)
		{
			error("Error on sending");
		}
	}
	printf("File Transferred\n");

	fclose(fp);
	close(newsockfd);
	close(sockfd);
	return 0;
	
}
