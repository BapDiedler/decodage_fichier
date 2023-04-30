//
// Created by baptistedie on 05/04/23.
//
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>


/**
 * fonction qui permet de retrouver l'indice correspondant après décalage
 *
 * @param buffer caractère que l'on décale
 * @return l'indice du nouveau caractère
 */
int chercheIndex(char buffer){
    char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";
    char* valIndex;
    int index = -1;
    if(buffer != '.' && buffer != ',') {
        valIndex = strchr(alphabet,buffer);
        if(valIndex != NULL){
            index = valIndex - alphabet;
        }
    }
    return index;
}

/**
 * fonction qui permet de décrypter le message en lisant dans le tube
 *
 * @param lecture descripteur de lecture du tube
 * @param decalage décalage du cryptage
 * @param mot_test mot à tester
 * @return 1 si le mot n'est pas trouvé 0 sinon
 */
int decryptMessage(int lecture, int decalage, char* mot_test){

    char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";
    char message[150];
    char tableau_test[150];
    char buffer;
    int i=0;
    int trouve = 1;
    int posMessage=0;
    int val;

    do{
        buffer='_';
        val = read(lecture,&buffer, sizeof(char));
        if(decalage != 0 && buffer != '+') {
            // Chercher l'index du caractère dans l'alphabet
            int index = chercheIndex(buffer);

            // Si le caractère est dans l'alphabet, le déchiffrer
            if (index != -1) {
                //trouve = dechiffrement(message,tableau_test,mot_test,posMessage,decalage,index,i);
                index = (index - decalage + 27) % 27;
                message[posMessage] = alphabet[index];
                if (index == 26) {
                    tableau_test[i] = '\0';
                    if (strcmp(tableau_test, mot_test) == 0) {
                        trouve = 0;
                    }
                    i = -1;
                }else{
                    tableau_test[i] = message[posMessage];
                }
            }else{
                message[posMessage]=buffer;
            }
        }

        if(buffer == '+'){
            close(lecture);
            val = 0;
        }
        i++;
        posMessage++;
    } while (val != 0 && val != -1);
    if(trouve==0) {
        message[posMessage - 1] = '\0';
        printf("%s\n",message);
        fflush(stdout);
    }
    return trouve;
}

/*MAIN PROGRAMME*/
int main(int argc, char** argv){

    //vérification du nombre d'arguments
    int lecture = atoi(argv[2]);
    int ecriture = atoi(argv[1]);
    int decalage = atoi(argv[4]);
    char* mot_test = argv[3];

    close(ecriture);

    int trouve = decryptMessage(lecture, decalage, mot_test);

    sleep(1);
    if(trouve==1){
        return 1;
    }
    return atoi(argv[4]);
}