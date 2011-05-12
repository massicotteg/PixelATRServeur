#include "armee.h"

Armee::Armee(){}
Base::Base(){}

Armee::Armee(QPoint StartPosition)
{
    aPosition = StartPosition;
    NbrPixels = 100;
}

Base::Base(QPoint Position, int SpawnRate) : Armee(Position)
{
    ProductRate = SpawnRate;
    NbrPixels = 50;
}
