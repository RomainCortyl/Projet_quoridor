//
// Created by MSI on 27/11/2024.
//
///fonctions de conversion utiles pour convertir les sasies utilisateurs en données utilisables

// Fonction pour convertir une lettre en indice de colonne
#include "header.h"

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