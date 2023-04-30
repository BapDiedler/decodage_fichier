//
// Created by baptistedie on 29/04/23.
//


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 1000

int main() {
    char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";
    int decalage;
    char message[BUFFER_SIZE];

    // Lire le décalage et le message à envoyer
    printf("Entrez le décalage : ");
    scanf("%d", &decalage);
    printf("Entrez le message à envoyer : ");
    fgets(message, BUFFER_SIZE, stdin);

    // Créer un tableau de descripteurs de tubes
    int nb_tubes = 25;
    int descr[nb_tubes][2];

    // Créer les tubes
    for (int i = 0; i < nb_tubes; i++) {
        if (pipe(descr[i]) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    // Générer les processus enfant
    for (int i = 0; i < nb_tubes; i++) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // Fermer les descripteurs de tubes inutilisés dans le processus enfant
            for (int j = 0; j < nb_tubes; j++) {
                if (j != i) {
                    close(descr[j][0]);
                    close(descr[j][1]);
                }
            }

            // Lire le message crypté à partir du tube
            char buffer[BUFFER_SIZE];
            int n = read(descr[i][0], buffer, BUFFER_SIZE);
            if (n == -1) {
                perror("read");
                exit(EXIT_FAILURE);
            }

            // Déchiffrer le message
            for (int j = 0; j < n; j++) {
                int index = -1;
                for (int k = 0; k < strlen(alphabet); k++) {
                    if (buffer[j] == alphabet[k]) {
                        index = k;
                        break;
                    }
                }
                if (index != -1) {
                    index = (index - decalage + strlen(alphabet)) % strlen(alphabet);
                    buffer[j] = alphabet[index];
                }
            }

            // Afficher le message déchiffré
            printf("Processus %d : Message déchiffré : %s\n", i+1, buffer);

            // Fermer le descripteur de tube utilisé dans le processus enfant
            close(descr[i][0]);

            exit(EXIT_SUCCESS);
        } else {
            // Fermer les descripteurs de tubes inutilisés dans le processus parent
            close(descr[i][0]);

            // Envoyer le message crypté dans le tube correspondant au processus enfant
            int n = write(descr[i][1], message, strlen(message));
            if (n == -1) {
                perror("write");
                exit(EXIT_FAILURE);
            }

            // Fermer le descripteur de tube utilisé dans le processus parent
            close(descr[i][1]);
        }
    }

    return 0;
}