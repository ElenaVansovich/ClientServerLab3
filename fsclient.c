#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>

#define BUF_SIZE 2048
 
int main (int argc, char* argv[]) {
	int sock, connection, bytes = 0;      
	struct sockaddr_in sock_addr;

	char cmd[BUF_SIZE], buf[BUF_SIZE];

	char addr[BUF_SIZE];

    	sock = socket(AF_INET, SOCK_STREAM, 0);

	if (sock < 0) {
		perror("socket error\n");
		exit(1);
	}
	
	if (argc > 0) {
		strcpy(addr, argv[1]); 
	}
	else {
		perror("arguments error\n");
	}

	sock_addr.sin_addr.s_addr = inet_addr(addr);
    	sock_addr.sin_family = AF_INET;
    	sock_addr.sin_port = htons(3425);
    
	connection = connect(sock, (struct sockaddr *)&sock_addr, sizeof(sock_addr));

   	if (connection < 0) {
		perror("connect error\n");
		exit(2);
	}
	

	printf("Input text:");
	fgets(cmd, 2048, stdin);
	send(sock, cmd, BUF_SIZE, 0);

	bytes = recv(sock, buf, BUF_SIZE, 0);
	if (bytes < 0) {
		perror("recv error\n");
		exit(2);
	}

	printf("Result: %s\n", buf);
	return 0;
}
