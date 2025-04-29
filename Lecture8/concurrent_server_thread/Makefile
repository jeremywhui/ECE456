main: 
	gcc $(CFLAGS) server.c -o server -pthread
	gcc $(CFLAGS) client.c -o client
	gcc $(CFLAGS) client_thread.c -o client_thread -pthread
clean:
	rm -f *.o server client client_thread
