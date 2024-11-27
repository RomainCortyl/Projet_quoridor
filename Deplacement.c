//
// Created by MSI on 27/11/2024.
//
#include "header.h"
// Fonction pour vérifier si le déplacement est valide
int mouvement_valide(etatJeu *jeu, Joueur *joueur, int x_dest, int y_dest, Joueur joueurs[], int nombre_joueurs) {
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
        if(dx == 1 && jeu->murs_verticaux[y_orig][x_orig + 1])
            return 0; // Mur à droite
        if(dx == -1 && jeu->murs_verticaux[y_orig][x_orig])
            return 0; // Mur à gauche
        if(dy == 1 && jeu->murs_horizontaux[y_orig + 1][x_orig])
            return 0; // Mur en haut
        if(dy == -1 && jeu->murs_horizontaux[y_orig][x_orig])
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
        if(dx == 2 && jeu->murs_verticaux[y_orig][x_orig + 1])
            return 0; // Mur à droite
        if(dx == -2 && jeu->murs_verticaux[y_orig][x_orig])
            return 0; // Mur à gauche
        if(dy == 2 && jeu->murs_horizontaux[y_orig + 1][x_orig])
            return 0; // Mur en haut
        if(dy == -2 && jeu->murs_horizontaux[y_orig][x_orig])
            return 0; // Mur en bas

        // Vérifier qu'il n'y a pas de mur derrière le pion à sauter
        if(dx == 2 && jeu->murs_verticaux[y_inter][x_inter + 1])
            return 0; // Mur derrière le pion à droite
        if(dx == -2 && jeu->murs_verticaux[y_inter][x_inter])
            return 0; // Mur derrière le pion à gauche
        if(dy == 2 && jeu->murs_horizontaux[y_inter + 1][x_inter])
            return 0; // Mur derrière le pion en haut
        if(dy == -2 && jeu->murs_horizontaux[y_inter][x_inter])
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
                if(jeu->murs_verticaux[y_orig][x_orig + 1] || jeu->murs_horizontaux[y_orig + 1][x_orig])
                    return 0; // Mur en haut à droite
            } else if(dx == -1 && dy == 1) {
                if(jeu->murs_verticaux[y_orig][x_orig] || jeu->murs_horizontaux[y_orig + 1][x_orig])
                    return 0; // Mur en haut à gauche
            } else if(dx == 1 && dy == -1) {
                if(jeu->murs_verticaux[y_orig][x_orig + 1] || jeu->murs_horizontaux[y_orig][x_orig])
                    return 0; // Mur en bas à droite
            } else if(dx == -1 && dy == -1) {
                if(jeu->murs_verticaux[y_orig][x_orig] || jeu->murs_horizontaux[y_orig][x_orig])
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
int existe_chemin(etatJeu *jeu, Joueur joueur, int ligne_cible, int colonne_cible) {
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
            if(!jeu->murs_horizontaux[y + 1][x]) {
                visite[y + 1][x] = 1;
                file_x[fin] = x;
                file_y[fin] = y + 1;
                fin++;
            }
        }
        // Bas
        if(y - 1 >= 0 && !visite[y - 1][x]) {
            if(!jeu->murs_horizontaux[y][x]) {
                visite[y - 1][x] = 1;
                file_x[fin] = x;
                file_y[fin] = y - 1;
                fin++;
            }
        }
        // Droite
        if(x + 1 < TAILLE && !visite[y][x + 1]) {
            if(!jeu->murs_verticaux[y][x + 1]) {
                visite[y][x + 1] = 1;
                file_x[fin] = x + 1;
                file_y[fin] = y;
                fin++;
            }
        }
        // Gauche
        if(x - 1 >= 0 && !visite[y][x - 1]) {
            if(!jeu->murs_verticaux[y][x]) {
                visite[y][x - 1] = 1;
                file_x[fin] = x - 1;
                file_y[fin] = y;
                fin++;
            }
        }
    }

    return 0; // Pas de chemin trouvé
}

// Fonction pour déplacer le joueur
void deplacer_joueur(etatJeu *jeu, Joueur *joueur, Joueur joueurs[], int nombre_joueurs) {
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

        if(mouvement_valide(jeu, joueur, x, y, joueurs, nombre_joueurs)) {
            joueur->x = x;
            joueur->y = y;

                if(joueur->x == 4 && joueur->y == 4 && jeu->bonus_reclame == 0) {
                    joueur->score += 2; // Ajouter 2 points bonus
                    jeu->bonus_reclame = 1; // Marquer le bonus comme réclamé
                    printf("Felicitations %s ! Vous avez atteint la case speciale et gagnez 2 points bonus !\n", joueur->pseudo);
                }
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

                    if(dx == 1 && (x_apres >= TAILLE || jeu->murs_verticaux[y_adj][x_adj + 1]))
                        mur_derriere = 1;
                    if(dx == -1 && (x_apres < 0 || jeu->murs_verticaux[y_adj][x_adj]))
                        mur_derriere = 1;
                    if(dy == 1 && (y_apres >= TAILLE || jeu->murs_horizontaux[y_adj + 1][x_adj]))
                        mur_derriere = 1;
                    if(dy == -1 && (y_apres < 0 || jeu->murs_horizontaux[y_adj][x_adj]))
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
                            if(y_adj + 1 < TAILLE && !jeu->murs_horizontaux[y_adj + 1][x_adj]) {
                                // Vérifier qu'il n'y a pas de mur vertical bloquant
                                if((dx == 1 && !jeu->murs_verticaux[y_adj][x_adj + 1]) || (dx == -1 && !jeu->murs_verticaux[y_adj][x_adj])) {
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
                            if(y_adj - 1 >= 0 && !jeu->murs_horizontaux[y_adj][x_adj]) {
                                if((dx == 1 && !jeu->murs_verticaux[y_adj - 1][x_adj + 1]) || (dx == -1 && !jeu->murs_verticaux[y_adj - 1][x_adj])) {
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
                            if(x_adj + 1 < TAILLE && !jeu->murs_verticaux[y_adj][x_adj + 1]) {
                                if((dy == 1 && !jeu->murs_horizontaux[y_adj + 1][x_adj]) || (dy == -1 && !jeu->murs_horizontaux[y_adj][x_adj])) {
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
                            if(x_adj - 1 >= 0 && !jeu->murs_verticaux[y_adj][x_adj]) {
                                if((dy == 1 && !jeu->murs_horizontaux[y_adj + 1][x_adj - 1]) || (dy == -1 && !jeu->murs_horizontaux[y_adj][x_adj - 1])) {
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
