#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "csapp.h"

/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400

void doit(int fd);
int is_valid_method(char *method);
int is_valid_version(char *version);
int is_absolute_uri(char *uri);
int is_absolute_path(char *uri);
int parse_uri(char *uri, char *host, char *path);

/* static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n"; */

void
doit(int fd)
{
        char buf[MAXLINE], method[MAXLINE], uri[MAXLINE],
                host[MAXLINE], path[MAXLINE], version[MAXLINE];
        rio_t rio;

        /* read request */
        Rio_readinitb(&rio, fd);
        if (!Rio_readlineb(&rio, buf, MAXLINE))
                printf("TODO: HANDLE ERROR\n");
        printf("%s", buf);
        sscanf(buf, "%s %s %s", method, uri, version);

        /* transform the uri to lowercase */
        char *urip = uri;
        for ( ; *urip; urip++)
                *urip = tolower(*urip);

        /* check if the method and version is valid */
        if (!(is_valid_method(method) &&
              is_valid_version(version))) {
                printf("TODO: HANDLE ERROR\n");
        }

        /* parse uri */
        if (!parse_uri(uri, host, path))
                printf("TODO: HANDLE ERROR\n");

        printf("host: %s\n", host);
        printf("path: %s\n", path);

        /* Read headers */



}

int
is_valid_method(char *method)
{
        return !strcasecmp(method, "GET");
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
is_absolute_uri(char *uri)
{
        return strstr(uri, "http://") == uri;
}

int
is_absolute_path(char *uri)
{
        return strstr(uri, "/") == uri;
}

/*
 * parse_uri - Parses given uri and extracts host and path. If the
 * result is successful it returns 1. Otherwise it returns 0.
 */
int
parse_uri(char *uri, char *host, char *path)
{
        char *ptr;

        if (is_absolute_uri(uri)) {
                printf("%s\n", uri + strlen("http://"));
                if ((ptr = strchr(uri+strlen("http://"), '/')) == NULL)
                        return 0;
                strcpy(path, ptr);
                *ptr = '\0';
                strcpy(host, uri+strlen("http://"));
                return 1;
        } else if (is_absolute_path(uri)) {
                strcpy(path, uri);
                strcpy(host, "");
                return 1;
        }
        return 0;
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
