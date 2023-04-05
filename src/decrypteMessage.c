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
 *
 */
void creation_processus(int lecture,int ecriture){
    pid_t res;
    char valLecture[5], valEcriture[5];
    sprintf(valEcriture,"%d",ecriture);
    sprintf(valLecture,"%d",lecture);
    //création d'un processus
    switch ( res = fork() ){
        case (pid_t) -1 ://erreur dans la création de processus
            perror("erreur dans la créatiob de processus");
            exit(1);
        case (pid_t) 0 ://programme fils
            execl("./decalageMessage","decalageMessage",valEcriture,valLecture,NULL);
            fflush(stdout);
            break;
        default ://programme père
            break;
    }
}


/**
 * @biref méthode qui permet de vérifier si le fichier est valide et le manipule
 *
 * @param nom_fichier nom de fichier à tester
 */
void fichier_manipulation(char* nom_fichier){

    //ouverture du fichier
    int fp = open(nom_fichier,O_RDONLY);
    if (fp == -1) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(-1);
    }

    //test fichier crypté
    char buffer[2];
    if(read(fp,&buffer, 2*sizeof(char))){
        if(!(buffer[0]=='C' && buffer[1]=='R')){//si le fichier n'est pas crypté
            printf("le fichier n'est pas crypté");
            close(fp);
            exit(-1);
        }
    }

    //test valeurs fichier
    int bufferInt[2];
    if(read(fp,&bufferInt,2* sizeof(int))){
        if(!(bufferInt[0]>=0 && bufferInt[0]<=lseek(fp,0,SEEK_END))){
            perror("la taille du fichier n'est pas correcte");
            exit(-1);
        }
        if(!(bufferInt[1]>0 && bufferInt[1]<lseek(fp,0,SEEK_END))){
            perror("les valeurs du fichier ne sont pas correctes");
            exit(-1);
        }
    }

    int tube[2];
    char cara;

    if (pipe(tube) == -1){ /*création d'un tube*/
        perror("création impossible du tube");fflush(stdout);
        exit(1);
    }

    int ecriture = tube[1];
    int lecture = tube[0];

    creation_processus(lecture,ecriture);


    close(lecture) ;

    //lecture du message
    lseek(fp,bufferInt[1]+2* sizeof(int)+2* sizeof(char),SEEK_SET);
    for(int i=0; i<bufferInt[0];i++){
        if(!read(fp,&cara, sizeof(char))){
            perror("erreur dans la lecture du fichier");
            exit(-1);
        }
        /* écriture (buffer est une suite d'un ou plusieurs octets ici de taille char)*/
        write(tube[1],&cara, sizeof(char)) ;
    }

    /* fin écriture */
    close(ecriture) ;


    close(fp);
}


int main(int argc, char** argv){

    char* nom_fichier = argv[1];

    validation_arguments(argc);
    fichier_manipulation(nom_fichier);

    return EXIT_SUCCESS;
}