#include "armee.h"

Armee::Armee(){}
Base::Base(){}

Armee::Armee(QPoint StartPosition)
{
    aPosition = StartPosition;
    pPosition = aPosition;
    NbrPixels = 100;
    Commandes = QList<QPoint>();
}

void Armee::Move()
{
    if (Commandes.count() > 0)
    {
        float varx, vary, distance;
        distance = sqrt(pow(Commandes[0].x() - pPosition.x(),2) + pow(Commandes[0].y() - pPosition.y(),2));

        varx = (Commandes[0].x() - pPosition.x()) * 5 / distance;
        vary = (Commandes[0].y() - pPosition.y()) * 5 / distance;
        pPosition.setX(pPosition.x() + varx);
        pPosition.setY(pPosition.y() + vary);
        aPosition = pPosition.toPoint();

        if (distance < 15)
            Commandes.removeAt(0);
    }
}

Base::Base(QPoint Position, int SpawnRate) : Armee(Position)
{
    ProductRate = SpawnRate;
    NbrPixels = 50;
}
