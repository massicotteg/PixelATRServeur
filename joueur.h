#ifndef JOUEUR_H
#define JOUEUR_H

#include <QtGui>
#include <armee.h>

class Joueur
{
public:
    explicit Joueur(QString, QPoint, int, QColor);
    QString Nom;
    Base jBase;
    QList<Armee> Armees;
    QColor Couleur;
};

#endif // JOUEUR_H
