# Lectures et Écriture bufferisées
Barona Stephanie, Grand Maxence

## Compilation

```shell
make all
```

## Exécution

### main

Ce programme lit un fichier donné en argument, et écrit le contenu lu
dans un autre fichier. Ce programme est compilé normalement.

```shell
./main <filename1> <filename2>
```
* filename1 : Le fichier à lire
* filename2 : Le fichier à écrire

### main_static

Ce programme lit un fichier donné en argument, et écrit le contenu lu
dans un autre fichier. Ce programme est compilé avec une bibliothèque
statique.

```shell
./main_static <filename1> <filename2>
```
* filename1 : Le fichier à lire
* filename2 : Le fichier à écrire

### main_dyn

Ce programme lit un fichier donné en argument, et écrit le contenu lu
dans un autre fichier. Ce programme est compilé avec une bibliothèque
dynamique.

```shell
./main_dyn <filename1> <filename2>
```
* filename1 : Le fichier à lire
* filename2 : Le fichier à écrire

### generator

Ce programme écrit un fichier au contenu et à la taille aléatoire dans
un fichier.

```shell
./generator <filename>
```
* filename : Le fichier cible

### test_format

Ce programme écrit une chaîne formatée dans un fichier, puis la relis
et verifie que les valeurs lues sont toujours les mêmes.

```shell
./test_format <filename1>
```
* filename1 : Le fichier où sera écrite puis lue la chaîne formatée

## Tests

```shell
make tests
```

Le tests sont effectuées par le script script_test.sh :
* Test les fonctions d'écritures et lectures bufférisées avec le
programme test_format
* Génère des fichiers au contenu et à la taille aléatoire, donne ce
fichier en entré du programme main (resp main_static  main_dyn) et
vérifie que le fichier <filename2> est identique au fichier <filename1>
