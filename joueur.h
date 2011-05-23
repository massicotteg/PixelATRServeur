#ifndef JOUEUR_H
#define JOUEUR_H

#include <QtGui>
#include <armee.h>

class Joueur
{
public:
    Joueur();
    explicit Joueur(QString, QPoint, int, QColor);
    QString m_Nom;
    Base *m_jBase;
    QList<Armee *> m_Armees;
    QColor m_Couleur;
};

#endif // JOUEUR_H
