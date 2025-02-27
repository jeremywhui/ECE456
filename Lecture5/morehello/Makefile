main: 
	gcc $(CFLAGS) server.c -o server
	gcc $(CFLAGS) client.c -o client
java: 
	javac HelloServer.java 
	javac HelloClient.java 
all: main java
clean:
	rm -f *.o server client *.class
