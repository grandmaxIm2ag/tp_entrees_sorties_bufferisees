#include "include/bfile.h"
#include "include/format_in_out.h"
#include <time.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char ** argv)
{
    if(argc != 3)
    {
        fprintf(stderr,"Erreur Argument : %d argument(s) au lieu des deux attendus", argc);
        fprintf(stderr, "usage : %s <name_file1> <name_file2>\n"
                , argv[0]);
        fprintf(stderr, "VOir le readme pour plus d'explications\n");
        exit(1);
    }

    srand(time(NULL));
    bfile * in = bOpen(argv[1], 'L');
    bfile * out = bOpen(argv[2], 'E');

    char s[100];
    int r,n;

    do
    {
        r = rand()%100 + 1;
        n = bRead(s, 1, r, in);
        if(n>0)
        {
            bWrite(s, n, 1, out);
        }
    }while(n>0);
    bClose(in);
    bClose(out);
    return 1;
}
