#include "include/format_in_out.h"
#include "include/bfile.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/types.h>

/**
 *La structure pour l'accès à un fichier f, ouvert en mode lecture ou en écriture se fassent
 *par le biais d'un buffer pour gèrerer l'allocation, libération, le remplissage et le vidage. 
 */
struct bfile
{
    FILE * f;
    char mode;
    char * buffer;
    unsigned file_seek;
    unsigned buffer_seek; 
    size_t size_buffer;
    unsigned eof;
};
/**
 *Le but de la fonction est ouvrir le fichier avec le nom path uniquement avec le mode
 *d'ouverture en lecture ('L') ou en écriture 'E' et allouer la structure de type FICHIER
 *avec son tampon et son descripteur de fichier. La fonction renvoie NULL si le fichier
 *ne peut pas être ouvert au contraire un pointeur sur un objet de type FICHIER. 
 */
bfile * bOpen(const char * path, char mode){

    if(mode != 'E' && mode != 'L')
    {
        return NULL;
    }

    FILE * f = fopen( path , ((mode == 'E') ? "w+" : "r") );

    if(f == NULL)
    {
        return NULL;
    }
    
    bfile * bf = (bfile *) malloc(sizeof(bfile));
    bf -> f = f;
    bf -> mode = mode;
    bf -> file_seek = 0;
    bf -> buffer_seek = 0;
    bf -> size_buffer = N;
    bf -> buffer = (char *) malloc(sizeof(char)*bf->size_buffer);

    if(mode == 'L')
    {
        bFill(bf);
    }
    bf -> eof = 0;
    return bf;
}
/**
 *Le but de la fonction est fermer et libérée le fichier pointé sur la structure de
 *donnée de type FICHIER.
 */
int bClose(bfile * bf)
{
    if(bf == NULL)
    {
        return -1;
    }

    if(bf -> mode == 'E')
    {
        bFlush(bf);
    }
    
    int ret = fclose(bf->f);

    free(bf -> buffer);
    free(bf);
    
    return ret;
}
/**
 *Le but de la fonction est écrit
 *
int ecrire(void *p, unsigned int taille, unsigned int nbelem, FICHIER *f);
écrit nbelem éléments de données tenant sur taille octets stockés à l’emplacement mémoire
pointé par p dans le fichier pointé par f. 
La fonction ecrire retourne le nombre d’éléments écrits. 
    
Le fichier doit avoir été ouvert en mode ’E’. 
    
    
données devront être écrites dans le tampon contenu dans f qui ne devra être lui-même écrit à
l’aide d’un write qu’en cas de besoin.
*/

int bWrite(void * p, int size, int nb_element, bfile * bf)
{
    if(bf == NULL || p == NULL)
    {
        return -1;
    }

    if(bf -> mode != 'E')
    {
        return -1;
    }

    char * pp = (char*) p;

    
    int ret = 0;

    int i;
    for( i = 0 ; i < nb_element*size; i++ )
    {
        if(pp[i] != '\0')
        {
            bf -> buffer [bf -> buffer_seek++] =  pp[i];
            if(bf -> buffer_seek >= bf -> size_buffer
               ||  pp[i] == '\n' )
                {
                    bFlush(bf);
                }
            ret ++;
        }
        
    }
    return ret;
}

int bRead(void * p, int size, int nb_element, bfile * bf)
{

    if(bf == NULL || p==NULL)
    {
        return -1;
    }

    if(bf -> mode != 'L')
    {
        return -1;
    }

    char * pp = (char *) p;

    int i, ret=0;
    for( i = 0 ; i < nb_element*size && !bf->eof ; i++ )
    {
        if(bf -> buffer [bf -> buffer_seek] != '\0')
        {
            pp[i] = bf -> buffer [bf -> buffer_seek++];
            if(bf -> buffer_seek >= bf -> size_buffer)
             {
                 bFill(bf);
             }
            ret ++;
        }
        else
        {
            break;
        }

    }
    pp[i] = '\0';
    
    return ret;
}

int bFlush(bfile * bf)
{
    int ret = fwrite(bf -> buffer, 1,bf->buffer_seek, bf -> f);
    fflush(bf->f);
    bf -> buffer_seek = 0;
    
    return ret;
}

int bFill(bfile * bf)
{
    int ret =  fread(bf -> buffer, 1,bf -> size_buffer, bf ->f);
    bf-> buffer_seek= 0;
    bf -> size_buffer = ret;
    if(ret==0)
        bf->eof=1;
    return ret;
}
