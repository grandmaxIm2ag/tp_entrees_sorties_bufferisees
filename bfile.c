/**
 * \file bfile.c
 *
 * \brief Implémentation des écritures et lectures bufférisées
 */

#include "include/format_in_out.h"
#include "include/bfile.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/types.h>

/**
 * \struct bfile 
 * \brief Structure pour les écritures et lectures bufferisées
 *
 * \param f : le fichier
 * \param mode : Mode de lecteur, E pour écriture, L pour lecture
 * \param buffer : Notre tampon
 * \param file_seek : Curseur de f
 * \param file_seek : Curseur de buffer
 * \param size_buffer : taille du buffer
 * \param eof : booléen indiquant si il reste des choses \`a lire dans le fichier
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
 * \fn bfile * bOpen(const char * path, char mode)
 *
 * \param path : le nom du fichier
 * \param mode : le mode d'ouverture
 * \return le bfile créé
 * \brief Le but de la fonction est d'ouvrir le fichier path avec le mode
 * d'ouverture en lecture 'L' ou en écriture 'E' et d'allouer la structure de 
 * bfile avec son tampon et son fichier. La fonction renvoie NULL si le fichier
 * ne peut pas être ouvert, un bfile sinon. 
 */
bfile * bOpen(const char * path, char mode){

    //Vérifie le mode
    if(mode != 'E' && mode != 'L')
    {
        return NULL;
    }

    //Ouverture du fichier
    FILE * f = fopen( path , ((mode == 'E') ? "w+" : "r") );

    
    if(f == NULL)
    {
        return NULL;
    }

    //Allocation de la strucuture bfile
    bfile * bf = (bfile *) malloc(sizeof(bfile));
    bf -> f = f;
    bf -> mode = mode;
    bf -> file_seek = 0;
    bf -> buffer_seek = 0;
    bf -> size_buffer = N;
    bf -> buffer = (char *) malloc(sizeof(char)*bf->size_buffer);

    //Si on est en lecture, on rempli de le buffer
    if(mode == 'L')
    {
        bFill(bf);
    }
    bf -> eof = 0;
    return bf;
}

/**
 * \fn int bClose(bfile * bf)
 *
 * \param bf : la structure à fermer
 *
 * \return Un entier retourné par la fonction fclose
 *
 * \brief Le but de la fonction est de fermer et libérée le fichier
 * pointé sur la structure de donnée de type bfile.
 */
int bClose(bfile * bf)
{
    if(bf == NULL)
    {
        return -1;
    }

    //Si on est en écriture, on vide le tampon
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
 * \fn int bWrite(void * p, int size, int nb_element, bfile * bf)
 *
 * \param p : le pointeur à écrire
 * \param size : la taille des données à écrire
 * \param nb_element : le nombre de fois où on écrit.
 * \param bf
 *
 * \return le nombre d’octets écrits
 *
 * \brief Le but de la fonction est d'écrire nb_element de size octets stockés
 * à l’emplacement mémoire pointé par p, dans le tampon. 
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
        //On écrit tous, sauf l'octet de fin de chaîne
        if(pp[i] != '\0')
        {
            bf -> buffer [bf -> buffer_seek++] =  pp[i];
            //Si le tampon est plein, où qu'on écrit \n, alors on vide le tampon
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

/**
 * \fn int bRead(void * p, int size, int nb_element, bfile * bf)
 * 
 * \param p : le pointeur à remplir
 * \param size : la taille des données à lrie
 * \param nb_element : le nombre de fois que nous lisons.
 * \param bf
 *
 * \return Le nombre d’octets lus.
 *
 * \brief Le but de la fonction est de lire nb_element de size octets dans bf
 * et y stocker à l’emplacement mémoire pointé par p/
 */
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
        //On écrit dans p tous, sauf l'octet de fin de chaîne
        if(bf -> buffer [bf -> buffer_seek] != '\0')
        {
            pp[i] = bf -> buffer [bf -> buffer_seek++];
            //Si on a lu tous le buffer, alors on le rempli à noveau
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

/**
 *\fn int bFlush(bfile * bf)
 *
 * \param bf
 * 
 * \return Le nombre d'octet écrit dans le fichier
 * \brief Le but de la fonction est xde vider le tampon dans le fichier f
 */
int bFlush(bfile * bf)
{
    int ret = fwrite(bf -> buffer, 1,bf->buffer_seek, bf -> f);
    fflush(bf->f);
    bf -> buffer_seek = 0;
    return ret;
}

/**
 *\fn int bFill(bfile * bf)
 *
 * \param bf
 * 
 * \return Le nombre d'octet lu dans le fichier
 * \brief Le but de la fonction est de remplir le tampon avec le fichier f
 */
int bFill(bfile * bf)
{
    int ret =  fread(bf -> buffer, 1,bf -> size_buffer, bf ->f);
    bf-> buffer_seek= 0;
    bf -> size_buffer = ret;
    //Si rien n'est lu, alors on met à true le bolléen de fin de fichier 
    if(ret==0)
        bf->eof=1;
    return ret;
}
