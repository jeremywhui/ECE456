all: main client server
main: 
	gcc $(CFLAGS) main.c -o main
server: 
	gcc $(CFLAGS) server.c -o server
client:
	gcc $(CFLAGS) client.c -o client
clean:
	rm -f *.o main client server
