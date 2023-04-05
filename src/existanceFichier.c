/**
* @brief le programme permet de tester si un fichier donné en paramètre éxiste ou non
 * @author baptistedie
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>


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

/**
 * @brief fonction qui regarde si le fichier est crypté ou non
 *
 * @param nom_fichier nom du fichier que l'on observe
 */
void fichier_cripte(FILE * fp){

}

/**
 * @biref méthode qui permet de vérifier si le fichier est valide
 *
 * @param nom_fichier nom de fichier à tester
 */
void fichier_valide(char* nom_fichier){

    //ouverture du fichier
    int fp = open(nom_fichier,O_RDONLY);
    if (fp == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(-1);
    }
    char buf;
    char buffer[2];
    int i=0;
    while(read(fp,&buf,1) && i<2){
        buffer[i]=buf;
        i++;
    }
    if(buffer[0]=='C' && buffer[1]=='R'){
        printf("le fichier est crypté");
    }

    close(fp);
}


int main(int argc, char** argv){

    char* nom_fichier = argv[1];

    validation_arguments(argc);
    fichier_valide(nom_fichier);

    return EXIT_SUCCESS;
}