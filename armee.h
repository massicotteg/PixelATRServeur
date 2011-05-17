#ifndef ARMEE_H
#define ARMEE_H

#include <QtGui>

class Armee
{
public:
    Armee();
    Armee(QPoint);
    QPoint aPosition;
    QPointF pPosition;
    QList<QPoint> Commandes;
    int NbrPixels;
    void Move();
};

class Base: public Armee
{
public:
    Base();
    Base(QPoint, int);
    int ProductRate;
};

#endif // ARMEE_H
