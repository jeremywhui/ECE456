/*
 * proxy.c - A Simple Concurrent Web proxy
 *
 * Course Name: 14:332:456-Network Centric Programming
 * Assignment 3
 * Student Name: Jeremy Hui
 * 
 * IMPORTANT: Give a high level description of your code here. You
 * must also provide a header comment at the beginning of each
 * function that describes what that function does.
 */ 

#include "csapp.h"
#include <getopt.h>
#include <poll.h>
#include <linux/limits.h>

const char * program_name; // Used in print_usage

// Clientinfo structure to be passed into thread_handle
typedef struct clientinfo {
    int clifd;
    struct sockaddr_in cliaddr;
} clinfo;

pthread_mutex_t log_mutex = PTHREAD_MUTEX_INITIALIZER;

/*
 * Function prototypes
 */
void format_log_entry(char *logstring, struct sockaddr_in *sockaddr, char *uri, int size);
void format_connect_entry(char *logstring, struct sockaddr_in *sockaddr, char *uri, int sent, int recv);

void *thread_handle(void *vargp) {
    clinfo client = *((clinfo *)vargp);
    int client_fd = client.clifd;
    struct sockaddr_in client_address = client.cliaddr;
    char buffer[10000] = {0};
    // Read request data from client_fd
    ssize_t read_bytes = read( client_fd , buffer, sizeof(buffer) - 1);
    if (read_bytes <= 0) {
        fprintf(stderr, "Read request from client failure\n");
        close(client_fd);
        return NULL;
    }
    // Null-terminate
    buffer[read_bytes] = '\0';
    printf("Read request from client successfully.\n");
    printf("%s\n", buffer);

    // Extract the HTTP method, url, and version from the browser request
    char method[1024], url[1024], version[1024];
    sscanf(buffer, "%s %s %s", method, url, version);

    // Check that the request is a GET or CONNECT method
    if (strncasecmp(method, "GET", 3) == 0) {
        // Extract the host name, port number (if exists), and the resource path from the url
        char host[1024], resource[1024];
        int requested_port = 80;
        if (sscanf(url, "http://%[^:/]:%d%s", host, &requested_port, resource) != 3 &&
            sscanf(url, "http://%[^:/]%s", host, resource) != 2) {
            fprintf(stderr, "Invalid url format\n");
            close(client_fd);
            return NULL;
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
            return NULL;
        }

        int server_fd;
        // Create a web server socket using the addrinfo servinfo struct
        if ((server_fd = socket(servinfo->ai_family, servinfo->ai_socktype, 0)) < 0) {
            perror("server socket failed");
            close(client_fd);
            return NULL;
        }

        // Connect the web server socket to the web server address
        if (connect(server_fd, servinfo->ai_addr, servinfo->ai_addrlen) < 0) {
            perror("connect failed");
            close(client_fd);
            close(server_fd);
            return NULL;
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
            close(server_fd);
            return NULL;
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
        FILE *log_fp = fopen("proxy.log", "a+");
        if (log_fp == NULL) {
            perror("error opening log file");
            close(client_fd);
            return NULL;
        }
        // Lock printing to the log
        pthread_mutex_lock(&log_mutex);
        // Write the logstring to the log_fp file stream
        char* logstring = (char *)malloc(1024 * sizeof(char));
        format_log_entry(logstring, &client_address, url, total);
        fprintf(log_fp, "%s", logstring);
        free(logstring);
        // Unlock printing to the log
        pthread_mutex_unlock(&log_mutex);
        fclose(log_fp);

        printf("Response sent back to client.\n");   
        // Close the file descriptors and exit
        close(server_fd);
        close(client_fd);
    } else if (strncasecmp(method, "CONNECT", 7) == 0) {
        // Extract the host name, port number (if exists), and the resource path from the url
        char host[1024];
        int requested_port = 80;
        if (sscanf(url, "%[^:/]:%d", host, &requested_port) != 2 &&
            sscanf(url, "%[^:/]", host) != 1) {
            fprintf(stderr, "Invalid url format\n");
            close(client_fd);
            return NULL;
        }

        // Perform a DNS lookup to map a hostname and service name to a socket address
        int status;
        struct addrinfo hints;
        struct addrinfo *servinfo;
        const char *service = "https";
        memset(&hints, 0, sizeof hints); // ensure an empty hints
        hints.ai_family = AF_UNSPEC; // leave unspecified for either IPv4 or IPv6
        hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
        if ((status = getaddrinfo(host, service, &hints, &servinfo)) != 0) {
            fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
            close(client_fd);
            return NULL;
        }

        int server_fd;
        // Create a web server socket using the addrinfo servinfo struct
        if ((server_fd = socket(servinfo->ai_family, servinfo->ai_socktype, 0)) < 0) {
            perror("server socket failed");
            close(client_fd);
            return NULL;
        }

        // Connect the web server socket to the web server address
        if (connect(server_fd, servinfo->ai_addr, servinfo->ai_addrlen) < 0) {
            perror("connect failed");
            close(client_fd);
            close(server_fd);
            return NULL;
        }
        freeaddrinfo(servinfo); // free the linked-list

        // Get the response from the web server, and write the response to the client_fd
        char* response = "HTTP/1.1 200 Connection Established\r\n\r\n";
        write(client_fd, response, strlen(response));
        printf("Response sent back to client.\n");
        long total[2] = {0};
        struct pollfd fds[2];
        fds[0].fd = client_fd;
        fds[0].events = POLLIN;
        fds[1].fd = server_fd;
        fds[1].events = POLLIN;
        while (1) {
            char buffer[1024] = {0};
            ssize_t sent_bytes = 0;
            int nfds = 2, timeout = 5000;
            int ret = poll(fds, nfds, timeout); // wait for up to 5 seconds
            if (ret == -1) {
                perror("poll");
                close(client_fd);
                close(server_fd);
                return NULL;
            } else if (ret == 0) {
                printf("Connection timed out, closing connection\n");
                break;
            } else {
                int exitCondition = 0;
                for (int i = 0; i < nfds; i++) {
                    if (fds[i].revents & POLL_HUP) {
                        printf("%s: hung up.\n", i ? "Server" : "Client");
                        exitCondition = 1;
                        break;
                    }
                    if (fds[i].revents & POLLERR) {
                        printf("%s: hung up.\n", i ? "Server" : "Client");
                        close(server_fd);
                        close(client_fd);
                        return NULL;
                    }
                    if (fds[i].revents & POLLIN) {
                        if ((sent_bytes = read(fds[i].fd, buffer, sizeof(buffer))) > 0) {
                            total[i] += sent_bytes;
                            write(fds[1 - i].fd, buffer, sent_bytes);
                        }
                    }
                }
                if (exitCondition) break;
            }
        }
        
        // Open a log file
        FILE *log_fp = fopen("proxy.log", "a+");
        if (log_fp == NULL) {
            perror("error opening log file");
            close(client_fd);
            return NULL;
        }
        // Lock printing to the log
        pthread_mutex_lock(&log_mutex);
        // Write the logstring to the log_fp file stream
        char* logstring = (char *)malloc(1024 * sizeof(char));
        format_connect_entry(logstring, &client_address, url, total[0], total[1]);
        fprintf(log_fp, "%s", logstring);
        free(logstring);
        // Unlock printing to the log
        pthread_mutex_unlock(&log_mutex);
        fclose(log_fp);

        close(server_fd);
        close(client_fd);
    } else {
        fprintf(stderr, "Not a GET and CONNECT request\n");
        close(client_fd);
    }
    return NULL;
}

// Displayed when help option is called or program is called incorrectly
void print_usage(FILE *stream, int exit_code) 
{
    fprintf(stream, "Usage: %s [-p | --process | -s | --systemcalls] <port number>\n", program_name);
    fprintf(stream, 
        "  -h --help            Display this usage information.\n"
        "  -s --systemcalls     Switch to the system calls implementation.\n");
    exit(exit_code);    
}

/* 
 * main - Main routine for the proxy program 
 */
int main(int argc, char **argv)
{
    int next_opt;
    int thread_flag = 0;
    // Short option string 
    const char* const short_options = "pt";
    // Long option array
    const struct option long_options[] = {
        { "process", 0, NULL, 'p'},
        { "thread",   0,  NULL, 't'},
        { NULL,     0,  NULL,  0  }     // Required at end of array
    };

    program_name = argv[0];         // Save program name for print_usage

    while(1) {		// parse options until no more left
        next_opt = getopt_long(argc, argv, short_options, long_options, NULL);
        if (next_opt < 0) {  // no more options
            break;
        }
        switch (next_opt) {
        case 'p':   // -p or --process: multiprocess implementation
            break;
        case 't': // -t or --thread: multithread implementation
            thread_flag = 1;
            break;
        case '?':   // User entered an invalid option
            print_usage(stderr, 1); // Print to stderr and exit(1)
        default:    // Something else unexpected
            abort();
        }
    }
    /* Check arguments */
    if (argc - optind < 1) {
        print_usage(stderr, 1);
    }
    printf("Using %s implementation\n", thread_flag ? "thread" : "process");

    // Create file descriptors for the web proxy, client browser, and real web server
    int proxy_fd, client_fd, server_fd;
    int port = atoi(argv[optind]); // Port number command line argument
    struct sockaddr_in proxy_address, client_address;
    int addrlen = sizeof(client_address);
    int pid;
    pthread_t tid;

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
        // Parent process checking that all children have terminated
        while(waitpid(-1, NULL, WNOHANG) > 0);
        printf("Connection established with client.\n");
        if (thread_flag) {
            clinfo client;
            client.clifd = client_fd;
            client.cliaddr = client_address;
            // Create new thread for client
            pthread_create(&tid, NULL, thread_handle, &client);
            printf("New thread created from main(): tid = %ld\n", tid);
            pthread_detach(tid);
        } else {
            char buffer[10000] = {0};
            // Create new process for client
            pid = fork();
            if (pid < 0) {
                perror("Fork failure");
                exit(EXIT_FAILURE);
            } else if (pid == 0) {
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
                printf("%s\n", buffer);

                // Extract the HTTP method, url, and version from the browser request
                char method[1024], url[1024], version[1024];
                sscanf(buffer, "%s %s %s", method, url, version);

                // Check that the request is a GET or CONNECT method
                if (strncasecmp(method, "GET", 3) == 0) {
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
                    struct flock lock;
                    int log_fd = open("proxy.log", O_WRONLY);
                    if (log_fd < 0) {
                        perror("error opening log file");
                        close(client_fd);
                        close(proxy_fd);
                        exit(EXIT_FAILURE);
                    }
                    // Lock printing to the log
                    memset(&lock, 0, sizeof(lock));
                    lock.l_type = F_WRLCK;
                    fcntl(log_fd, F_SETLKW, &lock);
                    // Write the logstring to the log_fd file stream
                    char* logstring = (char *)malloc(1024 * sizeof(char));
                    format_log_entry(logstring, &client_address, url, total);
                    // Go to end of the file
                    lseek(log_fd, 0, SEEK_END);
                    write(log_fd, logstring, strlen(logstring));
                    free(logstring);
                    // Release the lock
                    lock.l_type = F_UNLCK;
                    fcntl(log_fd, F_SETLKW, &lock);
                    close(log_fd);

                    printf("Response sent back to client.\n"); 
                } else if (strncasecmp(method, "CONNECT", 7) == 0) {
                    // Extract the host name, port number (if exists), and the resource path from the url
                    char host[1024];
                    int requested_port = 80;
                    if (sscanf(url, "%[^:/]:%d", host, &requested_port) != 2 &&
                        sscanf(url, "%[^:/]", host) != 1) {
                        fprintf(stderr, "Invalid url format\n");
                        close(client_fd);
                        close(proxy_fd);
                        exit(EXIT_FAILURE);
                    }

                    // Perform a DNS lookup to map a hostname and service name to a socket address
                    int status;
                    struct addrinfo hints;
                    struct addrinfo *servinfo;
                    const char *service = "https";
                    memset(&hints, 0, sizeof hints); // ensure an empty hints
                    hints.ai_family = AF_UNSPEC; // leave unspecified for either IPv4 or IPv6
                    hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
                    if ((status = getaddrinfo(host, service, &hints, &servinfo)) != 0) {
                        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
                        close(client_fd);
                        close(proxy_fd);
                        exit(EXIT_FAILURE);
                    }

                    int server_fd;
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

                    // Get the response from the web server, and write the response to the client_fd
                    char* response = "HTTP/1.1 200 Connection Established\r\n\r\n";
                    write(client_fd, response, strlen(response));
                    printf("Response sent back to client.\n");
                    long total[2] = {0};
                    struct pollfd fds[2];
                    fds[0].fd = client_fd;
                    fds[0].events = POLLIN;
                    fds[1].fd = server_fd;
                    fds[1].events = POLLIN;
                    while (1) {
                        char buffer[1024] = {0};
                        ssize_t sent_bytes = 0;
                        int nfds = 2, timeout = 5000;
                        int ret = poll(fds, nfds, timeout); // wait for up to 5 seconds
                        if (ret == -1) {
                            perror("poll");
                            close(client_fd);
                            close(proxy_fd);
                            close(server_fd);
                            exit(EXIT_FAILURE);
                        } else if (ret == 0) {
                            printf("Connection timed out, closing connection\n");
                            break;
                        } else {
                            int exitCondition = 0;
                            for (int i = 0; i < nfds; i++) {
                                if (fds[i].revents & POLL_HUP) {
                                    printf("%s: hung up.\n", i ? "Server" : "Client");
                                    exitCondition = 1;
                                    break;
                                }
                                if (fds[i].revents & POLLERR) {
                                    printf("%s: hung up.\n", i ? "Server" : "Client");
                                    close(server_fd);
                                    close(client_fd);
                                    close(proxy_fd);
                                    exit(EXIT_FAILURE);
                                }
                                if (fds[i].revents & POLLIN) {
                                    if ((sent_bytes = read(fds[i].fd, buffer, sizeof(buffer))) > 0) {
                                        total[i] += sent_bytes;
                                        write(fds[1 - i].fd, buffer, sent_bytes);
                                    }
                                }
                            }
                            if (exitCondition) break;
                        }
                    }
                    // Open a log file
                    struct flock lock;
                    int log_fd = open("proxy.log", O_WRONLY);
                    if (log_fd < 0) {
                        perror("error opening log file");
                        close(client_fd);
                        close(proxy_fd);
                        exit(EXIT_FAILURE);
                    }
                    // Lock printing to the log
                    memset(&lock, 0, sizeof(lock));
                    lock.l_type = F_WRLCK;
                    fcntl(log_fd, F_SETLKW, &lock);
                    // Write the logstring to the log_fd file stream
                    char* logstring = (char *)malloc(1024 * sizeof(char));
                    format_connect_entry(logstring,&client_address, url, total[0], total[1]);
                    // Go to end of the file
                    lseek(log_fd, 0, SEEK_END);
                    write(log_fd, logstring, strlen(logstring));
                    free(logstring);
                    // Release the lock
                    lock.l_type = F_UNLCK;
                    fcntl(log_fd, F_SETLKW, &lock);
                    close(log_fd);

                    close(server_fd);
                    close(client_fd);
                    close(proxy_fd);
                    exit(EXIT_SUCCESS);
                } else {
                    fprintf(stderr, "Not a GET or CONNECT request\n");
                    close(client_fd);
                    close(proxy_fd);
                    exit(EXIT_FAILURE);
                }  
            }
            // Close the file descriptors and exit
            close(client_fd);
        }
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

void format_connect_entry(char *logstring, struct sockaddr_in *sockaddr, char *uri, int sent, int recv) {
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
    sprintf(logstring, "%s CONNECT from %d.%d.%d.%d to %s, Data Transferred %d/%d\n", time_str, a, b, c, d, uri, sent, recv);
}
