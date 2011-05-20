#include "armee.h"

Armee::Armee(){}
Base::Base(){}

Armee::Armee(QPoint StartPosition)
{
    aPosition = StartPosition;
    pPosition = aPosition;
    NbrPixels = 100;
    Commandes = QList<QPoint>();
    BatailleEngagee = -1;
}

void Armee::Move()
{
    float varx, vary, distance, DepDist = 5;

    while (Commandes.count() > 0 && DepDist > 0)
    {
        distance = sqrt(pow(Commandes[0].x() - pPosition.x(),2) + pow(Commandes[0].y() - pPosition.y(),2));
        if (distance > DepDist)
        {
            varx = (Commandes[0].x() - pPosition.x()) * DepDist / distance;
            vary = (Commandes[0].y() - pPosition.y()) * DepDist / distance;
            DepDist = 0;
        }
        else
        {
            varx = (Commandes[0].x() - pPosition.x());
            vary = (Commandes[0].y() - pPosition.y());
            DepDist-=distance;
            Commandes.removeAt(0);
        }
        pPosition.setX(pPosition.x() + varx);
        pPosition.setY(pPosition.y() + vary);
    }
    aPosition = pPosition.toPoint();
}

Base::Base(QPoint Position, int SpawnRate) : Armee(Position)
{
    ProductRate = SpawnRate;
    NbrPixels = 50;
}
