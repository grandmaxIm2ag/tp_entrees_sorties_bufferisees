#include "include/bfile.h"
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{

    bfile * bf = bOpen("fichier_test.txt", 'E');
    char s [128] = "coucou est ce que cà marche ?\n";
    bWrite(s, sizeof(s), 1, bf);
    bClose(bf);
    /*
    bf = bOpen("lect.txt", 'L');
    int n;
    while( (n=bRead(s, sizeof(s), 1, bf) > 0) )
    {
        for(int i=0; i<n; i++)
        {
            printf("%c", s[i]);
        }
    }
    bClose(bf);*/
}
