//
// Created by MSI on 27/11/2024.
//
#include "header.h"
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
int tour_joueur(etatJeu *jeu, Joueur *joueur, Joueur joueurs[], int nombre_joueurs) {
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
            deplacer_joueur(jeu, joueur, joueurs, nombre_joueurs);
            break;
        } else if(choix == 2) {
            placer_mur(jeu, joueur, joueurs, nombre_joueurs);
            break;
        } else if(choix == 3) {
            printf("Tour passe.\n");
            break;
        } else if(choix == 4) {
            quitter_jeu(jeu, joueurs, nombre_joueurs, joueur); // Appel de la fonction quitter_jeu
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

// Fonction pour permettre aux joueurs de choisir leur pion
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