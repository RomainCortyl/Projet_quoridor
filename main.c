#include "header.h"

int main() {
    // Initialisation du générateur de nombres aléatoires
    srand(time(NULL));
    afficher_titre();

    // Création de l'état du jeu
    etatJeu jeu;

    // Menu d'accueil
    printf("%s\nBienvenue dans le jeu Quoridor !\n\n%s", ORANGE, RESET);
    int nombre_joueurs = 0;
    Joueur joueurs[MAX_JOUEURS];
    int joueur_actuel = 0;
    char fichier_sauvegarde[] = "sauvegarde.txt";

    // Menu de choix : Nouvelle partie ou charger une partie
    printf("%s1. Nouvelle partie%s\n", JAUNE, RESET);
    printf("%s2. Charger une partie%s\n", JAUNE, RESET);

    printf("%sVotre choix : %s", ROUGE, RESET);
    int choix;
    scanf("%d", &choix);
    while (getchar() != '\n'); // Vider le tampon d'entrée

    if (choix == 2) {
        if (charger_jeu(fichier_sauvegarde, &jeu, joueurs, &nombre_joueurs, &joueur_actuel)) {
            printf("Partie chargee avec succes !\n");
        } else {
            printf("Erreur lors du chargement de la partie. Une nouvelle partie sera demarree.\n");
            choix = 1; // Si le chargement échoue, démarrer une nouvelle partie
        }
    }

    // Démarrer une nouvelle partie
    if (choix == 1) {
        // Demander le nombre de joueurs
        nombre_joueurs = demander_nombre_joueurs();

        // Permettre aux joueurs de saisir leur pseudo et de choisir leur pion
        entrer_pseudos(joueurs, nombre_joueurs);
        choisir_pions(joueurs, nombre_joueurs);

        // Nettoyer l'écran après la sélection des pions
        system(EFFACER);

        // Initialiser les joueurs
        for (int i = 0; i < nombre_joueurs; i++) {
            joueurs[i].barrieres_restantes = (nombre_joueurs == 2) ? 10 : 5;
            joueurs[i].ligne_cible = -1;   // Pas de ligne cible par défaut
            joueurs[i].colonne_cible = -1; // Pas de colonne cible par défaut
            joueurs[i].score = 0;          // Initialiser le score à zéro
        }

        // Initialiser les positions et cibles en fonction du nombre de joueurs
        if (nombre_joueurs == 2) {
            joueurs[0].x = TAILLE / 2;
            joueurs[0].y = 0;
            joueurs[0].ligne_cible = TAILLE - 1;
            joueurs[1].x = TAILLE / 2;
            joueurs[1].y = TAILLE - 1;
            joueurs[1].ligne_cible = 0;
        } else if (nombre_joueurs == 4) {
            joueurs[0].x = TAILLE / 2;
            joueurs[0].y = 0;
            joueurs[0].ligne_cible = TAILLE - 1;
            joueurs[1].x = TAILLE / 2;
            joueurs[1].y = TAILLE - 1;
            joueurs[1].ligne_cible = 0;
            joueurs[2].x = 0;
            joueurs[2].y = TAILLE / 2;
            joueurs[2].colonne_cible = TAILLE - 1;
            joueurs[3].x = TAILLE - 1;
            joueurs[3].y = TAILLE / 2;
            joueurs[3].colonne_cible = 0;
        }

        // Initialiser le plateau
        initialiser_plateau(&jeu);

        // Joueur actuel choisi au hasard
        joueur_actuel = rand() % nombre_joueurs;
    }

    // Boucle principale du jeu
    while (1) {
        system(EFFACER); // Nettoyer l'écran au début du tour
        afficher_plateau(&jeu, joueurs, nombre_joueurs);

        // Effectuer le tour du joueur
        tour_joueur(&jeu, &joueurs[joueur_actuel], joueurs, nombre_joueurs);

        // Vérifier si le joueur a gagné
        if (a_gagne(&joueurs[joueur_actuel])) {
            system(EFFACER); // Nettoyer l'écran avant d'afficher le plateau final
            afficher_plateau(&jeu, joueurs, nombre_joueurs);
            printf("%s (%c) a gagne !\n", joueurs[joueur_actuel].pseudo, joueurs[joueur_actuel].symbole);
            joueurs[joueur_actuel].score+=5;
            sauvegarder_jeu("sauvegarde.txt", &jeu, joueurs, nombre_joueurs, joueur_actuel);

            // Afficher les scores
            for (int i = 0; i < nombre_joueurs; ++i) {
                printf("Score de %s : %d\n", joueurs[i].pseudo, joueurs[i].score);
            }

            // Demander si les joueurs veulent une revanche
            printf("Voulez-vous relancer une partie ? 1.OUI / 2.NON\n");
            int revanche;
            scanf("%d", &revanche);
            while(getchar() != '\n'); // Vider le tampon d'entrée

            if(revanche == 1) {
                // Réinitialiser le plateau
                initialiser_plateau(&jeu);

                // Réinitialiser les joueurs
                for (int i = 0; i < nombre_joueurs; i++) {
                    joueurs[i].barrieres_restantes = (nombre_joueurs == 2) ? 10 : 5;
                    joueurs[i].ligne_cible = -1;
                    joueurs[i].colonne_cible = -1;
                    // Les scores peuvent être conservés ou réinitialisés selon votre choix
                }

                // Initialiser les positions et cibles en fonction du nombre de joueurs
                if (nombre_joueurs == 2) {
                    joueurs[0].x = TAILLE / 2;
                    joueurs[0].y = 0;
                    joueurs[0].ligne_cible = TAILLE - 1;
                    joueurs[1].x = TAILLE / 2;
                    joueurs[1].y = TAILLE - 1;
                    joueurs[1].ligne_cible = 0;
                } else if (nombre_joueurs == 4) {
                    joueurs[0].x = TAILLE / 2;
                    joueurs[0].y = 0;
                    joueurs[0].ligne_cible = TAILLE - 1;
                    joueurs[1].x = TAILLE / 2;
                    joueurs[1].y = TAILLE - 1;
                    joueurs[1].ligne_cible = 0;
                    joueurs[2].x = 0;
                    joueurs[2].y = TAILLE / 2;
                    joueurs[2].colonne_cible = TAILLE - 1;
                    joueurs[3].x = TAILLE - 1;
                    joueurs[3].y = TAILLE / 2;
                    joueurs[3].colonne_cible = 0;
                }

                // Choisir aléatoirement le joueur qui commence
                joueur_actuel = rand() % nombre_joueurs;
                continue; // Revenir au début de la boucle principale
            }
            else if(revanche == 2){
                printf("Merci d'avoir joué !\n");
                break; // Sortir de la boucle principale et terminer le jeu
            }
            else{
                printf("Choix invalide, veuillez réessayer.\n");
                continue; // Reposer la question
            }
        }

        // Passer au joueur suivant
        joueur_actuel = (joueur_actuel + 1) % nombre_joueurs;
    }

    return 0;
}
