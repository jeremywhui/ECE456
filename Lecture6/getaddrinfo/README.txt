This directory contains 3 source files:
main.c - show how to use getaddrinfo to convert domain-name and service/port to address structure used by socket.

server.c - example server using getaddrinfo. Usage: ./server [port/service]
           If no argument, default port is http -> 80 which require root privilege
client.c - example client using getaddrinfo. Usage: ./client domain/IP [port/service]
           If server specifies port/service argument, client must also specifies 
           the same argument.

How to build: make all
