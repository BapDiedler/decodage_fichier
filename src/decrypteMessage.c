/**
* @brief le programme permet de tester si un fichier donné en paramètre éxiste ou non
* @author baptistedie
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>


/**
 * @brief fonction qui permet de savoir si le nombre d'arguments est correcte
 *
 * @param nb_arguments nombre d'arguments dans le main
 */
void validation_arguments(int nb_arguments){
    //le nombre d'arguments doit être de 1
    if(nb_arguments != 4){
        perror("le nombre d'arguments doit être de 1");
        exit(-1);
    }
}


/**
 * méthode qui gère la création de processus
 *
 * @param lecture mode lecture du tube
 * @param ecriture mode ecriture du tube
 * @param mot mot à tester
 * @param decalage décalage dans le code
 */
void creation_processus(int lecture,int ecriture, char* mot, char* decalage){
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
            execl("./decalageMessage","decalageMessage",valEcriture,valLecture,mot,decalage,NULL);
            fflush(stdout);
            break;
        default ://programme père
            break;
    }
}


/**
 * fonction qui permet l'ouverture du fichier
 *
 * @param nom_fichier nom de fichier à ouvrir
 *
 * @return descripteur de fichier
 */
int ouverture_fichier(char* nom_fichier){
    //ouverture du fichier
    int fp = open(nom_fichier,O_RDONLY);
    if (fp == -1) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(-1);
    }
    return fp;
}


/**
 * fonction qui regarde si un fichier est crypté
 *
 * @param fp fichier que l'on test
 */
void test_fichier_crypt(int fp){
    //test fichier crypté
    char buffer[2];
    if(read(fp,&buffer, 2*sizeof(char))){
        if(!(buffer[0]=='C' && buffer[1]=='R')){//si le fichier n'est pas crypté
            printf("le fichier n'est pas crypté");
            close(fp);
            exit(-1);
        }
    }
}


/**
 * fonction qui test la validité des données du fichier
 *
 * @param fp fichier
 * @param bufferInt1 taille du fichier
 * @param bufferInt2 taille du message
 */
void test_donnees_fichier(int fp,int* bufferInt1, int* bufferInt2){
    int bufferInt[2];
    //test valeurs fichier
    if(read(fp,&bufferInt,2* sizeof(int))){
        if(!(bufferInt[0]>=0 && bufferInt[0] < lseek(fp, 0, SEEK_END))){
            perror("la taille du fichier n'est pas correcte");
            exit(-1);
        }else{
            *bufferInt1 = bufferInt[0];
        }
        if(!(bufferInt[1]>=0 && bufferInt[1] < lseek(fp, 0, SEEK_END) && bufferInt[0] > bufferInt[1])){
            perror("les valeurs du fichier ne sont pas correctes");
            exit(-1);
        }else{
            *bufferInt2 = bufferInt[1];
        }
    }
}



/**
 * fonction qui récupère les données du programme de décryptage
 *
 * @param nom_fichier fichier sur lequel on a travaillé
 */
void recuperation_donnees_fils(char * nom_fichier){
    int status;
    wait(&status); // Attendre la fin de l'exécution du processus fils
    fflush(stdout);
    if (WIFEXITED(status)) {
        if(WEXITSTATUS(status)!=0){
            printf("FICHIER : %s\n",nom_fichier);
            printf("DECALAGE : %d\n",WEXITSTATUS(status));
        }
    }
}


/**
 * méthode qui va lire le contenu du fichier et le copie dans le tube
 *
 * @param lecture mode lecture du tube
 * @param ecriture mode ecriture du tube
 * @param fp fichier
 * @param bufferInt buffer sur le descritif du fichier
 */
void lecture_message(int lecture, int ecriture, int fp, int* bufferInt){
    char cara;
    close(lecture) ;
    //lecture du message
    lseek(fp, (__off_t) (bufferInt[1] + 2 * sizeof(int) + 2 * sizeof(char)), SEEK_SET);
    for(int i=0;  i < bufferInt[0]; i++){
        if(!read(fp,&cara, sizeof(char))){
            perror("erreur dans la lecture du fichier");
            exit(-1);
        }
        /* écriture (buffer est une suite d'un ou plusieurs octets ici de taille char)*/
        write(ecriture,&cara, sizeof(char)) ;
    }
    /* fin écriture */
    close(ecriture) ;
}


 /**
  * méthode qui permet de vérifier si le fichier est valide et le manipule
  *
  * @param nom_fichier nom de fichier
  * @param mot mot a tester
  * @param decalage nombre de décalage
  */
void fichier_manipulation(char* nom_fichier, char* mot, char* decalage){
    int fp = ouverture_fichier(nom_fichier);
    test_fichier_crypt(fp);
    int bufferInt[2];
    test_donnees_fichier(fp,&bufferInt[0],&bufferInt[1]);
    int tube[2];
    if (pipe(tube) == -1){ /*création d'un tube*/
        perror("création impossible du tube");fflush(stdout);
        exit(1);
    }
    int ecriture = tube[1];
    int lecture = tube[0];
    creation_processus(lecture,ecriture,mot,decalage);
    lecture_message(lecture,ecriture,fp,bufferInt);
    recuperation_donnees_fils(nom_fichier);
    close(fp);
}


/**
 * programme principal
 *
 * @param argc nombre d'arguments
 * @param argv les arguments
 *
 * @return 1 si tout c'est bien passé ou -1 en cas d'erreur
 */
int main(int argc, char** argv){

    validation_arguments(argc);

    char* nom_fichier = argv[1];

    fichier_manipulation(nom_fichier,argv[2],argv[3]);

    return EXIT_SUCCESS;
}