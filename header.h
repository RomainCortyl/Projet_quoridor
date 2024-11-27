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
#include <windows.h> // Pour Sleep() sous Windows
#include <conio.h>


#define TAILLE 9
#define LARGEUR_COLONNE 6 // Largeur constante pour chaque colonne
#define MAX_JOUEURS 4     // Nombre maximum de joueurs

#ifdef _WIN32
#define CLEAR_SCREEN ""
#define EFFACER ""
#else
#define CLEAR_SCREEN "\x1b[2J\x1b[H"
#define EFFACER "clear"
#endif

#define RESET   "\x1b[0m"
#define ROUGE   "\x1b[31m"
#define ORANGE  "\x1b[38;5;208m"
#define VERT    "\x1b[32m"
#define JAUNE   "\x1b[33m"
#define BLEU    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"

#define PION1 254
#define PION2 169
#define PION3 207
#define PION4 184
#define PION5 158
#define PION6 189

#define TEMPS_MAX 30

typedef struct {
    char pseudo[50]; // Pseudo du joueur
    int symbole;     // Changement de char à int
    int x;
    int y;
    int barrieres_restantes;
    int ligne_cible;   // Pour les joueurs se déplaçant verticalement
    int colonne_cible; // Pour les joueurs se déplaçant horizontalement
    int score;         // Score du joueur
    const char *couleur; // Code de couleur ANSI du joueur
    double temps_total;
} Joueur;

// Structure pour l'état du jeu
typedef struct {
    char plateau[TAILLE][TAILLE]; // Plateau de jeu
    int murs_horizontaux[TAILLE+1][TAILLE]; // Murs horizontaux
    int murs_verticaux[TAILLE][TAILLE+1];   // Murs verticaux
    int bonus_reclame; // 0 si le bonus n'est pas réclamé, 1 si réclamé
} etatJeu;

// Suppression des variables globales (déjà incluses dans etatJeu)
// char plateau[TAILLE][TAILLE]; // Supprimé
// int murs_horizontaux[TAILLE+1][TAILLE]; // Supprimé
// int murs_verticaux[TAILLE][TAILLE+1];   // Supprimé

// Prototypes des fonctions avec etat
//Jeu *jeu ajouté
void afficher_plateau(etatJeu *jeu, Joueur joueurs[], int nombre_joueurs);
void afficher_titre();
void initialiser_plateau(etatJeu *jeu);                                         //affichage.c
void afficher_scores(Joueur joueurs[], int nombre_joueurs);
int lettre_vers_indice(char lettre);
char indice_vers_lettre(int indice);                                  //conversion.c
void quitter_jeu(etatJeu *jeu, Joueur joueurs[], int nombre_joueurs, Joueur *joueur);
void sauvegarder_jeu(const char *nom_fichier, etatJeu *jeu, Joueur joueurs[], int nombre_joueurs, int joueur_actuel);
int charger_jeu(const char *nom_fichier, etatJeu *jeu, Joueur joueurs[], int *nombre_joueurs, int *joueur_actuel);            //Sauvegarde.c
int mouvement_valide(etatJeu *jeu, Joueur *joueur, int x, int y, Joueur joueurs[], int nombre_joueurs);
int existe_chemin(etatJeu *jeu, Joueur joueur, int ligne_cible, int colonne_cible);
void deplacer_joueur(etatJeu *jeu, Joueur *joueur, Joueur joueurs[], int nombre_joueurs);           //Deplacement.c
int mur_valide(etatJeu *jeu, int x[], int y[], Joueur joueurs[], int nombre_joueurs);
void placer_mur(etatJeu *jeu, Joueur *joueur, Joueur joueurs[], int nombre_joueurs);
void deplacer_barriere(etatJeu *jeu, Joueur joueurs[], int nombre_joueurs);
int parser_coordonnees_barriere(char *saisie, int x[], int y[]);
int barriere_existe(etatJeu *jeu, int x[], int y[]);
void placer_barriere_aux(etatJeu *jeu, int x[], int y[]);
void retirer_barriere(etatJeu *jeu, int x[], int y[]);       //barriere.c
int a_gagne(Joueur *joueur);
int tour_joueur(etatJeu *jeu, Joueur *joueur, Joueur joueurs[], int nombre_joueurs);
void appliquer_penalite(Joueur joueurs[], int nombre_joueurs);
void entrer_pseudos(Joueur joueurs[], int nombre_joueurs);
void choisir_pions(Joueur joueurs[], int nombre_joueurs);
int demander_nombre_joueurs();                                  //joueur.c

int lancer_timer();

#endif //QORIDORV1_HEADER_H
