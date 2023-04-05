//
// Created by baptistedie on 05/04/23.
//
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char** argv){

    //vérification du nombre d'arguments
    int lecture = atoi(argv[2]);
    int ecriture = atoi(argv[1]);
    char* mot_test = argv[3];
    char tableau_test[100];

    printf("MESSAGE : ");

    char buffer;
    close(ecriture) ;
    int i=0;
    int trouve = 1;
    int new_cara;
    while(read(lecture,&buffer, sizeof(char))){
        if(buffer != '.') {
            new_cara = (buffer - 65 - atoi(argv[4])) % 27;
            if (new_cara == -1) {
                printf(" ");
                fflush(stdout);
                tableau_test[i] = '\0';
                if (strcmp(tableau_test, mot_test) == 0 && trouve == 1) {
                    trouve = 0;
                }
                printf("%c", tableau_test[i]);
                fflush(stdout);
                i = -1;
            } else if (new_cara + 65 >= 'A' && new_cara + 65 <= 'Z') {
                printf("%c", (char) (new_cara + 65));
                fflush(stdout);
                tableau_test[i] = (char) (new_cara + 65);
            }
        }else{
            printf(".");
            fflush(stdout);
            i=-1;
        }
        i++;
    }
    close(lecture) ;
    printf("\n");
    fflush(stdout);
    if(trouve==1){
        return 0;
    }
    printf("le mot est trouvé\n");
    return atoi(argv[4]);
}