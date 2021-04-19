#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "csapp.h"

/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400

void doit(int fd);
int is_valid_method(char *method);
int is_valid_uri(char *uri);
int is_valid_version(char *version);

/* static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n"; */

void
doit(int fd)
{
        char buf[MAXLINE], method[MAXLINE], uri[MAXLINE],
                version[MAXLINE];
        rio_t rio;

        Rio_readinitb(&rio, fd);
        if (!Rio_readlineb(&rio, buf, MAXLINE))
                return;
        printf("%s", buf);
        sscanf(buf, "%s %s %s", method, uri, version);

        if (!(is_valid_method(method) &&
              is_valid_uri(uri) &&
              is_valid_version(version))) {
                printf("TODO: HANDLE ERROR\n");
        }
}

int
is_valid_method(char *method)
{
        return !strcasecmp(method, "GET");
}

int
is_valid_uri(char *uri)
{
        char *urip = uri;

        for ( ; *urip; urip++)
                *urip = tolower(*urip);

        return strstr(uri, "http://") == uri ||
                strstr(uri, "/") == uri;
}

int
is_valid_version(char *version)
{
        return strcasecmp(version, "HTTP/0.9") == 0 ||
                strcasecmp(version, "HTTP/1.0") == 0 ||
                strcasecmp(version, "HTTP/1.1") == 0 ||
                strcasecmp(version, "HTTP/2.0") == 0;
}


int
main(int argc, char **argv)
{
        int listenfd, connfd;
        char hostname[MAXLINE], port[MAXLINE];
        socklen_t clientlen;
        struct sockaddr_storage clientaddr;

        if (argc != 2) {
                fprintf(stderr, "usage: %s <port>\n", argv[0]);
                exit(1);
        }

        listenfd = Open_listenfd(argv[1]);
        while (1) {
                clientlen = sizeof(clientaddr);
                connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
                Getnameinfo((SA *) &clientaddr, clientlen, hostname,
                            MAXLINE, port, MAXLINE, 0);
                printf("Accepted connection from (%s, %s)\n", hostname, port);
                doit(connfd);
                Close(connfd);
        }
        return 0;
}
