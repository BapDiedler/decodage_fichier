//
// Created by baptistedie on 28/04/23.
//

/**
 * Le but du programme est de faire en sorte de parcourir tout les fichier d'un répertoir pour
 * exécuter le programme de décryptage
 */

#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>


int main(int argc, char** argv){

    if(argc != 3){
        perror("le nombre d'argument doit être de 2: le nom du répertoire et le mot à trouver\n");
        exit(1);
    }
    DIR *dir;
    int status;
    struct dirent *ent;

    // Ouvre le répertoire courant
    dir = opendir(argv[1]);
    if (dir == NULL) {
        printf("Impossible d'ouvrir le repertoire\n");
        exit(1);
    }
    int trouve;
    char* mot = argv[2];
    char* fichier;
    // Parcourt le répertoire
    while ((ent = readdir(dir)) != NULL) {
        if(ent->d_type != DT_DIR) {
            pid_t res;
            fichier = strcat(argv[1],ent->d_name);
            //création d'un processus
            switch (res = fork()) {
                case (pid_t) -1 ://erreur dans la création de processus
                    perror("erreur dans la créatiob de processus");
                    exit(1);
                case (pid_t) 0 ://programme fils
                    execl("./decrypteMessage","decrypteMessage",fichier,mot,NULL);
                    fflush(stdout);
                    break;
                default ://programme père
                    break;
            }

            wait(&status); // Attendre la fin de l'exécution du processus fils
            fflush(stdout);
            if (WIFEXITED(status)) {
                if (WEXITSTATUS(status) == 0) {
                    trouve = 1;
                }
            }

            // Affiche le nom de chaque fichier
            //printf("%s\n", ent->d_name);
        }
    }

    if(trouve != 1){
        perror("le programme est à sa fin mais aucun fichier ne correspond au mot cherché");
    }



    // Ferme le répertoire
    closedir(dir);


    return EXIT_SUCCESS;
}