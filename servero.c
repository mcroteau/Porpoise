#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 3000


int handle(int socket, int fd, struct sockaddr *socketaddr, socklen_t *pInt, char *output);

void tostring(char *size, int length);

char *strip(char *bare, const char *string);

int main(int argc, char const *argv[])
{

    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR,
                   &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    if (bind(server_fd, (struct sockaddr *)&address,
            sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }


    char* output[3000];

    char* content = "∆˚¬";
    char* httpVersion = "HTTP/1.1 200 OK\r\n";
    char* contentLengthTitle = "Content-Length: ";
    int length = strlen(content);

    char *contentLengthSize[3000];
    tostring(contentLengthSize, length);
    char* newLine = "\r\n";
    char contentLength[3000];
    strncat(contentLength, contentLengthTitle, strlen(contentLengthTitle));
    strncat(contentLength, contentLengthSize, strlen(contentLengthSize));
    strncat(contentLength, newLine, strlen(newLine));

    char* server = "Server: n\r\n";
    char* contentType = "Content-Type: text/html; charset=UTF-8";
    strncat(output, httpVersion, strlen(httpVersion));
    strncat(output, contentLength, strlen(contentLength));
    strncat(output, server, strlen(server));
    strncat(output, contentType, strlen(contentType));
    strncat(output, newLine, strlen(newLine));
    strncat(output, newLine, strlen(newLine));
    strncat(output, content, strlen(content));
    strncat(output, newLine, strlen(newLine));
    strncat(output, newLine, strlen(newLine));

    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    while(1){
        struct sockaddr *sa = (struct sockaddr *)&address;
        socklen_t *sl = (socklen_t*)&addrlen;
        handle(new_socket, server_fd, sa, sl, output);
    }
}


struct Headers{
    char** statusline;
    char** contenttype;
};


struct Headers getheaders(char buffer[1024]);

int handle(int new_socket, int server_fd, struct sockaddr *sa, socklen_t *sl, char *output) {

    if ((new_socket = accept(server_fd, sa, sl))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    char buffer[1024] = {0};
    read(new_socket, buffer, 1024);

    struct Headers headers = getheaders(buffer);
    printf("statusline %s\n", headers.statusline[0]);
    printf("contenttype %s\n", headers.contenttype[0]);

    write(new_socket, output, strlen(output));

    return 0;
}



struct Headers getheaders(char buffer[1024]) {

    struct Headers headers = {malloc(1), malloc(1)};

    int idx = 0;

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
                headers.statusline[0] = temp;
            }
            if(idx == 7){
                headers.contenttype[0] = temp;
            }

            printf("%s\n", temp);
        }
        else printf("malloc() failed!?\n");

        idx++;

        curLine = nextLine ? (nextLine+1) : NULL;
    }

    //    free(statusline);
    //    free(contentline);

    return headers;
}



void tostring(char *str, int num) {

    int i, rem, len = 0, n;

    n = num;
    while (n != 0)
    {
        len++;
        n /= 10;
    }
    for (i = 0; i < len; i++)
    {
        rem = num % 10;
        num = num / 10;
        str[len - (i + 1)] = rem + '0';
    }
    str[len] = '\0';
}

char *strip(char *bare, const char *string) {
    char *p, *q;
    for (q = p = bare; *p; p++)
        if (*p != string){
            *q++ = *p;
            printf("q, p, %s, %s, \n", q, p);
        }
    *q = '\0';

}