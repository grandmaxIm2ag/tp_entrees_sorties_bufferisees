/**
 * \file generator.c
 *
 * \brief Génère des fichiers de taille et de contenu aléatoire.
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char ** argv)
{
    if(argc != 2)
    {
        fprintf(stderr, "Error : Give filename\n");
    }
    int fd = open(argv[1], O_WRONLY | O_CREAT);
    srand(time(NULL));
    int length = rand() % 10000;
    for(int i=0; i<length; i++)
    {
        int r = rand()%25 + 65;
        char c = (char)r;
        write(fd,&c,1);
    }
    close(fd);
}
