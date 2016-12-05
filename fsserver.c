#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define BUF_SIZE 2048

void cmd(void* param)
{
	int sock = (intptr_t)param;
	char buf[BUF_SIZE], buf1[BUF_SIZE];
	int bytes;

	FILE * f;

	bytes = recv(sock, buf, BUF_SIZE, 0);
	if (bytes <= 0) {
		perror("recv error\n");
		return;
	}

	printf("\nReceive command: %s", buf);

	f = popen(buf, "r");
	if (f == NULL) {
		perror("popen error\n");
		return;
	}
	int len = -1;
	while ((len = fread(buf1, 1, BUF_SIZE, f)) > 0) {
		
	}

	pclose(f);

	printf("\nSend result.\n\n");
	send(sock, buf1, bytes, 0);
}

int main()
{
	int sock, listener;
	struct sockaddr_in sock_addr; 
	
	listener = socket(AF_INET, SOCK_STREAM, 0);
	if (listener < 0) {
		perror("socket error");
		exit(1);
	}

	sock_addr.sin_family = AF_INET;
	sock_addr.sin_port = htons(3425);
	sock_addr.sin_addr.s_addr = inet_addr("0.0.0.0");

	if (bind(listener, (struct sockaddr *)&sock_addr, sizeof(sock_addr)) < 0) {
		perror("bind error");
		exit(2);
	}

	listen(listener, 5);
	while(1) {
		sock = accept(listener, NULL, NULL);

		if (sock < 0) {
			perror("accept error");
			return 1;
		}
		#ifdef PROCESS
			pid_t id = 0;
			id = fork();
			if (id == -1) {
				perror("fork error");
				exit(1);
			}
			else if (id == 0) {
				cmd((void *)(intptr_t)sock);
				return 0;
			}
		#else
			pthread_t thread1;
			int result = pthread_create(&thread1, NULL, (void *)cmd, (void *)(intptr_t)sock);
			if (result != 0) {
				perror("pthread_create error");
			}
		#endif
	}
	return 0;
}
