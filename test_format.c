#include "include/bfile.h"
#include "include/format_in_out.h"
#include <time.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

int main(int argc, char ** argv)
{

    if(argc != 2)
    {
        fprintf(stderr, "Error : Give filename\n");
    }

    int d=98;
    char c='e';
    char * s1 = "une_chaine_sans_separateur",
        *s2 = "Une autre";

    bfile * out = bOpen(argv[1], 'E');

    fbWrite(out, "%d %c %s %s", d, c, s1, s2);
    bClose(out);
    
    bfile * in = bOpen(argv[1], 'L');
    
    int d_ret;
    char c_ret;
    char s1_ret[26],s2_ret[3];

    fbRead(in, "%d %c %s %s", &d_ret, &c_ret, s1_ret, s2_ret);
    bClose(in);

    assert(d==d_ret);
    assert(c==c_ret);

    for(int i=0; i<26; i++)
    {
        assert(s1[i] == s1_ret[i]);
    }
    for(int i=0; i<3; i++)
    {
        assert(s2[i] == s2_ret[i]);
    }
    return 0;
}
