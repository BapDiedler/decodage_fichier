//
// Created by baptistedie on 05/04/23.
//
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

/**
int afficherMessage(int lecture, int decalage, char* mot_test){
    printf("MESSAGE (%d): %d\n",getpid(),decalage);

    char tableau_test[100];
    char buffer;
    int i=0;
    int trouve = 1;
    int new_cara;
    char message[100];
    int posMessage=0;
    while(read(lecture,&buffer, sizeof(char))){
        if(decalage != 0) {
            if (buffer != '.' && buffer != ',') {
                new_cara = 'A'+(buffer - 'A' - decalage +26) % 26;
                if(new_cara > 'Z'-decalage){
                    new_cara++;
                }
                if(new_cara<'A'){
                    new_cara = 'Z'-decalage+1;
                }
                if (buffer == decalage + 'A' - 1) {
                    message[posMessage]=' ';
                    tableau_test[i] = '\0';
                    if (strcmp(tableau_test, mot_test) == 0) {
                        trouve = 0;
                    }
                    printf("test%c", tableau_test[i]);
                    fflush(stdout);
                    i = -1;
                } else if (new_cara >= 'A' && new_cara <= 'Z') {
                    message[posMessage]=new_cara;
                    tableau_test[i] = new_cara;
                }else{
                    message[posMessage]=buffer+1;
                }
            } else {
                message[posMessage]=buffer;
                i = -1;
            }
            i++;
        }else {
            message[posMessage]=buffer;
        }
        posMessage++;
    }
    message[posMessage]='\0';
    if(trouve==0){
        printf("\n%s\n",message);
    }
    return trouve;
}

int main(int argc, char** argv){

    //vérification du nombre d'arguments
    int lecture = atoi(argv[2]);
    int ecriture = atoi(argv[1]);
    int decalage = atoi(argv[4]);
    char* mot_test = argv[3];


    close(ecriture) ;

    int trouve = afficherMessage(lecture,decalage,mot_test);

    printf("\n");
    fflush(stdout);
    sleep(2);
    if(trouve==1){
        return 1;
    }
    printf("le mot est trouvé\n");
    return atoi(argv[4]);
}

*/

//autre
int afficherMessage(int lecture, int decalage, char* mot_test){
    printf("MESSAGE (%d): %d\n",getpid(),decalage);


    char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";

    char tableau_test[100];
    char buffer[100];
    int i=0;
    int trouve = 1;
    char* message = calloc(sizeof(char),100);
    int posMessage=0;
    int cpt=0;
    int nbOctets = read(lecture,&buffer, 100*sizeof(char));
    while(cpt!= strlen(buffer) && buffer[cpt]!=0) {
        if(nbOctets==0){
            close(lecture);
        }
        // Chercher l'index du caractère dans l'alphabet
        int index = -1;
        if(buffer[cpt] != '.' && buffer[cpt] != ',') {
            for (int j = 0; j < 27; j++) {
                if (buffer[cpt] == alphabet[j]) {
                    index = j;
                    break;
                }
            }
        }

        // Si le caractère est dans l'alphabet, le déchiffrer
        if (index != -1) {
            index = (index - decalage + 27) % 27;
            message[i] = alphabet[index];
            if (index == 26 ) {
                tableau_test[i] = '\0';
                if (strcmp(tableau_test, mot_test) == 0) {
                    trouve = 0;
                }
                i = -1;
            } else {
                tableau_test[i] = alphabet[index];
            }
        }
        cpt++;
        i++;
    }


    message[posMessage]='\0';
    return trouve;
}

int main(int argc, char** argv){

    //vérification du nombre d'arguments
    int lecture = atoi(argv[2]);
    int ecriture = atoi(argv[1]);
    int decalage = atoi(argv[4]);
    char* mot_test = argv[3];

    close(ecriture);

    int trouve = afficherMessage(lecture,decalage,mot_test);

    close(lecture);
    sleep(2);
    if(trouve==1){
        return 1;
    }
    printf("le mot est trouvé\n");
    return atoi(argv[4]);
}