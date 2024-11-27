//
// Created by MSI on 27/11/2024.
//
#include "header.h"

// Fonction pour vérifier si le placement du mur est valide
int mur_valide(etatJeu *jeu, int x[], int y[], Joueur joueurs[], int nombre_joueurs) {
    // Vérifier que les quatre points forment un mur valide de taille 2
    int est_horizontal = 0;
    if(y[0] == y[1] && y[2] == y[3] && x[0] == x[2] && x[1] == x[3]) {
        // Mur vertical
        est_horizontal = 0;
    } else if(x[0] == x[1] && x[2] == x[3] && y[0] == y[2] && y[1] == y[3]) {
        // Mur horizontal
        est_horizontal = 1;
    } else {
        return 0; // Pas un mur valide
    }

    // Vérifier que les positions sont adjacentes correctement
    if(est_horizontal) {
        // Mur horizontal
        if(abs(x[0] - x[2]) != 1 || abs(y[0] - y[1]) != 1)
            return 0;
    } else {
        // Mur vertical
        if(abs(y[0] - y[2]) != 1 || abs(x[0] - x[1]) != 1)
            return 0;
    }

    // Vérifier si le mur sort du plateau
    for(int i = 0; i < 4; i++) {
        if(x[i] < 0 || x[i] >= TAILLE || y[i] < 0 || y[i] >= TAILLE)
            return 0;
    }

    // Vérifier s'il y a déjà un mur à cet endroit
    if(est_horizontal) {
        int minX = (x[0] < x[2]) ? x[0] : x[2];
        int y_pos = y[0] + 1;
        if(jeu->murs_horizontaux[y_pos][minX] || jeu->murs_horizontaux[y_pos][minX + 1])
            return 0; // Mur déjà présent
    } else {
        int x_pos = x[0] + 1;
        int minY = (y[0] < y[2]) ? y[0] : y[2];
        if(jeu->murs_verticaux[minY][x_pos] || jeu->murs_verticaux[minY + 1][x_pos])
            return 0; // Mur déjà présent
    }

    // Placer temporairement le mur
    if(est_horizontal) {
        int minX = (x[0] < x[2]) ? x[0] : x[2];
        int y_pos = y[0] + 1;
        jeu->murs_horizontaux[y_pos][minX] = 1;
        jeu->murs_horizontaux[y_pos][minX + 1] = 1;
    } else {
        int x_pos = x[0] + 1;
        int minY = (y[0] < y[2]) ? y[0] : y[2];
        jeu->murs_verticaux[minY][x_pos] = 1;
        jeu->murs_verticaux[minY + 1][x_pos] = 1;
    }

    // Vérifier que chaque joueur a un chemin vers sa cible
    for(int i = 0; i < nombre_joueurs; i++) {
        if(!existe_chemin(jeu, joueurs[i], joueurs[i].ligne_cible, joueurs[i].colonne_cible)) {
            // Retirer le mur temporaire
            if(est_horizontal) {
                int minX = (x[0] < x[2]) ? x[0] : x[2];
                int y_pos = y[0] + 1;
                jeu->murs_horizontaux[y_pos][minX] = 0;
                jeu->murs_horizontaux[y_pos][minX + 1] = 0;
            } else {
                int x_pos = x[0] + 1;
                int minY = (y[0] < y[2]) ? y[0] : y[2];
                jeu->murs_verticaux[minY][x_pos] = 0;
                jeu->murs_verticaux[minY + 1][x_pos] = 0;
            }
            return 0; // Placement invalide, bloque le chemin d'un joueur
        }
    }

    // Retirer le mur temporaire (il sera placé définitivement dans la fonction placer_mur)
    if(est_horizontal) {
        int minX = (x[0] < x[2]) ? x[0] : x[2];
        int y_pos = y[0] + 1;
        jeu->murs_horizontaux[y_pos][minX] = 0;
        jeu->murs_horizontaux[y_pos][minX + 1] = 0;
    } else {
        int x_pos = x[0] + 1;
        int minY = (y[0] < y[2]) ? y[0] : y[2];
        jeu->murs_verticaux[minY][x_pos] = 0;
        jeu->murs_verticaux[minY + 1][x_pos] = 0;
    }

    return 1; // Placement valide
}

// Fonction pour placer un mur
void placer_mur(etatJeu *jeu, Joueur *joueur, Joueur joueurs[], int nombre_joueurs) {
    if(joueur->barrieres_restantes <= 0) {
        char choix;
        printf("Vous n'avez plus de barrieres.\n");
        printf("Voulez-vous deplacer une barriere existante ? (O/N) : ");
        scanf(" %c", &choix);
        while(getchar() != '\n'); // Vider le tampon d'entrée

        if(toupper(choix) == 'O') {
            deplacer_barriere(jeu, joueurs, nombre_joueurs);
        } else {
            printf("Action annulee.\n");
        }
        return;
    }
    while(1) {
        char saisie[40];
        printf("Entrez les coordonnees (ex: B3B4C3C4): ");
        fflush(stdout); // Assurer l'affichage avant la saisie
        fgets(saisie, sizeof(saisie), stdin);
        // Enlever le caractère de nouvelle ligne
        saisie[strcspn(saisie, "\n")] = '\0';

        // Vérifier la longueur de l'entrée
        if(strlen(saisie) < 8 || strlen(saisie) > 10) {
            printf("Entree invalide. Essayez a nouveau.\n");
            continue;
        }

        char coord1[3], coord2[3], coord3[3], coord4[3];
        strncpy(coord1, saisie, 2);
        coord1[2] = '\0';
        strncpy(coord2, &saisie[2], 2);
        coord2[2] = '\0';
        strncpy(coord3, &saisie[4], 2);
        coord3[2] = '\0';
        strncpy(coord4, &saisie[6], 2);
        coord4[2] = '\0';

        int x[4], y[4];
        x[0] = lettre_vers_indice(coord1[0]);
        y[0] = atoi(&coord1[1]) - 1;
        x[1] = lettre_vers_indice(coord2[0]);
        y[1] = atoi(&coord2[1]) - 1;
        x[2] = lettre_vers_indice(coord3[0]);
        y[2] = atoi(&coord3[1]) - 1;
        x[3] = lettre_vers_indice(coord4[0]);
        y[3] = atoi(&coord4[1]) - 1;

        int coords_valides = 1;
        for(int i = 0; i < 4; i++) {
            if(x[i] == -1 || y[i] < 0 || y[i] >= TAILLE) {
                coords_valides = 0;
                break;
            }
        }
        if(!coords_valides) {
            printf("Coordonnees invalides. Essayez a nouveau.\n");
            continue;
        }

        if(mur_valide(jeu, x, y, joueurs, nombre_joueurs)) {
            // Placer le mur définitivement
            if(y[0] == y[1]) { // Mur vertical
                int x_pos = x[0] + 1;
                int minY = (y[0] < y[2]) ? y[0] : y[2];
                jeu->murs_verticaux[minY][x_pos] = 1;
                jeu->murs_verticaux[minY + 1][x_pos] = 1;
            } else if(x[0] == x[1]) { // Mur horizontal
                int minX = (x[0] < x[2]) ? x[0] : x[2];
                int y_pos = y[0] + 1;
                jeu->murs_horizontaux[y_pos][minX] = 1;
                jeu->murs_horizontaux[y_pos][minX + 1] = 1;
            }
            joueur->barrieres_restantes--;
            break;
        } else {
            printf("Placement invalide. Soit le mur est deja present, soit il bloque le chemin d'un joueur. Essayez a nouveau.\n");
        }
    }
}

// Fonction pour déplacer une barrière existante
void deplacer_barriere(etatJeu *jeu, Joueur joueurs[], int nombre_joueurs) {
    while(1) {
        char saisie[40];
        printf("Entrez les coordonnees de la barriere a deplacer (ex: B3B4C3C4): ");
        fgets(saisie, sizeof(saisie), stdin);
        // Enlever le caractère de nouvelle ligne
        saisie[strcspn(saisie, "\n")] = '\0';

        // Vérifier la longueur de l'entrée
        if(strlen(saisie) < 8 || strlen(saisie) > 10) {
            printf("Entree invalide. Essayez a nouveau.\n");
            continue;
        }

        int x[4], y[4];
        if(!parser_coordonnees_barriere(saisie, x, y)) {
            printf("Coordonnees invalides. Essayez a nouveau.\n");
            continue;
        }

        // Vérifier si une barrière existe à ces coordonnées
        if(!barriere_existe(jeu, x, y)) {
            printf("Aucune barriere n'existe a ces coordonnees. Essayez a nouveau.\n");
            continue;
        }

        // Retirer temporairement la barrière
        retirer_barriere(jeu, x, y);

        // Demander les nouvelles coordonnées
        printf("Entrez les nouvelles coordonnees pour placer la barriere (ex: D5D6E5E6): ");
        fgets(saisie, sizeof(saisie), stdin);
        // Enlever le caractère de nouvelle ligne
        saisie[strcspn(saisie, "\n")] = '\0';

        // Vérifier la longueur de l'entrée
        if(strlen(saisie) < 8 || strlen(saisie) > 10) {
            printf("Entree invalide. La barriere n'a pas ete déplacee.\n");
            // Remettre la barrière à sa position initiale
            placer_barriere_aux(jeu, x, y);
            return;
        }

        int x_new[4], y_new[4];
        if(!parser_coordonnees_barriere(saisie, x_new, y_new)) {
            printf("Coordonnees invalides. La barriere n'a pas ete deplacee.\n");
            // Remettre la barrière à sa position initiale
            placer_barriere_aux(jeu, x, y);
            return;
        }

        // Vérifier si le placement est valide
        if(mur_valide(jeu, x_new, y_new, joueurs, nombre_joueurs)) {
            // Placer la barrière à la nouvelle position
            placer_barriere_aux(jeu, x_new, y_new);
            printf("La barriere a ete deplacee avec succes.\n");
            break;
        } else {
            printf("Placement invalide. La barriere n'a pas ete deplacee.\n");
            // Remettre la barrière à sa position initiale
            placer_barriere_aux(jeu, x, y);
            return;
        }
    }
}

// Fonction pour parser les coordonnées de la barrière
int parser_coordonnees_barriere(char *saisie, int x[], int y[]) {
    if(strlen(saisie) < 8 || strlen(saisie) > 10)
        return 0;

    char coord1[3], coord2[3], coord3[3], coord4[3];
    strncpy(coord1, saisie, 2);
    coord1[2] = '\0';
    strncpy(coord2, &saisie[2], 2);
    coord2[2] = '\0';
    strncpy(coord3, &saisie[4], 2);
    coord3[2] = '\0';
    strncpy(coord4, &saisie[6], 2);
    coord4[2] = '\0';

    x[0] = lettre_vers_indice(coord1[0]);
    y[0] = atoi(&coord1[1]) - 1;
    x[1] = lettre_vers_indice(coord2[0]);
    y[1] = atoi(&coord2[1]) - 1;
    x[2] = lettre_vers_indice(coord3[0]);
    y[2] = atoi(&coord3[1]) - 1;
    x[3] = lettre_vers_indice(coord4[0]);
    y[3] = atoi(&coord4[1]) - 1;

    for(int i = 0; i < 4; i++) {
        if(x[i] == -1 || y[i] < 0 || y[i] >= TAILLE)
            return 0;
    }

    return 1;
}

// Fonction pour vérifier si une barrière existe
int barriere_existe(etatJeu *jeu, int x[], int y[]) {
    int est_horizontal = 0;
    if(y[0] == y[1] && y[2] == y[3] && x[0] == x[2] && x[1] == x[3]) {
        // Barrière verticale
        est_horizontal = 0;
    } else if(x[0] == x[1] && x[2] == x[3] && y[0] == y[2] && y[1] == y[3]) {
        // Barrière horizontale
        est_horizontal = 1;
    } else {
        return 0; // Pas une barrière valide
    }

    if(est_horizontal) {
        int minX = (x[0] < x[2]) ? x[0] : x[2];
        int y_pos = y[0] + 1;
        if(jeu->murs_horizontaux[y_pos][minX] && jeu->murs_horizontaux[y_pos][minX + 1])
            return 1; // Barrière horizontale existe
    } else {
        int x_pos = x[0] + 1;
        int minY = (y[0] < y[2]) ? y[0] : y[2];
        if(jeu->murs_verticaux[minY][x_pos] && jeu->murs_verticaux[minY + 1][x_pos])
            return 1; // Barrière verticale existe
    }

    return 0; // Aucune barrière à ces coordonnées
}

// Fonction pour placer une barrière aux coordonnées spécifiées
void placer_barriere_aux(etatJeu *jeu, int x[], int y[]) {
    int est_horizontal = 0;
    if(y[0] == y[1]) {
        est_horizontal = 0;
    } else if(x[0] == x[1]) {
        est_horizontal = 1;
    }

    if(est_horizontal) {
        int minX = (x[0] < x[2]) ? x[0] : x[2];
        int y_pos = y[0] + 1;
        jeu->murs_horizontaux[y_pos][minX] = 1;
        jeu->murs_horizontaux[y_pos][minX + 1] = 1;
    } else {
        int x_pos = x[0] + 1;
        int minY = (y[0] < y[2]) ? y[0] : y[2];
        jeu->murs_verticaux[minY][x_pos] = 1;
        jeu->murs_verticaux[minY + 1][x_pos] = 1;
    }
}



// Fonction pour retirer une barrière
void retirer_barriere(etatJeu *jeu, int x[], int y[]) {
    int est_horizontal = 0;
    if(y[0] == y[1]) {
        est_horizontal = 0;
    } else if(x[0] == x[1]) {
        est_horizontal = 1;
    }

    if(est_horizontal) {
        int minX = (x[0] < x[2]) ? x[0] : x[2];
        int y_pos = y[0] + 1;
        jeu->murs_horizontaux[y_pos][minX] = 0;
        jeu->murs_horizontaux[y_pos][minX + 1] = 0;
    } else {
        int x_pos = x[0] + 1;
        int minY = (y[0] < y[2]) ? y[0] : y[2];
        jeu->murs_verticaux[minY][x_pos] = 0;
        jeu->murs_verticaux[minY + 1][x_pos] = 0;
    }
}

