/**
* @brief le programme permet de tester si un fichier donné en paramètre éxiste ou non
 * @author baptistedie
*/

#include <stdlib.h>
#include <stdio.h>

void validation_arguments(int nb_arguments){
    //le nombre d'arguments doit être de 1
    if(nb_arguments != 2){
        perror("le nombre d'arguments doit être de 1");
        exit(-1);
    }
}


int main(int argc, char** argv){

    validation_arguments(argc);

    return EXIT_SUCCESS;
}