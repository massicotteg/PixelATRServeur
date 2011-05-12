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
    while (I < Joueurs.count() && Joueurs[I] != Player)
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
    if (I == Ready.count() && I > 1)
    {
        emit GameBegin();
        start();
    }
    PlayersUpdate();
}

void thJeu::PlayersUpdate()
{
    QByteArray PlayersList = QByteArray(1, Ui::GamePlayers);
    for (int I = 0; I < Joueurs.count(); I++)
        PlayersList.append(Joueurs[I] + (Ready[I]?" (Pret)":"") + '\n');
    emit PlayersUpdate(PlayersList);
}

void thJeu::EndGame(QString Partie)
{
    if (NomPartie == Partie)
    {
        emit GameEnd();
        emit Destroy(this);
    }
}

void thJeu::PlayersData(QString PlayerName, QByteArray Data)
{

}

void thJeu::InitGame()
{
    QByteArray envoi;
    envoi.append("0\n\n");
    iJoueurs = QList<Joueur>();
    for (int I = 0; I < Joueurs.count(); I++)
    {
        envoi.append(Joueurs[I] + "\t");
        envoi.append(QString::number((Qt::GlobalColor)I) + "\t");
        iJoueurs.append(Joueur(Joueurs[I], QPoint(250 + pow(-1,I) * 100, 250 + pow(-1,I) * 100), 5, (Qt::GlobalColor)I));
        envoi.append(QString::number(iJoueurs[I].jBase.aPosition.x()) + "\r" + QString::number((iJoueurs[I].jBase.aPosition.y())) + "\r"  + QString::number(iJoueurs[I].jBase.NbrPixels) + "\t");
        iJoueurs[I].Armees.append(Armee(iJoueurs[I].jBase.aPosition + QPoint(50,50)));
        iJoueurs[I].Armees.append(Armee(iJoueurs[I].jBase.aPosition + QPoint(-50,50)));
        iJoueurs[I].Armees.append(Armee(iJoueurs[I].jBase.aPosition - QPoint(50,50)));
        iJoueurs[I].Armees.append(Armee(iJoueurs[I].jBase.aPosition + QPoint(50,-50)));
        for (int J = 0; J < iJoueurs[I].Armees.count(); J++)
            envoi.append(QString::number(iJoueurs[I].Armees[J].aPosition.x()) + "\r" + QString::number((iJoueurs[I].Armees[J].aPosition.y())) + "\r"  + QString::number(iJoueurs[I].Armees[J].NbrPixels) + "\t");
        envoi.append("\n");
    }

    emit SendGameSData(envoi);
}
void thJeu::run()
{
    InitGame();
}
