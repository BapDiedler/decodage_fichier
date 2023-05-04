/**
* @brief le programme permet de tester si un fichier donné en paramètre éxiste ou non
* @author baptistedie
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>


#define NBPROCESS 26

/**
 * méthode qui gère la création de processus
 *
 * @param lecture mode lecture du tube
 * @param ecriture mode ecriture du tube
 * @param mot mot à tester
 * @param decalage décalage dans le code
 */
void creation_processus(int lecture, int ecriture, char* mot, char* decalage, int* ecritures){
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
            for(int i = 0 ; i < NBPROCESS ; i += 1) {
                close(ecritures[i]);
            }
            execl("./decalageMessage","decalageMessage",valLecture,mot,decalage,NULL);
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

    if (access(nom_fichier, F_OK) != 0) {
        printf("Le fichier %s n'existe pas.\n", nom_fichier);
        fflush(stdout);
        exit(1);
    }

    //ouverture du fichier
    int fp = open(nom_fichier,O_RDONLY);
    if (fp == -1) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(1);
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
            perror("le fichier n'est pas crypté");
            close(fp);
            exit(1);
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
        if(!(bufferInt[0]>=0 && bufferInt[0] < lseek(fp, SEEK_SET, SEEK_END))){
            perror("la taille du fichier n'est pas correcte");
            exit(1);
        }else{//sauvegarde de la valeur
            *bufferInt1 = bufferInt[0];
        }
        if(!(bufferInt[1]>=0 && bufferInt[1] < lseek(fp, SEEK_SET, SEEK_END)
        && bufferInt[0]-2*sizeof(char) -2*sizeof(int) > bufferInt[1])){
            perror("les valeurs du fichier ne sont pas correctes");
            exit(1);
        }else{//sauvegarde de la valeur
            *bufferInt2 = bufferInt[1];
        }
    }
}



/**
 * fonction qui récupère les données du programme de décryptage
 *
 * @param nom_fichier fichier sur lequel on a travaillé
 */
int recuperation_donnees_fils(char * nom_fichier){
    int status;
    int trouve = 1;
    for(int i=0; i<NBPROCESS; i++) {
        wait(&status); // Attendre la fin de l'exécution du processus fils
        if (WIFEXITED(status)) {
            if (WEXITSTATUS(status) != 0) {
                printf("FICHIER : %s\n", nom_fichier);
                fflush(stdout);
                printf("DECALAGE : %d\n", WEXITSTATUS(status));
                fflush(stdout);
                trouve = 0;
            }
        }
    }
    return trouve;
}


/**
 * méthode qui va lire le contenu du fichier et le copie dans le tube
 *
 * @param lecture mode lecture du tube
 * @param ecriture mode ecriture du tube
 * @param fp fichier
 * @param bufferInt buffer sur le descritif du fichier
 */
void lecture_message(int* ecriture, int fp, int* bufferInt){
    char buffer;
    ssize_t val;

    //déplacement de curseur dans le fichier
    lseek(fp, (__off_t) (bufferInt[1] + 2 * sizeof(int) + 2 * sizeof(char)), SEEK_SET);

    //lecture du message
    for(int i=0;  i < bufferInt[0]+1; i++){
        val = read(fp, &buffer, sizeof(char));
        if (val==-1) {
            printf("erreur dans la lecture du fichier: %zd\n",val);
            fflush(stdout);
            exit(1);
        }
        for (int j = 0; j < NBPROCESS; j++) {
            /* écriture (buffer est une suite d'un ou plusieurs octets ici de taille char)*/
            write(ecriture[j], &buffer, sizeof(buffer));
        }
    }
}


/**
 * programme principal
 *
 * @param argc nombre d'arguments
 * @param argv les arguments argv[1] nom_fichier | argv[2] mot_test | argv[3] décalage
 *
 * @return 1 si tout c'est bien passé ou -1 en cas d'erreur ou 0 si les processus fils on trouvés le mot
 */
int main(int argc, char** argv){

    //le nombre d'arguments doit être de 2
    if(argc != 3){
        perror("le nombre d'arguments doit être de 2");
        exit(1);
    }

    //distribution des arguments et déclarations
    char* nom_fichier = argv[1];
    char* mot = argv[2];
    char decalage[3];//décalage du décryptage
    int bufferInt[2];
    int ecriture[NBPROCESS];//déscripteur d'écriture des tubes
    int lecture[NBPROCESS];//déscripteur de lecture des tubes
    int tube[NBPROCESS][2];//déscripteurs des tubes

    int fp = ouverture_fichier(nom_fichier);//ouverture du fichier à manipuler

    test_fichier_crypt(fp);//vérification pour voir si le fichier est crypté

    test_donnees_fichier(fp,&bufferInt[0],&bufferInt[1]);//vérification des données du fichier

    //initialisation des tubes
    for(int i=0; i<NBPROCESS; i++) {
        if (pipe(tube[i]) == -1) { /*création d'un tube*/
            perror("création impossible du tube");
            fflush(stdout);
            exit(1);
        }
        lecture[i] = tube[i][0];
        ecriture[i] = tube[i][1];
    }

    // création des processus
    for(int i=0; i<NBPROCESS; i++){
        sprintf(decalage,"%d",i+1);
        creation_processus(lecture[i],ecriture[i],mot,decalage, ecriture);
    }

    lecture_message(ecriture,fp,bufferInt);//lecture des messages cryptés

    //fermeture des tubes d'écriture
    for (int j = 0; j < NBPROCESS; j++) {
        close(ecriture[j]);
    }

    int trouve = recuperation_donnees_fils(nom_fichier);//récupération des données des processus fils

    close(fp);//fermeture du fichier

    return trouve;
}