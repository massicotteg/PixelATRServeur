#include "joueur.h"

Joueur::Joueur(){}
Joueur::Joueur(QString NomJoueur, QPoint Position, int SpawnRate, QColor  CouleurJoueur)
{
    Nom = NomJoueur;
    jBase = new Base(Position, SpawnRate);
    Couleur = CouleurJoueur;
    Armees = QList<Armee *>();
}
