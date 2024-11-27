cmake_minimum_required(VERSION 3.26)
project(Projet_quoridor C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(Projet_quoridor
        header.h
        main.c
        affichage.c
        Sauvegarde.c
        Deplacement.c
        Barriere.c
        Joueur.c
        Conversion.c
        timer.c
)
