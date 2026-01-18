#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <ctype.h>

#include "xdump.h"

void hexDump(void * buffer, uint32_t bufferLen) {
    static char szASCIIBuf[17];
    static uint32_t offset = 0;

    uint8_t * buf = (uint8_t *)buffer;

    int j = 0;
    for (int i = 0;i < bufferLen;i++) {
        if ((i % 16) == 0) {
            if (i != 0) {
                szASCIIBuf[j] = 0;
                j = 0;

                printf("  |%s|\n", szASCIIBuf);
            }
                
            printf("%08X\t", offset);
            offset += 16;
        }

        if ((i % 2) == 0 && (i % 16) > 0) {
            printf(" ");
        }

        printf("%02X", buf[i]);
        szASCIIBuf[j++] = isprint(buf[i]) ? buf[i] : '.';
    }

    /*
    ** Print final ASCII block...
    */
    szASCIIBuf[j] = 0;
    printf("  |%s|\n", szASCIIBuf);
}
