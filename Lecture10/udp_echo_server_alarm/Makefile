main: signal sigaction
signal:
	gcc $(CFLAGS) server.c -o server
	gcc $(CFLAGS) client.c -o client
	gcc $(CFLAGS) client1.c -o client1
sigaction:
	gcc $(CFLAGS) server_act.c -o server_act
	gcc $(CFLAGS) client_act.c -o client_act

clean:
	rm -f *.o server client client1 server_act client_act
