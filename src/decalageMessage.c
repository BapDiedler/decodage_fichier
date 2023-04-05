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
    char* mot_test = "TEST";
    char tableau_test[100];


    char buffer;
    close(ecriture) ;
    int i=0;
    int trouve = 1;
    while(read(lecture,&buffer, sizeof(char))){
        if(buffer=='B'){
            printf(" ");
            tableau_test[i]= '\0';
            if(strcmp(tableau_test,mot_test)!=0 && trouve){
                trouve=0;
                i=0;
            }
        }else if(buffer >= 'A' && buffer <= 'Z'){
            printf("%c", buffer - 2);
            tableau_test[i]= buffer - 2;
        }else{
            printf("%c", buffer);
            tableau_test[i]= buffer;
        }
        i++;
    }
    if(!trouve){
        printf("\n le message est trouvé\n");
    }
    close(lecture) ;

    return EXIT_SUCCESS;
}