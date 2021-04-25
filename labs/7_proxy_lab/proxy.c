#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "csapp.h"
#include "sbuf.h"

/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400
#define MAX_HDRS 102400
#define NTHREADS 4
#define SBUFSIZE 16

void *thread(void *vargp);
void doit(int fd);
int is_valid_method(char *method);
int is_valid_version(char *version);
int is_absolute_uri(char *uri);
int is_absolute_path(char *uri);
int parse_uri(char *uri, char *host, char *path);
int parse_requesthdrs(rio_t *rp, char *hdrs, char *host);

static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";

sbuf_t sbuf;                    /* Shared buffer of connected descriptors */

void
*thread(void *vargp)
{
        int connfd;

        Pthread_detach(pthread_self());
        while (1) {
                connfd = sbuf_remove(&sbuf); /* Remove connfd from buffer */
                doit(connfd);
                Close(connfd);
        }
}

void
doit(int fd)
{
        int clientfd;
        char *ptr;
        char buf[MAXLINE], method[MAXLINE], uri[MAXLINE],
                host[MAXLINE], port[MAXLINE], path[MAXLINE],
                version[MAXLINE], clientreq[MAXLINE];
        char *hdrs;
        rio_t rio, clientrio;

        hdrs = (char *)Malloc(MAX_HDRS*sizeof(char));
        strcpy(hdrs, "");

        /* read request */
        Rio_readinitb(&rio, fd);
        if (!Rio_readlineb(&rio, buf, MAXLINE))
                printf("TODO: HANDLE ERROR\n");
        printf("\nINCOMING REQUEST:\n");
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

        /* Read headers */
        if (!parse_requesthdrs(&rio, hdrs, host))
                printf("TODO: HANDLE ERROR\n");

        strcpy(clientreq, method);
        strcat(clientreq, " ");
        strcat(clientreq, path);
        strcat(clientreq, " HTTP/1.0\r\n");
        printf("\nOUTGOING REQUEST: \n");
        printf("%s", clientreq);
        printf("%s", hdrs);

        if ((ptr = strchr(host, ':')) != NULL) {
                *ptr = '\0';
                strcpy(port, ptr+1);
        } else {
                strcpy(port, "80");
        }


        clientfd = Open_clientfd(host, port);
        Rio_readinitb(&clientrio, clientfd);
        Rio_writen(clientfd, clientreq, strlen(clientreq));
        Rio_writen(clientfd, hdrs, strlen(hdrs));

        while (Rio_readnb(&clientrio, buf, MAXLINE)) {
                Rio_writen(fd, buf, MAXLINE);
        }

        Close(clientfd);
        Free(hdrs);
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

/*
 * parse_requesthdrs - Parse request headers and store the result in
 * hdrs. If headers provide "Host" header, the value will be extracted
 * and will replace host. Returns 1 if parsing was successful. Otherwise
 * it returns 0.
 */
int parse_requesthdrs(rio_t *rp, char *hdrs, char *host)
{
        char *ptr, *keyp;
        int host_exst;
        char buf[MAXLINE], buf_dup[MAXLINE];

        host_exst = 0;
        Rio_readlineb(rp, buf, MAXLINE);

        strcat(hdrs, "User-Agent: ");
        strcat(hdrs, user_agent_hdr);
        strcat(hdrs, "Connection: close\r\n");
        strcat(hdrs, "Proxy-Connection: close\r\n");

        while (strcmp(buf, "\r\n")) {
                printf("%s", buf);
                if((ptr = strchr(buf, ':')) == NULL)
                        return 0;
                *ptr++ = '\0';
                while(*ptr == ' ' || *ptr == '\t')
                        ptr++;

                strcpy(buf_dup, buf);
                for (keyp = buf_dup; *keyp; keyp++)
                        *keyp = tolower(*keyp);

                if (strstr(buf_dup, "host")) {
                        host_exst = 1;
                        strcpy(host, ptr);
                        *(host+strlen(host)-2) = '\0'; /* Remove CLRS */
                }

                /* Skip preset headers */
                if (strstr(buf_dup, "user-agent") ||
                    strstr(buf_dup, "connection") ||
                    strstr(buf_dup, "proxy-connection")) {
                        Rio_readlineb(rp, buf, MAXLINE);
                        continue;
                }

                strcat(hdrs, buf); /* Key */
                strcat(hdrs, ": ");
                strcat(hdrs, ptr); /* Value */

                Rio_readlineb(rp, buf, MAXLINE);
        }

        if (!host_exst) {
                strcat(hdrs, "Host: ");
                strcat(hdrs, host);
                strcat(hdrs, "\r\n");
        }

        strcat(hdrs, "\r\n");

        return 1;
}

int
main(int argc, char **argv)
{
        int i, listenfd, connfd;
        char hostname[MAXLINE], port[MAXLINE];
        socklen_t clientlen;
        pthread_t tid;
        struct sockaddr_storage clientaddr;

        if (argc != 2) {
                fprintf(stderr, "usage: %s <port>\n", argv[0]);
                exit(1);
        }

        listenfd = Open_listenfd(argv[1]);

        sbuf_init(&sbuf, SBUFSIZE);
        for (i = 0; i < NTHREADS; i++) /* Create worker threads */
                Pthread_create(&tid, NULL, thread, NULL);

        while (1) {
                clientlen = sizeof(clientaddr);
                connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
                Getnameinfo((SA *) &clientaddr, clientlen, hostname,
                            MAXLINE, port, MAXLINE, 0);
                printf("Accepted connection from (%s, %s)\n", hostname, port);
                sbuf_insert(&sbuf, connfd); /* Insert connfd in buffer */
        }
}
