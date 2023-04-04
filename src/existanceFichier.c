/**
* @brief le programme permet de tester si un fichier donné en paramètre éxiste ou non
 * @author baptistedie
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

/**
 * @brief fonction qui permet de savoir si le nombre d'arguments est correcte
 *
 * @param nb_arguments nombre d'arguments dans le main
 */
void validation_arguments(int nb_arguments){
    //le nombre d'arguments doit être de 1
    if(nb_arguments != 2){
        perror("le nombre d'arguments doit être de 1");
        exit(-1);
    }
}

/**
 * @brief fonction qui permet de savoir si le fichier existe
 *
 * @param nom_fichier nom de fihier à vérifier
 */
void fichier_existe(char * nom_fichier){
    if(access(nom_fichier, F_OK ) != -1){
        perror("le fichier n'existe pas !!!");
        exit(-1);
    }
}


int main(int argc, char** argv){

    validation_arguments(argc);
    fichier_existe(argv[1]);

    return EXIT_SUCCESS;
}