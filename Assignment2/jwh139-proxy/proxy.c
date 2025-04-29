/*
 * proxy.c - A Simple Sequential Web proxy
 *
 * Course Name: 14:332:456-Network Centric Programming
 * Assignment 2
 * Student Name: Jeremy Hui
 * 
 * IMPORTANT: Give a high level description of your code here. You
 * must also provide a header comment at the beginning of each
 * function that describes what that function does.
 */ 

#include "csapp.h"

/*
 * Function prototypes
 */
void format_log_entry(char *logstring, struct sockaddr_in *sockaddr, char *uri, int size);

/* 
 * main - Main routine for the proxy program 
 */
int main(int argc, char **argv)
{

    /* Check arguments */
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port number>\n", argv[0]);
        exit(0);
    }

    // Create file descriptors for the web proxy, client browser, and real web server
    int proxy_fd, client_fd, server_fd;
    int port = atoi(argv[1]); // Port number command line argument
    char buffer[10000] = {0};
    struct sockaddr_in proxy_address, client_address;
    int addrlen = sizeof(client_address);

    // Set up socket for server
    if ((proxy_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    memset(&proxy_address, 0, sizeof(proxy_address));
    proxy_address.sin_family = AF_INET;
    proxy_address.sin_addr.s_addr = htonl(INADDR_ANY);
    proxy_address.sin_port = htons(port);

    // Associate the socket with any IP address and the specified port
    if (bind(proxy_fd, (struct sockaddr *)&proxy_address, sizeof(proxy_address)) < 0) {
        perror("bind failed");
        close(proxy_fd);
        exit(EXIT_FAILURE);
    }

    // Prepare the socket to accept connections
    if (listen(proxy_fd, 1024) < 0) {
        perror("listen failed");
        close(proxy_fd);
        exit(EXIT_FAILURE);
    }

    while (1) {
        // Wait for connection with client browser and create new client_fd when accepted
        if ((client_fd = accept(proxy_fd, (struct sockaddr *)&client_address, (socklen_t*)&addrlen)) < 0) {
            perror("accept failed");
            close(proxy_fd);
            exit(EXIT_FAILURE);
        }
        printf("Connection established with client.\n");

        // Read request data from client_fd
        ssize_t read_bytes = read( client_fd , buffer, sizeof(buffer) - 1);
        if (read_bytes <= 0) {
            fprintf(stderr, "read request from client failure\n");
            close(client_fd);
            close(proxy_fd);
            exit(EXIT_FAILURE);
        }
        // Null-terminate
        buffer[read_bytes] = '\0';
        printf("Read request from client successfully.\n");

        // Extract the HTTP method, url, and version from the browser request
        char method[1024], url[1024], version[1024];
        sscanf(buffer, "%s %s %s", method, url, version);

        // Check that the request is a GET method
        if (strncasecmp(method, "GET", 3) != 0) {
            fprintf(stderr, "Not a GET request\n");
            close(client_fd);
            close(proxy_fd);
            exit(EXIT_FAILURE);
        }

        // Extract the host name, port number (if exists), and the resource path from the url
        char host[1024], resource[1024];
        int requested_port = 80;
        if (sscanf(url, "http://%[^:/]:%d%s", host, &requested_port, resource) != 3 &&
            sscanf(url, "http://%[^:/]%s", host, resource) != 2) {
            fprintf(stderr, "Invalid url format\n");
            close(client_fd);
            close(proxy_fd);
            exit(EXIT_FAILURE);
        }

        // Perform a DNS lookup to map a hostname and service name to a socket address
        int status;
        struct addrinfo hints;
        struct addrinfo *servinfo;
        const char *service = "http";
        memset(&hints, 0, sizeof hints); // ensure an empty
        hints.ai_family = AF_UNSPEC; // leave unspecified for either IPv4 or IPv6
        hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
        if ((status = getaddrinfo(host, service, &hints, &servinfo)) != 0) {
            fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
            close(client_fd);
            close(proxy_fd);
            exit(EXIT_FAILURE);
        }

        // Create a web server socket using the addrinfo servinfo struct
        if ((server_fd = socket(servinfo->ai_family, servinfo->ai_socktype, 0)) < 0) {
            perror("server socket failed");
            close(client_fd);
            close(proxy_fd);
            exit(EXIT_FAILURE);
        }

        // Connect the web server socket to the web server address
        if (connect(server_fd, servinfo->ai_addr, servinfo->ai_addrlen) < 0) {
            perror("connect failed");
            close(client_fd);
            close(proxy_fd);
            close(server_fd);
            exit(EXIT_FAILURE);
        }
        freeaddrinfo(servinfo); // free the linked-list

        // Rewrite the first line of the HTTP request
        char request[10000] = {0};
        sprintf(request, "GET %s %s\r\n", resource, version);
        // For the remaining part of the HTTP request, if connection is keep-alive, change to close
        char* remaining = strchr(buffer, '\n') + 1;
        int modReqLen = 0;
        char* header = strstr(remaining, "Connection: keep-alive\r\n");
        if (header != NULL) {
            modReqLen = header - remaining;
            strncat(request, remaining, modReqLen);
            strcat(request, "Connection: close\r\n");
            header += strlen("Connection: keep-alive\r\n");
        } else {
            strcpy(request, remaining);
            modReqLen = strlen(remaining);
        }
        strcat(request, header != NULL ? header : "");

        // Send the request to the real web server
        if (write(server_fd, request, strlen(request)) < 0) {
            perror("send failed");
            close(client_fd);
            close(proxy_fd);
            close(server_fd);
            exit(EXIT_FAILURE);
        }
        printf("Request sent successful.\n");

        // Get the response from the web server, and write the response to the client_fd
        char response[1024] = {0};
        ssize_t received_bytes = read(server_fd, response, sizeof(response));
        write(client_fd, response, received_bytes);
        int total = received_bytes;
        if (received_bytes == 1024) {
            // Didn't read the entire message yet
            char* cLengthLine = strstr(response, "Content-Length:");
            if (cLengthLine == NULL) {
                // Using chunked encoding
                while ((received_bytes = read(server_fd, response, sizeof(response))) > 0) {
                    total += received_bytes;
                    write(client_fd , response , received_bytes);
                }
            } else {
                // Using content length
                int headerSize = strstr(response, "\r\n\r\n") - response + 4;
                int cLength = 0;
                sscanf(cLengthLine, "Content-Length: %d", &cLength);
                int remainingLength = cLength + headerSize - 1024;
                char remainingContent[remainingLength];
                received_bytes = read(server_fd, remainingContent, remainingLength);
                write(client_fd , remainingContent, received_bytes);
                total += received_bytes;
            }
        }
        
        // Open a log file
        FILE *log_fp = fopen("logfile.txt", "a+");
        if (log_fp == NULL) {
            perror("error opening log file");
            close(client_fd);
            close(proxy_fd);
            exit(EXIT_FAILURE);
        }

        // Write the logstring to the log_fp file stream
        char* logstring = (char *)malloc(1024 * sizeof(char));
        format_log_entry(logstring, &client_address, url, total);
        fprintf(log_fp, "%s", logstring);
        fclose(log_fp);
        free(logstring);

        printf("Response sent back to client.\n");

        // Close the file descriptors and exit
        close(client_fd);
    }
    close(proxy_fd);
    exit(0);
}

/*
 * format_log_entry - Create a formatted log entry in logstring. 
 * 
 * The inputs are the socket address of the requesting client
 * (sockaddr), the URI from the request (uri), and the size in bytes
 * of the response from the server (size).
 */
void format_log_entry(char *logstring, struct sockaddr_in *sockaddr, 
		      char *uri, int size)
{
    time_t now;
    char time_str[MAXLINE];
    unsigned long host;
    unsigned char a, b, c, d;

    /* Get a formatted time string */
    now = time(NULL);
    strftime(time_str, MAXLINE, "%a %d %b %Y %H:%M:%S %Z", localtime(&now));

    /* 
     * Convert the IP address in network byte order to dotted decimal
     * form. Note that we could have used inet_ntoa, but chose not to
     * because inet_ntoa is a Class 3 thread unsafe function that
     * returns a pointer to a static variable (Ch 13, CS:APP).
     */
    host = ntohl(sockaddr->sin_addr.s_addr);
    a = host >> 24;
    b = (host >> 16) & 0xff;
    c = (host >> 8) & 0xff;
    d = host & 0xff;


    /* Return the formatted log entry string */
    sprintf(logstring, "%s: %d.%d.%d.%d %s %d\n", time_str, a, b, c, d, uri, size);
}


