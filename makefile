client:
	gcc fsclient.c -o client
server-thread:
	gcc -pthread fsserver.c -o server
server-process:
	gcc fsserver.c -DPROCESS -o server
clean:
	rm -f server client
