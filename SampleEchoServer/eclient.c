#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>

void error(const char *msg)
{
	perror(msg);
	exit(0);
}

int main(int argc, char *argv[])
{
	int sockfd, portno,n;
	struct sockaddr_in serv_addr;
	
	//store information about a given host, such as host name, IPv4 address, and so forth.
	struct hostent *server;
	
	char buffer[500];
	if (argc<3)
	{
		fprintf(stderr,"usage: %s hostname port\n",argv[0]);
		exit(0);
	}
	
	portno = atoi(argv[2]);
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd<0)
		error("Error opening socket");
	
	//gethostbyname function retrieves host information 
	server = gethostbyname(argv[1]);
	if(server == NULL)
	{
		fprintf(stderr,"Error, NO such host");
	}
		
	//void bzero(void *s, size_t n);
	//The bzero() function erases the data in the n bytes of the memory starting at the 
	//location pointed to by s, by writing zeros (bytes containing '\0') to that area.
	bzero((char*) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	
	//void bcopy(const void *src, void *dest, size_t n);
	//bcopy() function copies n bytes from src to dest
	bcopy((char*) server->h_addr, (char*) &serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(portno);
	if(connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) <0)
		error("Connection failed\n");

	bzero(buffer, 500);
	printf("Client: ");
	//fgets(buffer, 255, stdin);
	
	//press ~ to end the paragraph
	scanf("%[^~]s",buffer);
	
	n = send(sockfd,buffer,strlen(buffer),0);
	if(n<0)
		error("Error on writing\n");
	
	n = recv(sockfd, buffer, 500,0);
	if(n<0)
		error("Error on reading");
	printf("\nMessage from Server: %s\n",buffer);
	close(sockfd);
	return 0;
}