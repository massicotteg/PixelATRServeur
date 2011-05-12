#include "joueur.h"

Joueur::Joueur(QString NomJoueur, QPoint Position, int SpawnRate, QColor  CouleurJoueur)
{
    Nom = NomJoueur;
    jBase = Base(Position, SpawnRate);
    Couleur = CouleurJoueur;
}
