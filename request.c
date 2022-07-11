#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>


struct rq {
    char** u;
    char** m;
    char** c;
};

struct rq getrq(char buffer[1024]) {

    struct rq r = {malloc(1),malloc(1), malloc(1)};

    int idx = 0;

    char** sl = malloc(1);

    const char * curLine = buffer;
    while(curLine)
    {
        const char * nextLine = strchr(curLine, '\n');
        int curLineLen = nextLine ? (nextLine-curLine) : strlen(curLine);
        char * temp = (char *) malloc(curLineLen+1);
        if (temp)
        {
            memcpy(temp, curLine, curLineLen);
            temp[curLineLen] = '\0';  // NUL-terminate!
            if(idx == 0){
                sl[0] = temp;
            }
            if(idx == 7){
                r.c[0] = temp;
            }

            //printf("%s\n", temp);
        }
        else printf("malloc() failed!?\n");

        idx++;

        curLine = nextLine ? (nextLine+1) : NULL;
    }

    printf("c:%s\n", r.c[0]);
    printf("sl:%s\n", sl[0]);
    return r;
}