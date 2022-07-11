#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
    printf("Hello, World!\n");
    char* a = "mike";

    *(a + 4) = "s";


    printf("again %s\n", a);

    return 0;

}
