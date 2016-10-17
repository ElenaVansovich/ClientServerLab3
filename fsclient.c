#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>

#define BUF_SIZE 2048
 
int main ()
{
	int sock, connection, bytes = 0;      
	struct sockaddr_in sock_addr;

	char cmd[BUF_SIZE], buf[BUF_SIZE];

    	sock = socket(AF_INET, SOCK_STREAM, 0);

	if(sock < 0)
	{
		perror("socket error\n");
		exit(1);
	}

	sock_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    	sock_addr.sin_family = AF_INET;
    	sock_addr.sin_port = htons(3426);
    
	connection = connect(sock, (struct sockaddr *)&sock_addr, sizeof(sock_addr));

   	if(connection < 0)
	{
		perror("connect error\n");
		exit(2);
	}

	strcpy(cmd, "echo Hello");
	printf("Command: echo Hello\n");
	send(sock, cmd, BUF_SIZE, 0);

	bytes = recv(sock, buf, BUF_SIZE, 0);
	if(bytes < 0)
	{
		perror("recv error\n");
		exit(2);
	}

	printf("Result: %s\n", buf);
	return 0;
}
