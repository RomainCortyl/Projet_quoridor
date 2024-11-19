
#include "header.h"

int main() {
    // Initialisation du générateur de nombres aléatoires
    srand(time(NULL));

    // Menu d'accueil
    printf("\nBienvenue dans le jeu Quoridor !\n\n");
    int nombre_joueurs = 0;
    Joueur joueurs[MAX_JOUEURS];
    int joueur_actuel = 0;
    char fichier_sauvegarde[] = "sauvegarde.txt";

    // Menu de choix : Nouvelle partie ou charger une partie
    printf("1. Nouvelle partie\n");
    printf("2. Charger une partie\n");
    printf("Votre choix : ");
    int choix;
    scanf("%d", &choix);
    while (getchar() != '\n'); // Vider le tampon d'entrée

    if (choix == 2) {
        if (charger_jeu(fichier_sauvegarde, joueurs, &nombre_joueurs, &joueur_actuel)) {
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
        while (getchar() != '\n'); // Vider le tampon d'entrée

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
        initialiser_plateau();

        // Joueur actuel choisi au hasard
        joueur_actuel = rand() % nombre_joueurs;
    }



    // Boucle principale du jeu
    while (1) {
        system(EFFACER); // Nettoyer l'écran au début du tour
        afficher_plateau(joueurs, nombre_joueurs);

        // Effectuer le tour du joueur
        tour_joueur(&joueurs[joueur_actuel], joueurs, nombre_joueurs);

        // Vérifier si le joueur a gagné
        if (a_gagne(&joueurs[joueur_actuel])) {
            system(EFFACER); // Nettoyer l'écran avant d'afficher le plateau final
            afficher_plateau(joueurs, nombre_joueurs);
            printf("%s (%c) a gagné !\n", joueurs[joueur_actuel].pseudo, joueurs[joueur_actuel].symbole);
            // Sauvegarder la partie à la fin
            sauvegarder_jeu(fichier_sauvegarde, joueurs, nombre_joueurs, joueur_actuel);
            break;
        }

        // Passer au joueur suivant
        system(EFFACER);
        joueur_actuel = (joueur_actuel + 1) % nombre_joueurs;
    }

    return 0;
}