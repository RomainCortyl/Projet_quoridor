
#include "header.h"

int main() {

    // Menu d'accueil
    printf("\nBienvenue dans le jeu Quoridor !\n\n");
    int nombre_joueurs = demander_nombre_joueurs();
    Joueur joueurs[MAX_JOUEURS];

    // Vider le tampon d'entrée après scanf dans demander_nombre_joueurs
    // pour éviter les problèmes avec fgets
    //while(getchar() != '\n'); // Déjà fait dans demander_nombre_joueurs

    // Permettre aux joueurs de saisir leur pseudo
    entrer_pseudos(joueurs, nombre_joueurs);

    // Permettre aux joueurs de choisir leur pion
    choisir_pions(joueurs, nombre_joueurs);

    // Nettoyer l'écran après la sélection des pions
    system(EFFACER);

    // Initialisation des joueurs
    // Initialisation commune
    for(int i = 0; i < nombre_joueurs; i++) {
        joueurs[i].barrieres_restantes = (nombre_joueurs == 2) ? 10 : 5;
        joueurs[i].ligne_cible = -1;   // Par défaut, pas de ligne cible
        joueurs[i].colonne_cible = -1; // Par défaut, pas de colonne cible
    }

    // Positions initiales et cibles en fonction du nombre de joueurs
    if(nombre_joueurs == 2) {
        // Joueur 1
        joueurs[0].x = TAILLE / 2; // Milieu du plateau en x
        joueurs[0].y = 0;          // Bas du plateau
        joueurs[0].ligne_cible = TAILLE - 1; // Ligne cible en haut

        // Joueur 2
        joueurs[1].x = TAILLE / 2; // Milieu du plateau en x
        joueurs[1].y = TAILLE - 1; // Haut du plateau
        joueurs[1].ligne_cible = 0; // Ligne cible en bas

    } else if(nombre_joueurs == 4) {
        // Joueur 1 (Bas)
        joueurs[0].x = TAILLE / 2;
        joueurs[0].y = 0;
        joueurs[0].ligne_cible = TAILLE - 1;

        // Joueur 2 (Haut)
        joueurs[1].x = TAILLE / 2;
        joueurs[1].y = TAILLE - 1;
        joueurs[1].ligne_cible = 0;

        // Joueur 3 (Gauche)
        joueurs[2].x = 0;
        joueurs[2].y = TAILLE / 2;
        joueurs[2].colonne_cible = TAILLE - 1;

        // Joueur 4 (Droite)
        joueurs[3].x = TAILLE - 1;
        joueurs[3].y = TAILLE / 2;
        joueurs[3].colonne_cible = 0;
    }

    initialiser_plateau();

    int joueur_actuel = 0;

    // Boucle principale du jeu
    while(1) {
        system(EFFACER); // Nettoyer l'écran au début du tour
        afficher_plateau(joueurs, nombre_joueurs);

        tour_joueur(&joueurs[joueur_actuel], joueurs, nombre_joueurs);

        // Vérifier si le joueur a gagné
        if(a_gagne(&joueurs[joueur_actuel])) {
            system(EFFACER); // Nettoyer l'écran avant d'afficher le plateau final
            afficher_plateau(joueurs, nombre_joueurs);
            printf("%s (%c) a gagne !\n", joueurs[joueur_actuel].pseudo, joueurs[joueur_actuel].symbole);
            break;
        }

        // Passer au joueur suivant
        system(EFFACER);
        joueur_actuel = (joueur_actuel + 1) % nombre_joueurs;
    }

    return 0;
}