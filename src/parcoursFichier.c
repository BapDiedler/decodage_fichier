/**
 * Le but du programme est de faire en sorte de parcourir tout les fichier d'un répertoir pour
 * exécuter le programme de décryptage
 *
 * @author baptistedie
 */

#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

/**
 * fonction qui permet de récupérer les données de retour de chaque processus
 *
 * @param nbFichiers nombre de fichiers présents
 * @return 1 si aucun fichier n'as trouvé le mot 0 sinon
 */
int lectureDonnees(int nbFichiers){
    // Attendre la fin de l'exécution du processus fils
    int trouve = 1;
    int status;
    for(int i=0; i<nbFichiers; i++) {
        wait(&status);
        fflush(stdout);
        if (WIFEXITED(status)) {
            if (WEXITSTATUS(status) == 0) {
                trouve = 0;
            }
        }
    }
    return trouve;
}

/*MAIN PROGRAMME*/
int main(int argc, char** argv){

    //vérification du nombre d'arguments
    if(argc != 3){
        perror("le nombre d'arguments doit être de 2: le nom du répertoire et le mot à trouver\n");
        exit(1);
    }

    struct dirent * ent;
    char* fichier;
    int nbFichiers=0;

    // Ouvre le répertoire courant
    DIR* dir = opendir(argv[1]);
    if (dir == NULL) {
        printf("Impossible d'ouvrir le repertoire\n");
        exit(1);
    }

    // Parcourt le répertoire
    while ((ent = readdir(dir)) != NULL) {
        if(ent->d_type != DT_DIR) {
            nbFichiers++;
            pid_t res;
            fichier = strdup(argv[1]);
            strcat(fichier,ent->d_name);
            //création d'un processus
            switch (res = fork()) {
                case (pid_t) -1 ://erreur dans la création de processus
                    perror("erreur dans la créatiob de processus");
                    exit(1);
                case (pid_t) 0 ://programme fils
                    execl("./decrypteMessage","decrypteMessage",fichier,argv[2],NULL);
                    fflush(stdout);
                    break;
                default ://programme père
                    break;
            }
            free(fichier);//libération mémoire du strdup
        }
    }

    //on vérifie si le mot a été trouvé
    if(lectureDonnees(nbFichiers) == 1){
        perror("le programme est à sa fin mais aucun fichier ne correspond au mot cherché");
    }

    // Ferme le répertoire
    closedir(dir);

    return EXIT_SUCCESS;
}