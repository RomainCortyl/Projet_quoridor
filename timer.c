//
// Created by MSI on 27/11/2024.
//
#include "header.h"


int lancer_timer(clock_t debut_temps) {
    clock_t temps_ecoule = (clock() - debut_temps) / CLOCKS_PER_SEC;
    if (temps_ecoule >= TEMPS_MAX) {
        return 1; // Indique que le temps est écoulé
    }
    return 0; // Temps non écoulé
}
