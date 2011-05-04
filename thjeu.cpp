#include "thjeu.h"

thJeu::thJeu(QList<QByteArray> InitData, QObject *parent) :
    QThread(parent)
{
    NomPartie = InitData[0];
    InitData[1].number(NoMap);
    Joueurs = QList<QString>();
    Ready = QList<bool>();
}

int thJeu::SearchPlayer(QString Player)
{
    int I = 0;
    while (Joueurs[I] == Player)
           I++;
    return I;
}

void thJeu::ExcludePlayer(QString Player)
{
    Joueurs.removeAt(SearchPlayer(Player));
}

void thJeu::CumReady(QString Player)
{
    bool BeginReady = true;
    int I = SearchPlayer(Player);
    Ready[I] = !Ready[I];
    I = 0;
    while (I < Ready.count() && BeginReady)
    {
        I++;
        BeginReady = Ready[I];
    }
    if (I == Ready.count() && I > 2)
        emit GameBegin();
}
