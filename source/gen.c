#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "gen.h"

char *GEN_PseudoGen_String(int max) {
    srand(time(NULL));

    char *buf = malloc(sizeof(char) * max + 1);

    int n = 0;
    for (int i = 0; i < max; i++) {

        int foo = rand() % 123;
        if ((foo > 47 && foo < 58) || (foo > 96 && foo < 123))
            buf[n++] = foo;
        else i--;
    }

    buf[n] = '\0';
    return buf;
}
