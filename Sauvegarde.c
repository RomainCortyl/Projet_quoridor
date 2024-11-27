//
// Created by MSI on 27/11/2024.
//
#include "header.h"

// Fonction pour sauvegarder le jeu
void sauvegarder_jeu(const char *nom_fichier, etatJeu *jeu, Joueur joueurs[], int nombre_joueurs, int joueur_actuel) {
    FILE *fichier = fopen(nom_fichier, "w");
    if (!fichier) {
        perror("Erreur lors de l'ouverture du fichier de sauvegarde");
        return;
    }

    // Sauvegarder le nombre de joueurs
    fprintf(fichier, "%d\n", nombre_joueurs);

    // Sauvegarder les joueurs
    for (int i = 0; i < nombre_joueurs; i++) {
        // Obtenir le nom de la couleur
        const char *nom_couleur = "";
        if (joueurs[i].couleur == ROUGE)
            nom_couleur = "ROUGE";
        else if (joueurs[i].couleur == VERT)
            nom_couleur = "VERT";
        else if (joueurs[i].couleur == JAUNE)
            nom_couleur = "JAUNE";
        else if (joueurs[i].couleur == BLEU)
            nom_couleur = "BLEU";
        else if (joueurs[i].couleur == MAGENTA)
            nom_couleur = "MAGENTA";
        else if (joueurs[i].couleur == CYAN)
            nom_couleur = "CYAN";
        else
            nom_couleur = "RESET"; // Par défaut

        fprintf(fichier, "%s %c %d %d %d %d %d %d %s\n",
                joueurs[i].pseudo,               // Pseudo du joueur
                joueurs[i].symbole,              // Symbole du joueur
                joueurs[i].x,                    // Coordonnée x
                joueurs[i].y,                    // Coordonnée y
                joueurs[i].barrieres_restantes,  // Barrières restantes
                joueurs[i].ligne_cible,          // Ligne cible
                joueurs[i].colonne_cible,        // Colonne cible
                joueurs[i].score,                // Score du joueur
                nom_couleur);                    // Nom de la couleur
    }

    // Sauvegarder les murs placés : format h: x y ou v: x y pour chaque mur
    for (int i = 0; i < TAILLE + 1; i++) {
        for (int j = 0; j < TAILLE; j++) {
            if (jeu->murs_horizontaux[i][j] == 1) {
                fprintf(fichier, "h: %d %d\n", i, j);  // Mur horizontal
            }
        }
    }

    for (int i = 0; i < TAILLE; i++) {
        for (int j = 0; j < TAILLE + 1; j++) {
            if (jeu->murs_verticaux[i][j] == 1) {
                fprintf(fichier, "v: %d %d\n", i, j);  // Mur vertical
            }
        }
    }

// Ajouter un marqueur de fin pour les murs
    fprintf(fichier, "END_WALLS\n");

    // Sauvegarder le joueur actuel (index du joueur en cours)
    fprintf(fichier, "%d\n", joueur_actuel);

    // Sauvegarder l'état du bonus_reclame
    fprintf(fichier, "bonus_reclame: %d\n", jeu->bonus_reclame);

    fclose(fichier);
    printf("Jeu sauvegarde dans %s.\n", nom_fichier);
}

// Fonction pour charger le jeu
int charger_jeu(const char *nom_fichier, etatJeu *jeu, Joueur joueurs[], int *nombre_joueurs, int *joueur_actuel) {
    FILE *fichier = fopen(nom_fichier, "r");
    if (!fichier) {
        perror("Erreur lors de l'ouverture du fichier de sauvegarde");
        return 0;
    }

    // Charger le nombre de joueurs
    fscanf(fichier, "%d\n", nombre_joueurs);

    // Charger les joueurs
    for (int i = 0; i < *nombre_joueurs; i++) {
        char pseudo[50];
        char symbole;
        int x, y, barrieres_restantes, ligne_cible, colonne_cible, score;
        char nom_couleur[20];

        fscanf(fichier, "%s %c %d %d %d %d %d %d %s\n",
               pseudo,
               &symbole,
               &x,
               &y,
               &barrieres_restantes,
               &ligne_cible,
               &colonne_cible,
               &score,
               nom_couleur);

        // Restaurer les informations du joueur
        strcpy(joueurs[i].pseudo, pseudo);
        joueurs[i].symbole = symbole;
        joueurs[i].x = x;
        joueurs[i].y = y;
        joueurs[i].barrieres_restantes = barrieres_restantes;
        joueurs[i].ligne_cible = ligne_cible;
        joueurs[i].colonne_cible = colonne_cible;
        joueurs[i].score = score;

        joueurs[0].couleur=ROUGE;
        joueurs[1].couleur=VERT;
        joueurs[2].couleur=JAUNE;
        joueurs[3].couleur=BLEU;
    }

    // Initialiser les matrices des murs
    memset(jeu->murs_horizontaux, 0, sizeof(jeu->murs_horizontaux));
    memset(jeu->murs_verticaux, 0, sizeof(jeu->murs_verticaux));

    // Charger les barrières horizontales (h:) et verticales (v:)
    char line[256];
    while (fgets(line, sizeof(line), fichier)) {
        if (strcmp(line, "END_WALLS\n") == 0) {
            break; // Fin de la lecture des murs
        }
    }
    char type;
    int x, y;
    while (fscanf(fichier, "%c: %d %d\n", &type, &x, &y) == 3) {
        if (type == 'h') {
            jeu->murs_horizontaux[x][y] = 1; // Mur horizontal
        } else if (type == 'v') {
            jeu->murs_verticaux[x][y] = 1; // Mur vertical
        }
    }

    // Charger le joueur actuel
    fscanf(fichier, "%d\n", joueur_actuel);

// Charger l'état du bonus_reclame
    fscanf(fichier, "bonus_reclame: %d\n", &(jeu->bonus_reclame));

    fclose(fichier);
    printf("Jeu charge depuis %s.\n", nom_fichier);
    return 1;
}

// Fonction pour quitter le jeu
void quitter_jeu(etatJeu *jeu, Joueur joueurs[], int nombre_joueurs, Joueur *joueur) {
    char choix;

    printf("\nLe joueur %s (%c) souhaite quitter la partie.\n", joueur->pseudo, joueur->symbole);
    printf("Voulez-vous sauvegarder la partie avant de quitter ? (O/N) : ");
    scanf(" %c", &choix);

    if (toupper(choix) == 'O') {
        // Sauvegarder la partie
        sauvegarder_jeu("sauvegarde.txt", jeu, joueurs, nombre_joueurs, joueur - joueurs); // Calcul de l'indice du joueur
        printf("Partie sauvegardee avec succes !\n");
    }

    printf("Merci d'avoir joue ! La partie est terminee.\n");
    exit(0); // Quitte le programme
}
