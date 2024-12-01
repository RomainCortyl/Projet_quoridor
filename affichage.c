//
// Created by MSI on 27/11/2024.
//
#include "header.h"

// Fonction pour initialiser le plateau
void initialiser_plateau(etatJeu *jeu) {
    for(int i = 0; i <= TAILLE; i++)
        for(int j = 0; j <= TAILLE; j++) {
            if(i < TAILLE && j < TAILLE)
                jeu->plateau[i][j] = ' ';
            if(i < TAILLE+1 && j < TAILLE)
                jeu->murs_horizontaux[i][j] = 0;
            if(i < TAILLE && j < TAILLE+1)
                jeu->murs_verticaux[i][j] = 0;
        }
    jeu->bonus_reclame = 0; // Le bonus n'a pas encore été réclamé
}

// Fonction pour afficher le plateau avec un alignement correct des lettres
void afficher_plateau(etatJeu *jeu, Joueur joueurs[], int nombre_joueurs) {
    // Ne pas nettoyer l'écran ici pour éviter de supprimer les messages précédents

    // Affichage des coordonnées x (lettres) centrées au-dessus des cases
    printf("   "); // Alignement avec l'indice de ligne
    for(int j = 0; j < TAILLE; j++) {
        printf("  %c   ", indice_vers_lettre(j)); // Total de LARGEUR_COLONNE (6) caractères par colonne
    }
    printf("\n");

    for(int i = TAILLE - 1; i >= 0; i--) {
        printf("   "); // Alignement avec l'indice de ligne
        for(int j = 0; j < TAILLE; j++) {
            printf("+");
            if(jeu->murs_horizontaux[i+1][j])
                printf("%s-----" RESET, ROUGE); // Mur horizontal en rouge
            else
                printf("     "); // Espace vide
        }
        printf("+\n");

        // Affichage de l'indice y (chiffres)
        printf(" %2d ", i + 1);

        // Affichage des murs verticaux et des cases
        for(int j = 0; j < TAILLE; j++) {
            // Mur vertical à gauche de la case
            if(jeu->murs_verticaux[i][j])
                printf("%s|%s", ROUGE, RESET); // Mur vertical en rouge
            else
                printf(" ");

            // Affichage du pion ou de l'espace
            int est_joueur = 0;
            for(int p = 0; p < nombre_joueurs; p++) {
                if(joueurs[p].x == j && joueurs[p].y == i) {
                    printf(" %s %c %s ", joueurs[p].couleur, joueurs[p].symbole, RESET); // Afficher le pion en couleur
                    est_joueur = 1;
                    break;
                }
            }
            if(!est_joueur)
                if(j == 4 && i == 4 && jeu->bonus_reclame == 0) {
                    // Afficher la case spéciale
                    printf(" %s B %s ", MAGENTA, RESET); // Utilisez une couleur pour indiquer le bonus
                } else {
                    printf("     "); // Espace vide de 5 caractères
                }
            }

            // Mur vertical à droite de la dernière case
        if(jeu->murs_verticaux[i][TAILLE])
            printf("|");
        else
            printf(" ");

        printf("\n");
    }

    // Affichage de la dernière ligne de murs horizontaux
    printf("   "); // Alignement avec l'indice de ligne
    for(int j = 0; j < TAILLE; j++) {
        printf("+");
        if(jeu->murs_horizontaux[0][j])
            printf("-----");
        else
            printf("     ");
    }
    printf("+\n");

}

void afficher_scores(Joueur joueurs[], int nombre_joueurs) {
    printf("\nScores actuels :\n");
    for(int i = 0; i < nombre_joueurs; i++) {
        printf("%s%s%s : %d points\n", joueurs[i].couleur, joueurs[i].pseudo, RESET, joueurs[i].score);
    }
    printf("\n");
}

// Fonction pour afficher le titre "QUORIDOR"
void afficher_titre() {
    printf(CLEAR_SCREEN);
    printf(CYAN "\n\n");
    printf("  ____  __  ______  ___  _______  ____  ___ \n");
    printf(" / __ \\/ / / / __ \\/ _ \\/  _/ _ \/ __ \\/ _ \\\n");
    printf("/ /_/ / /_/ / /_/ / , _// // // / /_/ / , _/\n");
    printf("\\___\\_\\____/\\____/_/|_/___/____/\\____/_/|_| \n\n" RESET);
}

