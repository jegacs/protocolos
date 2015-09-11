all: 
	gcc server.c -o server -g
	gcc client.c -o client -g
clean:
	rm server.c
	rm client.c
	rm client
	rm server
