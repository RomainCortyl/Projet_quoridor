//
// Created by rom1c on 18/11/2024.
//

#ifndef QORIDORV1_HEADER_H
#define QORIDORV1_HEADER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define TAILLE 9
#define LARGEUR_COLONNE 6 // Largeur constante pour chaque colonne
#define MAX_JOUEURS 4     // Nombre maximum de joueurs
#ifdef _WIN32
#define CLEAR_SCREEN "cls"
#else
#define CLEAR_SCREEN "\x1b[2J\x1b[H"
#endif

#ifdef _WIN32
#define EFFACER "cls"
#define RESET   "\x1b[0m"
#define ROUGE   "\x1b[31m"
#define ORANGE  "\x1b[38;5;208m"
#define VERT    "\x1b[32m"
#define JAUNE   "\x1b[33m"
#define BLEU    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#else
#endif

#define PION1 254
#define PION2 169
#define PION3 207
#define PION4 184
#define PION5 158
#define PION6 189


typedef struct {
    char pseudo[50]; // Pseudo du joueur
    int symbole;     // Changement de char à int
    int x;
    int y;
    int barrieres_restantes;
    int ligne_cible; // Pour les joueurs se déplaçant verticalement
    int colonne_cible; // Pour les joueurs se déplaçant horizontalement
    int score;       // Score du joueur
    const char *couleur; // Code de couleur ANSI du joueur
} Joueur;


char plateau[TAILLE][TAILLE]; // Plateau de jeu
int murs_horizontaux[TAILLE+1][TAILLE]; // Murs horizontaux
int murs_verticaux[TAILLE][TAILLE+1];   // Murs verticaux


void afficher_plateau(Joueur joueurs[], int nombre_joueurs);
int lettre_vers_indice(char lettre);
char indice_vers_lettre(int indice);
void initialiser_plateau();
void sauvegarder_jeu(const char *nom_fichier, Joueur joueurs[], int nombre_joueurs, int joueur_actuel);
int mouvement_valide(Joueur *joueur, int x, int y, Joueur joueurs[], int nombre_joueurs);
void deplacer_joueur(Joueur *joueur, Joueur joueurs[], int nombre_joueurs);
int existe_chemin(Joueur joueur, int ligne_cible, int colonne_cible);
int mur_valide(int x[], int y[], Joueur joueurs[], int nombre_joueurs);
void placer_mur(Joueur *joueur, Joueur joueurs[], int nombre_joueurs);
int a_gagne(Joueur *joueur);
int tour_joueur(Joueur *joueur, Joueur joueurs[], int nombre_joueurs);
void entrer_pseudos(Joueur joueurs[], int nombre_joueurs);
void choisir_pions(Joueur joueurs[], int nombre_joueurs);
int demander_nombre_joueurs();
int charger_jeu(const char *nom_fichier, Joueur joueurs[], int *nombre_joueurs, int *joueur_actuel);
void quitter_jeu(Joueur joueurs[], int nombre_joueurs, Joueur *joueur);
void placer_barriere_aux(int x[], int y[]);
void deplacer_barriere(Joueur joueurs[], int nombre_joueurs);
int parser_coordonnees_barriere(char *saisie, int x[], int y[]);
int barriere_existe(int x[], int y[]);
void retirer_barriere(int x[], int y[]);
void afficher_titre();


// Fonction pour convertir une lettre en indice de colonne
int lettre_vers_indice(char lettre) {
    lettre = toupper(lettre);
    if(lettre >= 'A' && lettre < 'A' + TAILLE)
        return lettre - 'A';
    else
        return -1; // Valeur invalide
}

// Fonction pour convertir un indice de colonne en lettre
char indice_vers_lettre(int indice) {
    if(indice >= 0 && indice < TAILLE)
        return 'A' + indice;
    else
        return '?';
}

// Fonction pour initialiser le plateau
void initialiser_plateau() {
    for(int i = 0; i <= TAILLE; i++)
        for(int j = 0; j <= TAILLE; j++) {
            if(i < TAILLE && j < TAILLE)
                plateau[i][j] = ' ';
            if(i < TAILLE+1 && j < TAILLE)
                murs_horizontaux[i][j] = 0;
            if(i < TAILLE && j < TAILLE+1)
                murs_verticaux[i][j] = 0;
        }
}

// Fonction pour afficher le plateau avec un alignement correct des lettres
void afficher_plateau(Joueur joueurs[], int nombre_joueurs) {
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
            if(murs_horizontaux[i+1][j])
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
            if(murs_verticaux[i][j])
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
                printf("     "); // Espace vide de 5 caractères

        }
        // Mur vertical à droite de la dernière case
        if(murs_verticaux[i][TAILLE])
            printf("|");
        else
            printf(" ");

        printf("\n");
    }

    // Affichage de la dernière ligne de murs horizontaux
    printf("   "); // Alignement avec l'indice de ligne
    for(int j = 0; j < TAILLE; j++) {
        printf("+");
        if(murs_horizontaux[0][j])
            printf("-----");
        else
            printf("     ");
    }
    printf("+\n");

}


// Fonction pour vérifier si le déplacement est valide
int mouvement_valide(Joueur *joueur, int x_dest, int y_dest, Joueur joueurs[], int nombre_joueurs) {
    int x_orig = joueur->x;
    int y_orig = joueur->y;

    // Vérifier si la case de destination est dans le plateau
    if(x_dest < 0 || x_dest >= TAILLE || y_dest < 0 || y_dest >= TAILLE)
        return 0; // Déplacement invalide

    // Calculer la différence
    int dx = x_dest - x_orig;
    int dy = y_dest - y_orig;

    // Vérifier si le déplacement est d'une case
    if(abs(dx) + abs(dy) == 1) {
        // Déplacement orthogonal d'une case
        // Vérifier s'il y a un mur entre les deux cases
        if(dx == 1 && murs_verticaux[y_orig][x_orig + 1])
            return 0; // Mur à droite
        if(dx == -1 && murs_verticaux[y_orig][x_orig])
            return 0; // Mur à gauche
        if(dy == 1 && murs_horizontaux[y_orig + 1][x_orig])
            return 0; // Mur en haut
        if(dy == -1 && murs_horizontaux[y_orig][x_orig])
            return 0; // Mur en bas

        // Vérifier si la case de destination est occupée
        for(int i = 0; i < nombre_joueurs; i++) {
            if(joueurs[i].x == x_dest && joueurs[i].y == y_dest) {
                // Case occupée par un autre joueur
                return 0; // Déplacement invalide
            }
        }

        return 1; // Déplacement valide
    }

    // Vérifier si c'est un saut par-dessus un pion
    if(abs(dx) + abs(dy) == 2 && (abs(dx) == 2 || abs(dy) == 2)) {
        int x_inter = x_orig + dx / 2;
        int y_inter = y_orig + dy / 2;

        // Vérifier si un joueur est sur la case intermédiaire
        int pion_present = 0;
        for(int i = 0; i < nombre_joueurs; i++) {
            if(joueurs[i].x == x_inter && joueurs[i].y == y_inter) {
                pion_present = 1;
                break;
            }
        }
        if(!pion_present)
            return 0; // Pas de pion à sauter

        // Vérifier les murs pour le saut
        if(dx == 2 && murs_verticaux[y_orig][x_orig + 1])
            return 0; // Mur à droite
        if(dx == -2 && murs_verticaux[y_orig][x_orig])
            return 0; // Mur à gauche
        if(dy == 2 && murs_horizontaux[y_orig + 1][x_orig])
            return 0; // Mur en haut
        if(dy == -2 && murs_horizontaux[y_orig][x_orig])
            return 0; // Mur en bas

        // Vérifier qu'il n'y a pas de mur derrière le pion à sauter
        if(dx == 2 && murs_verticaux[y_inter][x_inter + 1])
            return 0; // Mur derrière le pion à droite
        if(dx == -2 && murs_verticaux[y_inter][x_inter])
            return 0; // Mur derrière le pion à gauche
        if(dy == 2 && murs_horizontaux[y_inter + 1][x_inter])
            return 0; // Mur derrière le pion en haut
        if(dy == -2 && murs_horizontaux[y_inter][x_inter])
            return 0; // Mur derrière le pion en bas

        // Vérifier que la case de destination est libre
        for(int i = 0; i < nombre_joueurs; i++) {
            if(joueurs[i].x == x_dest && joueurs[i].y == y_dest) {
                return 0; // Case destination occupée
            }
        }

        return 1; // Saut valide
    }

    // Vérifier les mouvements en diagonale
    if(abs(dx) == 1 && abs(dy) == 1) {
        // Vérifier s'il y a un pion adjacent dans la direction orthogonale
        int x_inter = x_orig + dx;
        int y_inter = y_orig;
        int pion_present_x = 0;
        for(int i = 0; i < nombre_joueurs; i++) {
            if(joueurs[i].x == x_inter && joueurs[i].y == y_inter) {
                pion_present_x = 1;
                break;
            }
        }

        x_inter = x_orig;
        y_inter = y_orig + dy;
        int pion_present_y = 0;
        for(int i = 0; i < nombre_joueurs; i++) {
            if(joueurs[i].x == x_inter && joueurs[i].y == y_inter) {
                pion_present_y = 1;
                break;
            }
        }

        if(pion_present_x || pion_present_y) {
            // Vérifier les murs pour le déplacement en diagonale
            if(dx == 1 && dy == 1) {
                if(murs_verticaux[y_orig][x_orig + 1] || murs_horizontaux[y_orig + 1][x_orig])
                    return 0; // Mur en haut à droite
            } else if(dx == -1 && dy == 1) {
                if(murs_verticaux[y_orig][x_orig] || murs_horizontaux[y_orig + 1][x_orig])
                    return 0; // Mur en haut à gauche
            } else if(dx == 1 && dy == -1) {
                if(murs_verticaux[y_orig][x_orig + 1] || murs_horizontaux[y_orig][x_orig])
                    return 0; // Mur en bas à droite
            } else if(dx == -1 && dy == -1) {
                if(murs_verticaux[y_orig][x_orig] || murs_horizontaux[y_orig][x_orig])
                    return 0; // Mur en bas à gauche
            }

            // Vérifier que la case de destination est libre
            for(int i = 0; i < nombre_joueurs; i++) {
                if(joueurs[i].x == x_dest && joueurs[i].y == y_dest) {
                    return 0; // Case destination occupée
                }
            }

            return 1; // Déplacement en diagonale valide
        }
    }

    return 0; // Déplacement invalide
}


// Fonction pour vérifier si un joueur a un chemin vers sa cible
int existe_chemin(Joueur joueur, int ligne_cible, int colonne_cible) {
    int visite[TAILLE][TAILLE] = {0}; // Tableau pour marquer les cases visitées
    int file_x[TAILLE * TAILLE]; // File pour BFS (positions x)
    int file_y[TAILLE * TAILLE]; // File pour BFS (positions y)
    int debut = 0, fin = 0;

    // Ajouter la position initiale du joueur à la file
    file_x[fin] = joueur.x;
    file_y[fin] = joueur.y;
    fin++;
    visite[joueur.y][joueur.x] = 1;

    while(debut < fin) {
        int x = file_x[debut];
        int y = file_y[debut];
        debut++;

        // Vérifier si on a atteint la ligne ou colonne cible
        if((ligne_cible != -1 && y == ligne_cible) || (colonne_cible != -1 && x == colonne_cible))
            return 1; // Chemin trouvé

        // Explorer les mouvements possibles
        // Haut
        if(y + 1 < TAILLE && !visite[y + 1][x]) {
            // Vérifier les murs
            if(!murs_horizontaux[y + 1][x]) {
                visite[y + 1][x] = 1;
                file_x[fin] = x;
                file_y[fin] = y + 1;
                fin++;
            }
        }
        // Bas
        if(y - 1 >= 0 && !visite[y - 1][x]) {
            if(!murs_horizontaux[y][x]) {
                visite[y - 1][x] = 1;
                file_x[fin] = x;
                file_y[fin] = y - 1;
                fin++;
            }
        }
        // Droite
        if(x + 1 < TAILLE && !visite[y][x + 1]) {
            if(!murs_verticaux[y][x + 1]) {
                visite[y][x + 1] = 1;
                file_x[fin] = x + 1;
                file_y[fin] = y;
                fin++;
            }
        }
        // Gauche
        if(x - 1 >= 0 && !visite[y][x - 1]) {
            if(!murs_verticaux[y][x]) {
                visite[y][x - 1] = 1;
                file_x[fin] = x - 1;
                file_y[fin] = y;
                fin++;
            }
        }
    }

    return 0; // Pas de chemin trouvé
}

/// Fonction pour vérifier si le placement du mur est valide
int mur_valide(int x[], int y[], Joueur joueurs[], int nombre_joueurs) {
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
        if(murs_horizontaux[y_pos][minX] || murs_horizontaux[y_pos][minX + 1])
            return 0; // Mur déjà présent
    } else {
        int x_pos = x[0] + 1;
        int minY = (y[0] < y[2]) ? y[0] : y[2];
        if(murs_verticaux[minY][x_pos] || murs_verticaux[minY + 1][x_pos])
            return 0; // Mur déjà présent
    }

    // Placer temporairement le mur
    if(est_horizontal) {
        int minX = (x[0] < x[2]) ? x[0] : x[2];
        int y_pos = y[0] + 1;
        murs_horizontaux[y_pos][minX] = 1;
        murs_horizontaux[y_pos][minX + 1] = 1;
    } else {
        int x_pos = x[0] + 1;
        int minY = (y[0] < y[2]) ? y[0] : y[2];
        murs_verticaux[minY][x_pos] = 1;
        murs_verticaux[minY + 1][x_pos] = 1;
    }

    // Vérifier que chaque joueur a un chemin vers sa cible
    for(int i = 0; i < nombre_joueurs; i++) {
        if(!existe_chemin(joueurs[i], joueurs[i].ligne_cible, joueurs[i].colonne_cible)) {
            // Retirer le mur temporaire
            if(est_horizontal) {
                int minX = (x[0] < x[2]) ? x[0] : x[2];
                int y_pos = y[0] + 1;
                murs_horizontaux[y_pos][minX] = 0;
                murs_horizontaux[y_pos][minX + 1] = 0;
            } else {
                int x_pos = x[0] + 1;
                int minY = (y[0] < y[2]) ? y[0] : y[2];
                murs_verticaux[minY][x_pos] = 0;
                murs_verticaux[minY + 1][x_pos] = 0;
            }
            return 0; // Placement invalide, bloque le chemin d'un joueur
        }
    }

    // Retirer le mur temporaire (il sera placé définitivement dans la fonction placer_mur)
    if(est_horizontal) {
        int minX = (x[0] < x[2]) ? x[0] : x[2];
        int y_pos = y[0] + 1;
        murs_horizontaux[y_pos][minX] = 0;
        murs_horizontaux[y_pos][minX + 1] = 0;
    } else {
        int x_pos = x[0] + 1;
        int minY = (y[0] < y[2]) ? y[0] : y[2];
        murs_verticaux[minY][x_pos] = 0;
        murs_verticaux[minY + 1][x_pos] = 0;
    }

    return 1; // Placement valide
}

// Fonction pour placer un mur
void placer_mur(Joueur *joueur, Joueur joueurs[], int nombre_joueurs) {
    if(joueur->barrieres_restantes <= 0) {
        char choix;
        printf("Vous n'avez plus de barrieres.\n");
        printf("Voulez-vous deplacer une barriere existante ? (O/N) : ");
        scanf(" %c", &choix);
        while(getchar() != '\n'); // Vider le tampon d'entrée

        if(toupper(choix) == 'O') {
            deplacer_barriere(joueurs, nombre_joueurs);
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

        if(mur_valide(x, y, joueurs, nombre_joueurs)) {
            // Placer le mur définitivement
            if(y[0] == y[1]) { // Mur vertical
                int x_pos = x[0] + 1;
                int minY = (y[0] < y[2]) ? y[0] : y[2];
                murs_verticaux[minY][x_pos] = 1;
                murs_verticaux[minY + 1][x_pos] = 1;
            } else if(x[0] == x[1]) { // Mur horizontal
                int minX = (x[0] < x[2]) ? x[0] : x[2];
                int y_pos = y[0] + 1;
                murs_horizontaux[y_pos][minX] = 1;
                murs_horizontaux[y_pos][minX + 1] = 1;
            }
            joueur->barrieres_restantes--;
            break;
        } else {
            printf("Placement invalide. Soit le mur est deja present, soit il bloque le chemin d'un joueur. Essayez a nouveau.\n");
        }
    }
}

void deplacer_barriere(Joueur joueurs[], int nombre_joueurs) {
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
        if(!barriere_existe(x, y)) {
            printf("Aucune barriere n'existe a ces coordonnees. Essayez a nouveau.\n");
            continue;
        }

        // Retirer temporairement la barrière
        retirer_barriere(x, y);

        // Demander les nouvelles coordonnées
        printf("Entrez les nouvelles coordonnees pour placer la barriere (ex: D5D6E5E6): ");
        fgets(saisie, sizeof(saisie), stdin);
        // Enlever le caractère de nouvelle ligne
        saisie[strcspn(saisie, "\n")] = '\0';

        // Vérifier la longueur de l'entrée
        if(strlen(saisie) < 8 || strlen(saisie) > 10) {
            printf("Entree invalide. La barriere n'a pas ete déplacee.\n");
            // Remettre la barrière à sa position initiale
            placer_barriere_aux(x, y);
            return;
        }

        int x_new[4], y_new[4];
        if(!parser_coordonnees_barriere(saisie, x_new, y_new)) {
            printf("Coordonnees invalides. La barriere n'a pas ete deplacee.\n");
            // Remettre la barrière à sa position initiale
            placer_barriere_aux(x, y);
            return;
        }

        // Vérifier si le placement est valide
        if(mur_valide(x_new, y_new, joueurs, nombre_joueurs)) {
            // Placer la barrière à la nouvelle position
            placer_barriere_aux(x_new, y_new);
            printf("La barriere a ete deplacee avec succes.\n");
            break;
        } else {
            printf("Placement invalide. La barriere n'a pas ete deplacee.\n");
            // Remettre la barrière à sa position initiale
            placer_barriere_aux(x, y);
            return;
        }
    }
}
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
int barriere_existe(int x[], int y[]) {
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
        if(murs_horizontaux[y_pos][minX] && murs_horizontaux[y_pos][minX + 1])
            return 1; // Barrière horizontale existe
    } else {
        int x_pos = x[0] + 1;
        int minY = (y[0] < y[2]) ? y[0] : y[2];
        if(murs_verticaux[minY][x_pos] && murs_verticaux[minY + 1][x_pos])
            return 1; // Barrière verticale existe
    }

    return 0; // Aucune barrière à ces coordonnées
}
void retirer_barriere(int x[], int y[]) {
    int est_horizontal = 0;
    if(y[0] == y[1]) {
        est_horizontal = 0;
    } else if(x[0] == x[1]) {
        est_horizontal = 1;
    }

    if(est_horizontal) {
        int minX = (x[0] < x[2]) ? x[0] : x[2];
        int y_pos = y[0] + 1;
        murs_horizontaux[y_pos][minX] = 0;
        murs_horizontaux[y_pos][minX + 1] = 0;
    } else {
        int x_pos = x[0] + 1;
        int minY = (y[0] < y[2]) ? y[0] : y[2];
        murs_verticaux[minY][x_pos] = 0;
        murs_verticaux[minY + 1][x_pos] = 0;
    }
}
void placer_barriere_aux(int x[], int y[]) {
    int est_horizontal = 0;
    if(y[0] == y[1]) {
        est_horizontal = 0;
    } else if(x[0] == x[1]) {
        est_horizontal = 1;
    }

    if(est_horizontal) {
        int minX = (x[0] < x[2]) ? x[0] : x[2];
        int y_pos = y[0] + 1;
        murs_horizontaux[y_pos][minX] = 1;
        murs_horizontaux[y_pos][minX + 1] = 1;
    } else {
        int x_pos = x[0] + 1;
        int minY = (y[0] < y[2]) ? y[0] : y[2];
        murs_verticaux[minY][x_pos] = 1;
        murs_verticaux[minY + 1][x_pos] = 1;
    }
}


// Fonction pour vérifier si un joueur a gagné
int a_gagne(Joueur *joueur) {
    // Pour les joueurs se déplaçant verticalement
    if(joueur->ligne_cible != -1 && joueur->y == joueur->ligne_cible)
        return 1;
    // Pour les joueurs se déplaçant horizontalement
    if(joueur->colonne_cible != -1 && joueur->x == joueur->colonne_cible)
        return 1;
    return 0;
}

// Fonction pour gérer le tour d'un joueur
int tour_joueur(Joueur *joueur, Joueur joueurs[], int nombre_joueurs) {
    while(1) {
        printf("C'est au tour de %s (%s%c%s).\n", joueur->pseudo, joueur->couleur, joueur->symbole, RESET);
        printf("Barrieres restantes : %d\n", joueur->barrieres_restantes);
        printf("Choisissez une action :\n");
        printf("1. Deplacer\n");
        printf("2. Placer une barriere\n");
        printf("3. Passer\n");
        printf("4. Quitter\n");
        printf("Votre choix : ");

        int choix;
        if(scanf("%d", &choix) != 1) {
            printf("Entree invalide. Essayez a nouveau.\n");
            while(getchar() != '\n'); // Vider le tampon d'entrée
            continue;
        }
        while(getchar() != '\n'); // Vider le tampon d'entrée

        if(choix == 1) {
            deplacer_joueur(joueur, joueurs, nombre_joueurs);
            break;
        } else if(choix == 2) {
            placer_mur(joueur, joueurs, nombre_joueurs);
            break;
        } else if(choix == 3) {
            printf("Tour passe.\n");
            break;
        } else if(choix == 4) {
            quitter_jeu(joueurs, nombre_joueurs, joueurs); // Appel de la fonction quitter_jeu
            //printf("%s (%c) a quitte le jeu.\n", joueur->pseudo, joueur->symbole);
            //exit(0); // Quitter le programme
        } else {
            printf("Choix invalide. Essayez a nouveau.\n");
        }
    }
    return 0;
}

// Fonction pour permettre aux joueurs de saisir leur pseudo
void entrer_pseudos(Joueur joueurs[], int nombre_joueurs) {
    for(int i = 0; i < nombre_joueurs; i++) {
        printf("Joueur %d, entrez votre pseudo : \n", i + 1);
        fgets(joueurs[i].pseudo, sizeof(joueurs[i].pseudo), stdin);
        // Enlever le caractère de nouvelle ligne
        joueurs[i].pseudo[strcspn(joueurs[i].pseudo, "\n")] = '\0';
    }
}

void choisir_pions(Joueur joueurs[], int nombre_joueurs) {
    int pions_disponibles[] = {PION1, PION2, PION3, PION4, PION5, PION6};
    const char *couleurs_disponibles[] = {ROUGE, VERT, JAUNE, BLEU, MAGENTA, CYAN};
    int nombre_pions = sizeof(pions_disponibles) / sizeof(pions_disponibles[0]);
    int nombre_couleurs = sizeof(couleurs_disponibles) / sizeof(couleurs_disponibles[0]);

    for(int i = 0; i < nombre_joueurs; i++) {
        // Attribuer une couleur disponible au joueur
        if (i < nombre_couleurs) {
            joueurs[i].couleur = couleurs_disponibles[i];
        } else {
            joueurs[i].couleur = couleurs_disponibles[i % nombre_couleurs];
        }

        while(1) {
            printf("%s, choisissez votre pion en entrant un chiffre :\n", joueurs[i].pseudo);
            printf("Pions disponibles :\n");

            int mapping[nombre_pions];  // Pour mapper le numéro de choix à l'indice réel
            int index_choix = 0;

            for(int j = 0; j < nombre_pions; j++) {
                int deja_choisi = 0;
                // Vérifier si le pion est déjà pris
                for(int k = 0; k < i; k++) {
                    if(joueurs[k].symbole == pions_disponibles[j]) {
                        deja_choisi = 1;
                        break;
                    }
                }
                if(!deja_choisi) {
                    mapping[index_choix] = j; // Mappe le numéro de choix à l'indice réel
                    printf("%d. %s%c%s\n", index_choix + 1, joueurs[i].couleur, pions_disponibles[j], RESET);
                    index_choix++;
                }
            }

            if(index_choix == 0) {
                printf("Plus de pions disponibles.\n");
                exit(1); // Terminer le programme ou gérer autrement
            }

            printf("Votre choix : ");
            char saisie[10];
            fgets(saisie, sizeof(saisie), stdin);
            // Enlever le caractère de nouvelle ligne
            saisie[strcspn(saisie, "\n")] = '\0';

            // Vérifier que l'entrée est un nombre valide
            int choix = atoi(saisie);
            if(choix < 1 || choix > index_choix) {
                printf("Entree invalide. Veuillez entrer un chiffre correspondant a un pion disponible.\n");
                continue;
            }

            // Obtenir l'indice réel du pion choisi
            int indice_pion = mapping[choix - 1];
            joueurs[i].symbole = pions_disponibles[indice_pion];
            printf("Pion '%s%c%s' attribue a %s.\n", joueurs[i].couleur, joueurs[i].symbole, RESET, joueurs[i].pseudo);
            break;
        }
    }
}



// Fonction pour demander le nombre de joueurs
int demander_nombre_joueurs() {
    int nombre_joueurs;
    while(1) {
        printf("Combien de joueurs vont jouer ? (2 ou 4) : ");
        if(scanf("%d", &nombre_joueurs) != 1) {
            printf("Entree invalide. Veuillez entrer un nombre.\n");
           while(getchar() != '\n'); // Vider le tampon d'entrée
            continue;
        }
        while(getchar() != '\n'); // Vider le tampon d'entrée
        if(nombre_joueurs == 2 || nombre_joueurs == 4) {
            break;
        } else {
            printf("Nombre invalide. Veuillez entrer 2 ou 4.\n");
        }
    }
    return nombre_joueurs;
}

void sauvegarder_jeu(const char *nom_fichier, Joueur joueurs[], int nombre_joueurs, int joueur_actuel) {
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
            if (murs_horizontaux[i][j] == 1) {
                fprintf(fichier, "h: %d %d\n", i, j);  // Mur horizontal
            }
        }
    }

    for (int i = 0; i < TAILLE; i++) {
        for (int j = 0; j < TAILLE + 1; j++) {
            if (murs_verticaux[i][j] == 1) {
                fprintf(fichier, "v: %d %d\n", i, j);  // Mur vertical
            }
        }
    }

    // Sauvegarder le joueur actuel (index du joueur en cours)
    fprintf(fichier, "%d\n", joueur_actuel);

    fclose(fichier);
    printf("Jeu sauvegarde dans %s.\n", nom_fichier);
}



int charger_jeu(const char *nom_fichier, Joueur joueurs[], int *nombre_joueurs, int *joueur_actuel) {
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

        // Restaurer la couleur en fonction du nom
        if (strcmp(nom_couleur, "ROUGE") == 0)
            joueurs[i].couleur = ROUGE;
        else if (strcmp(nom_couleur, "VERT") == 0)
            joueurs[i].couleur = VERT;
        else if (strcmp(nom_couleur, "JAUNE") == 0)
            joueurs[i].couleur = JAUNE;
        else if (strcmp(nom_couleur, "BLEU") == 0)
            joueurs[i].couleur = BLEU;
        else if (strcmp(nom_couleur, "MAGENTA") == 0)
            joueurs[i].couleur = MAGENTA;
        else if (strcmp(nom_couleur, "CYAN") == 0)
            joueurs[i].couleur = CYAN;
        else
            joueurs[i].couleur = RESET; // Par défaut
    }

    // Initialiser les matrices des murs
    memset(murs_horizontaux, 0, sizeof(murs_horizontaux));
    memset(murs_verticaux, 0, sizeof(murs_verticaux));

    // Charger les barrières horizontales (h:) et verticales (v:)
    char type;
    int x, y;
    while (fscanf(fichier, "%c: %d %d\n", &type, &x, &y) == 3) {
        if (type == 'h') {
            murs_horizontaux[x][y] = 1; // Mur horizontal
        } else if (type == 'v') {
            murs_verticaux[x][y] = 1; // Mur vertical
        }
    }

    // Charger le joueur actuel
    fscanf(fichier, "%d\n", joueur_actuel);

    fclose(fichier);
    printf("Jeu charge depuis %s.\n", nom_fichier);
    return 1;
}



void quitter_jeu(Joueur joueurs[], int nombre_joueurs, Joueur *joueur) {
    char choix;

    printf("\nLe joueur %s (%c) souhaite quitter la partie.\n", joueur->pseudo, joueur->symbole);
    printf("Voulez-vous sauvegarder la partie avant de quitter ? (O/N) : ");
    scanf(" %c", &choix);

    if (toupper(choix) == 'O') {
        // Sauvegarder la partie
        sauvegarder_jeu("sauvegarde.txt", joueurs, nombre_joueurs, joueur - joueurs); // Calcul de l'indice du joueur
        printf("Partie sauvegardee avec succes !\n");
    }

    printf("Merci d'avoir joue ! La partie est terminee.\n");
    exit(0); // Quitte le programme
}

// Fonction pour déplacer le joueur
void deplacer_joueur(Joueur *joueur, Joueur joueurs[], int nombre_joueurs) {
    while(1) {
        char saisie[10];
        printf("Entrez les coordonnees (ex: B3): ");
        fgets(saisie, sizeof(saisie), stdin);
        // Enlever le caractère de nouvelle ligne
        saisie[strcspn(saisie, "\n")] = '\0';

        // Vérifier la longueur de l'entrée
        if(strlen(saisie) < 2 || strlen(saisie) > 3) {
            printf("Coordonnees invalides. Essayez a nouveau.\n");
            continue;
        }

        // Parser l'entrée utilisateur
        int x = lettre_vers_indice(saisie[0]);
        int y = atoi(&saisie[1]) - 1; // Les indices internes commencent à 0

        if(x == -1 || y < 0 || y >= TAILLE) {
            printf("Coordonnees invalides. Essayez a nouveau.\n");
            continue;
        }

        if(mouvement_valide(joueur, x, y, joueurs, nombre_joueurs)) {
            joueur->x = x;
            joueur->y = y;
            break;
        } else {
            printf("Deplacement invalide.\n");

            // Vérifier s'il s'agit d'un cas spécial où le joueur est bloqué par un pion
            int dx = x - joueur->x;
            int dy = y - joueur->y;

            if(abs(dx) + abs(dy) == 1) {
                int x_adj = joueur->x + dx;
                int y_adj = joueur->y + dy;

                // Vérifier si la case adjacente est occupée par un autre joueur
                int pion_present = 0;
                for(int i = 0; i < nombre_joueurs; i++) {
                    if(joueurs[i].x == x_adj && joueurs[i].y == y_adj) {
                        pion_present = 1;
                        break;
                    }
                }

                if(pion_present) {
                    // Vérifier s'il y a un mur derrière le pion adjacent
                    int x_apres = x_adj + dx;
                    int y_apres = y_adj + dy;
                    int mur_derriere = 0;

                    if(dx == 1 && (x_apres >= TAILLE || murs_verticaux[y_adj][x_adj + 1]))
                        mur_derriere = 1;
                    if(dx == -1 && (x_apres < 0 || murs_verticaux[y_adj][x_adj]))
                        mur_derriere = 1;
                    if(dy == 1 && (y_apres >= TAILLE || murs_horizontaux[y_adj + 1][x_adj]))
                        mur_derriere = 1;
                    if(dy == -1 && (y_apres < 0 || murs_horizontaux[y_adj][x_adj]))
                        mur_derriere = 1;

                    if(mur_derriere) {
                        // Proposer les déplacements en diagonale
                        int options = 0;
                        int x_option1 = joueur->x;
                        int y_option1 = joueur->y;
                        int x_option2 = joueur->x;
                        int y_option2 = joueur->y;

                        if(dx != 0) { // Déplacement horizontal initial
                            // Vérifier diagonale en haut
                            if(y_adj + 1 < TAILLE && !murs_horizontaux[y_adj + 1][x_adj]) {
                                // Vérifier qu'il n'y a pas de mur vertical bloquant
                                if((dx == 1 && !murs_verticaux[y_adj][x_adj + 1]) || (dx == -1 && !murs_verticaux[y_adj][x_adj])) {
                                    // Vérifier que la case est libre
                                    int libre = 1;
                                    for(int i = 0; i < nombre_joueurs; i++) {
                                        if(joueurs[i].x == x_adj && joueurs[i].y == y_adj + 1) {
                                            libre = 0;
                                            break;
                                        }
                                    }
                                    if(libre) {
                                        x_option1 = x_adj;
                                        y_option1 = y_adj + 1;
                                        options++;
                                    }
                                }
                            }
                            // Vérifier diagonale en bas
                            if(y_adj - 1 >= 0 && !murs_horizontaux[y_adj][x_adj]) {
                                if((dx == 1 && !murs_verticaux[y_adj - 1][x_adj + 1]) || (dx == -1 && !murs_verticaux[y_adj - 1][x_adj])) {
                                    int libre = 1;
                                    for(int i = 0; i < nombre_joueurs; i++) {
                                        if(joueurs[i].x == x_adj && joueurs[i].y == y_adj - 1) {
                                            libre = 0;
                                            break;
                                        }
                                    }
                                    if(libre) {
                                        if(options == 0) {
                                            x_option1 = x_adj;
                                            y_option1 = y_adj - 1;
                                        } else {
                                            x_option2 = x_adj;
                                            y_option2 = y_adj - 1;
                                        }
                                        options++;
                                    }
                                }
                            }
                        } else if(dy != 0) { // Déplacement vertical initial
                            // Vérifier diagonale à droite
                            if(x_adj + 1 < TAILLE && !murs_verticaux[y_adj][x_adj + 1]) {
                                if((dy == 1 && !murs_horizontaux[y_adj + 1][x_adj]) || (dy == -1 && !murs_horizontaux[y_adj][x_adj])) {
                                    int libre = 1;
                                    for(int i = 0; i < nombre_joueurs; i++) {
                                        if(joueurs[i].x == x_adj + 1 && joueurs[i].y == y_adj) {
                                            libre = 0;
                                            break;
                                        }
                                    }
                                    if(libre) {
                                        x_option1 = x_adj + 1;
                                        y_option1 = y_adj;
                                        options++;
                                    }
                                }
                            }
                            // Vérifier diagonale à gauche
                            if(x_adj - 1 >= 0 && !murs_verticaux[y_adj][x_adj]) {
                                if((dy == 1 && !murs_horizontaux[y_adj + 1][x_adj - 1]) || (dy == -1 && !murs_horizontaux[y_adj][x_adj - 1])) {
                                    int libre = 1;
                                    for(int i = 0; i < nombre_joueurs; i++) {
                                        if(joueurs[i].x == x_adj - 1 && joueurs[i].y == y_adj) {
                                            libre = 0;
                                            break;
                                        }
                                    }
                                    if(libre) {
                                        if(options == 0) {
                                            x_option1 = x_adj - 1;
                                            y_option1 = y_adj;
                                        } else {
                                            x_option2 = x_adj - 1;
                                            y_option2 = y_adj;
                                        }
                                        options++;
                                    }
                                }
                            }
                        }

                        if(options > 0) {
                            printf("Vous ne pouvez pas sauter par-dessus le pion, mais vous pouvez vous déplacer en diagonale.\n");
                            printf("Choisissez votre déplacement :\n");
                            if(options >= 1)
                                printf("1. %c%d\n", indice_vers_lettre(x_option1), y_option1 + 1);
                            if(options == 2)
                                printf("2. %c%d\n", indice_vers_lettre(x_option2), y_option2 + 1);
                            printf("Votre choix : ");
                            int choix;
                            scanf("%d", &choix);
                            while(getchar() != '\n'); // Vider le tampon

                            if(choix == 1) {
                                joueur->x = x_option1;
                                joueur->y = y_option1;
                                break;
                            } else if(choix == 2 && options == 2) {
                                joueur->x = x_option2;
                                joueur->y = y_option2;
                                break;
                            } else {
                                printf("Choix invalide. Reessayez.\n");
                            }
                        }
                    }
                }
            }
        }
    }
}

// Fonction pour afficher un grand titre "QUORIDOR" au début avec ASCII art
void afficher_titre() {
    printf(CLEAR_SCREEN);
    printf(CYAN "\n\n");
    printf("       ____  _    _  ____  _____  _____  ____   ____  _____  \n");
    printf("      / __ \\| |  | |/ __ \\|  __ \\|_   _|  __ \\ / __ \\|  __ \\ \n");
    printf("     | |  | | |  | | |  | | |__) | | | | |  | | |  | | |__) |\n");
    printf("     | |  | | |  | | |  | |  _  /  | | | |  | | |  | |  _  / \n");
    printf("     | |__| | |__| | |__| | | \\ \\ _| |_| |__| | |__| | | \\ \\ \n");
    printf("      \\____/ \\____/ \\____/|_|  \\_\\_____|_____/ \\____/|_|  \\_\\ \n\n\n" RESET);
}







#endif //QORIDORV1_HEADER_H
