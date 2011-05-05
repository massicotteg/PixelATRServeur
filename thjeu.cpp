#include "thjeu.h"

thJeu::thJeu(QList<QByteArray> InitData, QObject *parent) :
    QThread(parent)
{
    NomPartie = InitData[0];
    InitData[1].number(NoMap);
    Joueurs = QList<QString>();
    Ready = QList<bool>();

    tPlayersUpdate = new QTimer();
    connect(tPlayersUpdate, SIGNAL(timeout()), this, SLOT(tPlayersUpdate_Timeout()));
    tPlayersUpdate->start(500);
}

int thJeu::SearchPlayer(QString Player)
{
    int I = 0;
    while (Joueurs[I] != Player)
           I++;
    return I;
}

void thJeu::ExcludePlayer(QString Player)
{
    int I = SearchPlayer(Player);
    Joueurs.removeAt(I);
    Ready.removeAt(I);
}

void thJeu::CumReady(QString Player)
{
    bool BeginReady = true;
    int I = SearchPlayer(Player);
    Ready[I] = !Ready[I];
    I = 0;
    while (I < Ready.count() && BeginReady)
    {
        BeginReady = Ready[I];
        I++;
    }
    if (I == Ready.count() && I > 2)
        emit GameBegin();
}

void thJeu::tPlayersUpdate_Timeout()
{
    QByteArray PlayersList = QByteArray(1, Ui::GamePlayers);
    for (int I = 0; I < Joueurs.count(); I++)
        PlayersList.append(Joueurs[I] + (Ready[I]?" (Pret)":"") + '\n');
    emit PlayersUpdate(PlayersList);
}

void thJeu::PlayersData(QString PlayerName, QByteArray Data)
{

}
