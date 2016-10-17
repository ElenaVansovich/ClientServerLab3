#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#define BUF_SIZE 2048

typedef struct structSocket
{
	int socket;
}struck_sock;

void cmd(int *structSock)
{
	struck_sock* st = (struck_sock*) structSock;
	int sock = st->socket;
	char buf[BUF_SIZE], buf1[BUF_SIZE];
	int bytes;

	FILE * f;

	bytes = recv(sock, buf, BUF_SIZE, 0);
	if(bytes <= 0)
	{
		perror("recv error\n");
		return;
	}

	printf("\nReceive command: %s", buf);

	f = popen(buf, "r");
	if (f == NULL)
	{
		perror("popen error\n");
		return;
	}

	fread(buf1, 1, BUF_SIZE, f);
	pclose(f);

	printf("\nSend result.\n\n");
	send(sock, buf1, bytes, 0);
	close(sock);
}

int main()
{
	int sock, listener;
	struct sockaddr_in sock_addr; 
	
	listener = socket(AF_INET, SOCK_STREAM, 0);
	if(listener < 0)
	{
		perror("socket error");
		return 1;
	}

	sock_addr.sin_family = AF_INET;
	sock_addr.sin_port = htons(3426);
	sock_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	if(bind(listener, (struct sockaddr *)&sock_addr, sizeof(sock_addr)) < 0)
	{
		perror("bind error");
		return 1;
	}

	listen(listener, 1);
	while(1)
	{
		sock = accept(listener, NULL, NULL);

		if(sock < 0)
		{
			perror("accept error");
			return 1;
		}
		#ifdef PROCESS
			pid_t id = 0;
			id = fork();
			if (id == 0)
			{
				cmd(sock);
				return 0;
			}
			else
			{
				perror("fork error");
				return 1;
			}
		#else
			pthread_t thread1;
			int result = 0;
			struck_sock st;
			st.socket = sock;
			result = pthread_create(&thread1, NULL, (void *)cmd, &st);
			if (result != 0)
			{
				perror("pthread_create error");
				return 1;
			}
		#endif
	}
	return 0;
}
