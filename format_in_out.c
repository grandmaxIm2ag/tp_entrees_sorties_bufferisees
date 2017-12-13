/**
 * \file format_in_out.c
 *
 * \brief Implémentation des écritures et lectures formatées
 */

#include "include/format_in_out.h"
#include "include/bfile.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/types.h>
#include <string.h>

/**
 * \fn char * convert_int_to_str(int x)
 * \param x : l'entier à converir en chaîne
 * \return Une chaîne de caractère
 * \brief Le but de la fonction est de convertir un entier en chaîne.
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
 * \fn unsigned is_separator(char c)
 * \param c : le caractère à tester
 * \return Un booléen, true si c est séparateur, false sinon
 * \brief Le but de la fonction est de vérifier qu'un caractère est un 
 * séparateur ou non.
 */
unsigned is_separator(char c)
{
    return (c=='\n' || c==' ' || c=='\t' || c=='\0');
}

/**
 * \fn int fbWrite(bfile * bf, char * format, ...)
 * \param bf : la structure du fichier bufférisé
 * \param format : La chaîne formatée
 * \param ... : Les différentes variables à écrire
 * \return Le nombre d'octet écrit
 * \brief Le but de la fonction est d'écrire la chaîne format dans bf->f
 * en remplaçant les % par les valeurs des variables.
 */
int fbWrite(bfile * bf, char * format, ...)
{
    char * str, *s, c;
    
    int x, count=0;
    //contient des informations sur les arguments variables
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
        //cas dont l'argument est un entier : %d
        case 'd':
            x = va_arg(arg,int);
            s = convert_int_to_str(x);
            count ++;
            bWrite(s, sizeof(s), 1, bf);
            break;
        //cas dont l'argument est un caractère : %c
        case 'c':
            c = va_arg(arg,int);
            count ++;
            bWrite(&c, 1, 1, bf);
            break;
        //cas dont l'argument est une chaı̂ne : %s
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
 * \fn int fbread(bfile * bf, char * format, ...)
 * \param bf : la structure du fichier bufférisé
 * \param format : La chaîne formatée
 * \param ... : Les différents pointeurs des variables à écrire
 * \return Le nombre d'octet lus
 * \brief Le but de la fonction est de lire la chaîne format dans bf->f
 * en écrivant les valeurs des % dans la mémoire pointée par les variables.
 */
int fbRead(bfile * bf,  char * format, ...)
{
    char * str, *s,s1[N],*s2, c, * c1;
    s2 = (char *)malloc(N);
    int *x, count=0, seek=0, k, j;
    unsigned b = 1, b2;
    //contient des informations sur les arguments variables
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
        //cas dont l'argument est un entier : %d
        case 'd':
            x = (int *)va_arg(arg,int*);
            k=0;
            b2=1;
            do
            {
                //On lit l'entier contenu dans bf->f 
                for(k=seek; k<N && b2; k++ )
                {
                    if(!is_separator(s1[k]))
                    {
                        s2[k] = s1[k];
                    }
                    else
                    {
                        //Indique que l'entier a été lu entièrement
                        b2=0;
                    }
                    if(j>=N)
                    {
                        //Si l'entier est supérieurs à N, on ralloue s2
                        s2=realloc(s2, sizeof(s2)+N);
                    }
                }
                seek=k;
                if(b2 && (k=bRead(s1, 1, N, bf)))
                {
                    b = 0;
                }
            }while(b && b2);
            *x = atoi(s2);
            count ++;
            break;
        //cas dont l'argument est un caractère : %c
        case 'c':
            c1 = (char *) va_arg(arg,char*);
            *c1 = s1[seek++];
            count ++;
            seek++;
            break;
        //cas dont l'argument est une chaîne : %s
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

