#include "joueur.h"

Joueur::Joueur(){}
Joueur::Joueur(QString NomJoueur, QPoint Position, int tauxProduction, QColor  CouleurJoueur)
{
    m_Nom = NomJoueur;
    m_jBase = new Base(Position, tauxProduction);
    m_Couleur = CouleurJoueur;
    m_Armees = QList<Armee *>();
}
