#include "include/format_in_out.h"
#include "include/bfile.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/types.h>
#include <string.h>

/**
 *Le but de la fonction est convertir le paramètre entier reçu à un pointeur
 * de type char pour ecrire cette type de donnée dans la structure de type
 *FICHIER.
 */
char * convert_int_to_str(int x)
{
    static char buff[11];
    char * res;
    res = &buff[10];
    *res = '\0';

    int n = x;

    do
    {
        char c = (n % 10) + 48;
        *(--res) = c;
        n /= 10;
    }while(n > 0);
    return res;
}
/**
 *Le but de la fonction est identifier quand le paramètre de la chaine reçu
 *c'est un separator.
 */
unsigned is_separator(char c)
{
    return (c=='\n' || c==' ' || c=='\t' || c=='\0');
}

/**
 *Le but de la fonction est ecrire avec la fonction bWrite dans la structure
 *de type FICHIER un nombre variable de paramètres(format). Il faut 
 *parcourir la chaine reçu, caractère par caractère jusqu'au prochain %
 *en identifiant le type de donnée manipulés (caractère,chaı̂ne ou entier)
 *pour que selon le cas definir les paramètres d'ecriture.
 */
int fbWrite(bfile * bf, char * format, ...)
{
    char * str, *s, c;
    
    int x, count=0;
    
    va_list arg; 
    va_start(arg, format);

    //On parcourt la chaîne a écrire
  
    for(str=format; *str!='\0'; str++)
    {
        //on parcourt jusqu'au prochain %
        while(*str != '%')
        {
            c = *str;
            count ++;
            bWrite(&c, 1, 1, bf);
            str++;
        }

        //On récupert le caractère suivant le %
        str++;
        c = *str;
        switch(c)
        {
        case 'd':
            x = va_arg(arg,int);
            s = convert_int_to_str(x);
            count ++;
            bWrite(s, sizeof(s), 1, bf);
            break;
        case 'c':
            c = va_arg(arg,int);
            count ++;
            bWrite(&c, 1, 1, bf);
            break;
        case 's':
            s = va_arg(arg,char*);
            count ++;
            bWrite(s, strlen(s), 1, bf);
            break;
        default:
            return count;
        }
    }
    va_end(arg);
    
    return count;
}
/**
 *Le but de la fonction est lire avec la fonction bRead la structure
 *de type FICHIER un nombre variable de paramètres(format). Il faut 
 *parcourir la chaine reçu, caractère par caractère jusqu'au prochain %
 *en identifiant le type de donnée manipulés (caractère,chaı̂ne ou entier) 
 *pour que selon le cas definir les paramètres de lecture et reasignar
 *un bloque de memoria.
 */
int fbRead(bfile * bf,  char * format, ...)
{
    char * str, *s,s1[N],*s2, c, * c1;
    s2 = (char *)malloc(N);
    int *x, count=0, seek=0, k, j;
    unsigned b = 1, b2;
    va_list arg;
    bRead(s1, 1, N, bf);
    va_start(arg, format);

    //On parcourt la chaîne a lire
    for(str=format; *str!='\0'; str++)
    {
        //on parcourt jusqu'au prochain %
        while(*str != '%')
        {
            str++;
        }
        //On récupert le caractère suivant le %
        str++;
        c = *str;
        switch(c)
        {
        case 'd':
            x = (int *)va_arg(arg,int*);
            k=0;
            b2=1;
            do
            {
                for(k=seek; k<N && b2; k++ )
                {
                    if(!is_separator(s1[k]))
                    {
                        s2[k] = s1[k];
                    }
                    else
                    {
                        b2=0;
                    }
                    if(j>=N)
                    {
                        s2=realloc(s2, sizeof(s2)+N);
                    }
                }
                seek=k;
                if((k=bRead(s1, 1, N, bf)))
                {
                    b = 0;
                }
            }while(b && b2);
            *x = atoi(s2);
            count ++;
            break;
        case 'c':
            c1 = (char *) va_arg(arg,char*);
            *c1 = s1[seek++];
            count ++;
            seek++;
            break;
        case 's':
            s = (char *)va_arg(arg,char*);
            b2=1;
            j=0;
            do
            {
                for(k=seek; k<N && b2; k++ )
                {
                    if(!is_separator(s1[k]))
                    {
                        s[j++] = s1[k];
                    }
                    else
                    {
                        b2=0;
                    }
                    if(j>=N)
                    {
                        s=realloc(s, sizeof(s2)+N);
                    }
                }
                if(b2 && (k=bRead(s1, 1, N, bf)))
                {
                    b = 0;
                }
                seek=k;
            }while(b && b2);
            count ++;
            break;
        default:
            return count;
        }
    }
    va_end(arg);
    
    return count;
}

