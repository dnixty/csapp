#include <stdio.h>
#define MAXLINE 12

void good_echo(void)
{
    char buf[MAXLINE];
    char *p;
    while (1) {
        p = fgets(buf, MAXLINE, stdin);
        if (p == NULL)
            break;
        printf("%s", p);
    }
}

int main()
{
    good_echo();
    return 0;
}
