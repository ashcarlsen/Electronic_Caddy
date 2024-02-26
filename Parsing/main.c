#include <string.h>
#include <stdio.h>
#include <stdlib.h>


int main(void)
{
    unsigned char str[] = "424.000,4144.9243,N,11149.2188,W,2,10,0.80,1421.0,M,-16.5,M,,*69\r\n$GPGSA,A,3,17,22,14,02,13,30,06,21,15,24,,,1.10,0.80,0.75*0F\r\n$GPRMC,220424.000,A,4144.9243,N,11149.2188,W,0.20,329.94,270124,,,D*78\r\n$GPGGA,220425.000,4144.9243,N,11149.2188,W,2,10,0.80,142";

    char *sentence;
    char *delim = "$";
    char *ptr = strtok(str, delim);
    while(ptr != NULL)
    {
        int len = strlen(ptr);
        if(ptr[0] == 'G' && ptr[1] == 'P' && ptr[2] == 'G' && ptr[3] == 'G' && ptr[4] == 'A' && ptr[len-1] == '\n')
        {
            sentence = ptr;
            printf("%s\n", sentence);
        }
        ptr = strtok(NULL, delim);
    }


    return 0;
}