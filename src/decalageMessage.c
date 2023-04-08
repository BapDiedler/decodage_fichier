//
// Created by baptistedie on 05/04/23.
//
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int afficherMessage(int ecriture, int lecture, int decalage, char* mot_test){
    printf("MESSAGE (%d): ",decalage);

    char tableau_test[100];
    char buffer;
    close(ecriture) ;
    int i=0;
    int trouve = 1;
    int new_cara;
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
                    printf(" ");
                    fflush(stdout);
                    tableau_test[i] = '\0';
                    if (strcmp(tableau_test, mot_test) == 0 && trouve == 1) {
                        trouve = 0;
                    }
                    printf("%c", tableau_test[i]);
                    fflush(stdout);
                    i = -1;
                } else if (new_cara >= 'A' && new_cara <= 'Z') {
                    printf("%c", (char) (new_cara));
                    fflush(stdout);
                    tableau_test[i] = (char) (new_cara);
                }else{
                    printf("%c",(char)((int)buffer+1));
                }
            } else {
                printf("%c",buffer);
                fflush(stdout);
                i = -1;
            }
            i++;
        }else {
            printf("%c", buffer);
        }
    }
    return trouve;
}

int main(int argc, char** argv){

    //vérification du nombre d'arguments
    int lecture = atoi(argv[2]);
    int ecriture = atoi(argv[1]);
    int decalage = atoi(argv[4]);
    char* mot_test = argv[3];

    int trouve = afficherMessage(ecriture,lecture,decalage,mot_test);

    close(lecture) ;
    printf("\n");
    fflush(stdout);
    if(trouve==1){
        return 0;
    }
    printf("le mot est trouvé\n");
    return atoi(argv[4]);
}