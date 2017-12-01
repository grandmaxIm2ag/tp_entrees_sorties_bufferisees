#include "include/bfile.h"
#include "include/format_in_out.h"
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{

    bfile * bf = bOpen("fichier_test.txt", 'E');
    char s [128] = "coucou est ce que cà marche ?\n";
    bWrite(s, sizeof(s), 1, bf);

    fbWrite(bf, "Cela va t'il fonctionner ? %d %c %s \n", 256, 'c', "Oui");
    bClose(bf);
    
    bf = bOpen("fichier_test.txt", 'L');
    int n;
    while( (n=bRead(s, sizeof(s), 1, bf) > 0) )
    {
        printf("%s",s);
    }
    bClose(bf);
}
