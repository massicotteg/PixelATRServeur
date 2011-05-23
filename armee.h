#ifndef ARMEE_H
#define ARMEE_H

#include <QtGui>

class Armee
{
public:
    Armee();
    Armee(QPoint);
    QPoint m_aPosition;
    QPointF m_pPosition;
    QList<QPoint> m_Commandes;
    int m_NbrPixels;
    int m_BatailleEngagee;
    void Bouge();
};

class Base: public Armee
{
public:
    Base();
    Base(QPoint, int);
    int TauxProduction;
};

#endif // ARMEE_H
