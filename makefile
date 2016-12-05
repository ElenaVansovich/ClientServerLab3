client:
	gcc fsclient.c -o client
server_thread:
	gcc -pthread fsserver.c -o server
server_process:
	gcc fsserver.c -DPROCESS -o server
clean:
	rm -f server client
