#include "armee.h"

Armee::Armee(){}
Base::Base(){}

Armee::Armee(QPoint Position)
{
    m_aPosition = Position;
    m_pPosition = m_aPosition;
    m_NbrPixels = 100;
    m_Commandes = QList<QPoint>();
    m_BatailleEngagee = -1;
}

void Armee::Bouge()
{
    float varx, vary, distance, DepDist = 7.5;

    while (m_Commandes.count() > 0 && DepDist > 0)
    {
        distance = sqrt(pow(m_Commandes[0].x() - m_pPosition.x(),2) + pow(m_Commandes[0].y() - m_pPosition.y(),2));
        if (distance > DepDist)
        {
            varx = (m_Commandes[0].x() - m_pPosition.x()) * DepDist / distance;
            vary = (m_Commandes[0].y() - m_pPosition.y()) * DepDist / distance;
            DepDist = 0;
        }
        else
        {
            varx = (m_Commandes[0].x() - m_pPosition.x());
            vary = (m_Commandes[0].y() - m_pPosition.y());
            DepDist-=distance;
            m_Commandes.removeAt(0);
        }
        m_pPosition.setX(m_pPosition.x() + varx);
        m_pPosition.setY(m_pPosition.y() + vary);
    }
    m_aPosition = m_pPosition.toPoint();
}

Base::Base(QPoint Position, int tauxProduction) : Armee(Position)
{
    TauxProduction = tauxProduction;
    m_NbrPixels = 50;
}
