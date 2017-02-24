#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>

int main(){
	int sfd;
	
	struct sockaddr_in addr;
	
	sfd = socket(PF_INET, SOCK_STREAM, 0);
	if(sfd == -1){
		error("Error with SFD");
	}
	
//50014 port
	memset(&addr.sin_zero, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(50014);
	addr.sin_addr.s_addr = INADDR_ANY; // automatically find IP

	//bind(sfd, (struct sockaddr *)&addr, sizeof(addr));
	if(bind(sfd, (struct sockaddr *) &addr, sizeof(addr)) < 0) 
              error("ERROR on binding");

	listen(sfd,10);
	
	int connfd = accept(sfd, NULL, NULL);
	
	char buffer[1024];
	//strcpy(buffer, "Hello there!");
	//send (connfd, buffer, strlen(buffer), 0);

	char buffer2[1024];

	strcpy(buffer2, "Type hi to greet me");

	send(connfd, buffer2, strlen(buffer2), 0);

	strcpy(buffer2, "Hi!");

	//recv(connfd, buffer,1024,0);
	

	char key[] = "hi";
	
	recv(connfd, buffer, 2, 0);

	//while(1){
		
		
		//if(strcmp(key, buffer) == 0){
			//send(connfd, buffer2, strlen(buffer2), 0);
			
			//break;
		//}
		//else{
			//break;
		//}

		//recv(connfd, buffer,2,0);
			
		
	//}

	while(strcmp(key, buffer) == 0){
		send(connfd, buffer2, strlen(buffer2), 0);
		recv(connfd, buffer,2,0);
	}

	strcpy(buffer2, "bye! (if you did not see me respond Hi! you are a bad greeter)");
	send(connfd, buffer2, strlen(buffer2), 0);
	close(connfd);
	close(sfd);

	return 0;
}