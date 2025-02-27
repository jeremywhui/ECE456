// get hostinfo from domain name or IP address string
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <arpa/inet.h>
  
int main(int argc, char const *argv[])
{
    struct in_addr addr;
    struct hostent *hostp;
    char **pp;
    char ip_str[INET_ADDRSTRLEN];

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <domain name or dotted-decimal>\n", argv[0]);
        exit(1);
    }

    if (inet_pton(AF_INET, argv[1], &addr) > 0)
    {
        if ((hostp = gethostbyaddr((const char *)&addr, sizeof(addr), AF_INET)) == NULL) {
            fprintf(stderr, "gethostbyaddr error: %s\n:", hstrerror(h_errno)); 
            return 1;
        }
    }
    else {
        if ((hostp = gethostbyname(argv[1])) == NULL) {
            fprintf(stderr, "gethostbyname error: %s\n:", hstrerror(h_errno)); 
            return 1;
        }
    }
    printf("official hostname: %s\n", hostp->h_name);
    // print alias
    for (pp = hostp->h_aliases; *pp != NULL; pp++)  {
        printf("alias: %s\n", *pp);
    }
    // print IP addresses
    for (pp = hostp->h_addr_list; *pp != NULL; pp++)  {
        addr.s_addr = ((struct in_addr *) *pp)->s_addr;
        if (inet_ntop(AF_INET, &addr, ip_str, sizeof(ip_str))
                    == NULL)
        {
            perror("inet_ntop error");
            exit(EXIT_FAILURE);
        }
        printf("address: %s\n", ip_str);
    }
    return 0;
}
