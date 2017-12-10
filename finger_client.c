// a subset of finger utility in C.
// uses concept of 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
// This program uses the concept of Remote Procedure Call.
// it executes a program at Server on call from client. 
// run using 
/*
	gcc finger_server.c -o finger_server
	gcc finger_client.c -o finger_client
	./finger_server 8080
	./finger_client ip_address 8080

	then enter a command (shell command) in client 
	   which will be sent to server and executed there!
	


*/

char buffer[256];

// function to print error and exit
void error(const char *msg)
{
	printf("%s\n", msg);
	exit(0);
}

void finger(int sockfd,char* command)
{
	printf("Entered client stub ...\n");
	strcpy(buffer, command);
	printf("Executing Remote Finger command ... \n");
	write(sockfd, buffer, strlen(buffer));
	while(1)
	{
		bzero(buffer,256);	
		read(sockfd,buffer,255);
		if(buffer[0]=='@')
			break;
		printf("%s",buffer);		
	}
	
}

int main(int argc,char* argv[])
{
	int sockfd,portno,n,valid=1;
	struct sockaddr_in serv_addr;
	char ky;
	char command[50];
	
	if(argc<3)
	{
		fprintf(stderr, "usage %s hostname port \n", argv[0]);
		exit(0);
	}
	
	portno = atoi(argv[2]);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
		error("ERROR opening socket");
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(portno);
	
	if(connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr))<0)
		error("ERROR connecting");
	int option = -1;

	printf("\n1. Run finger utility\n2. RPC call\n\tEnter option : ");
	scanf("%d", &option);
	if(option == 1){
		printf("Calling Local Finger Command ... \n");
		finger(sockfd, "who");
	}
	else{
		printf("Enter a command\n");
		scanf("%s", command);
		finger(sockfd, command);
	}
	printf("Executed successfully!!\n");
	close(sockfd);
	return 0;
}