#include "thjeu.h"

thJeu::thJeu(QList<QByteArray> InitData, QObject *parent) :
    QThread(parent)
{
    NomPartie = InitData[0];
    InitData[1].number(NoMap);
    Joueurs = QList<QString>();
    Ready = QList<bool>();
    tTick = new QTimer();

    connect(tTick, SIGNAL(timeout()), this, SLOT(TickTimeOut()));
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
    if (BeginReady && I > 0)
        InitGame();
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

void thJeu::InitGame()
{
    QByteArray envoi;
    envoi.append("0\n\n");
    iJoueurs = QList<Joueur>();
    for (int I = 0; I < Joueurs.count(); I++)
    {
        envoi.append(Joueurs[I] + "\t");
        envoi.append(QString::number((Qt::GlobalColor)(I+7)) + "\t");
        iJoueurs.append(Joueur(Joueurs[I], QPoint(250 + pow(-1,I) * 100, 250 + pow(-1,I) * 100), 5, (Qt::GlobalColor)(I+7)));
        envoi.append(QString::number(iJoueurs[I].jBase.aPosition.x()) + "\r" + QString::number((iJoueurs[I].jBase.aPosition.y())) + "\r"  + QString::number(iJoueurs[I].jBase.NbrPixels) + "\t");
        iJoueurs[I].Armees.append(Armee(iJoueurs[I].jBase.aPosition + QPoint(50,50)));
        iJoueurs[I].Armees.append(Armee(iJoueurs[I].jBase.aPosition + QPoint(-50,50)));
        iJoueurs[I].Armees.append(Armee(iJoueurs[I].jBase.aPosition - QPoint(50,50)));
        iJoueurs[I].Armees.append(Armee(iJoueurs[I].jBase.aPosition + QPoint(50,-50)));
        for (int J = 0; J < iJoueurs[I].Armees.count(); J++)
            envoi.append(QString::number(iJoueurs[I].Armees[J].aPosition.x()) + "\r" + QString::number((iJoueurs[I].Armees[J].aPosition.y())) + "\r"  + QString::number(iJoueurs[I].Armees[J].NbrPixels) + "\t");
        envoi.append("\n");
    }

    emit GameBegin(envoi);
    tTick->start(100);
}

void thJeu::PlayersData(QString PlayerName, QByteArray Data)
{
    QList<QByteArray> TrameData;
    QList<QByteArray> ListePoints;
    int I = 0;
    while (I < iJoueurs.count() && PlayerName != iJoueurs[I].Nom)
        I++;

    if (I != iJoueurs.count())
    {
        TrameData = Data.split('\n');

        if (TrameData.count() > 2)
        {
            ListePoints = TrameData[2].split('\t');
            ListePoints.removeLast();
            if (TrameData[1].toInt() == 0)
            {
                iJoueurs[I].jBase.Commandes.clear();
                for (int J = 0; J < ListePoints.count(); J+=2)
                    iJoueurs[I].jBase.Commandes.append(QPoint(ListePoints[J].toInt(), ListePoints[J+1].toInt()));
            }
            else
            {
                iJoueurs[I].Armees[TrameData[1].toInt()-1].Commandes.clear();
                for (int J = 0; J < ListePoints.count(); J+=2)
                    iJoueurs[I].Armees[TrameData[1].toInt()-1].Commandes.append(QPoint(ListePoints[J].toInt(), ListePoints[J+1].toInt()));
            }
        }
    }
}

void thJeu::TickTimeOut()
{
    QByteArray envoi;
    envoi.append("1");

    for (int I = 0; I < iJoueurs.count(); I++)
    {
        iJoueurs[I].jBase.Move();
        envoi.append(QString::number(iJoueurs[I].jBase.aPosition.x()) + "\r" + QString::number((iJoueurs[I].jBase.aPosition.y())) + "\r"  + QString::number(iJoueurs[I].jBase.NbrPixels) + "\t");


        for (int J = 0; J < iJoueurs[I].Armees.count(); J++)
        {
            iJoueurs[I].Armees[J].Move();

            for (int K = 0; K < iJoueurs.count(); K++)
                if (K!=I)
                    for (int L = 0; L < iJoueurs[K].Armees.count(); L++)
                        if (sqrt(pow(iJoueurs[I].Armees[J].pPosition.y() - iJoueurs[K].Armees[L].pPosition.y(), 2) + pow(iJoueurs[I].Armees[J].pPosition.x() - iJoueurs[K].Armees[L].pPosition.x(), 2)) <= ((-20 * pow(2, -iJoueurs[I].Armees[J].NbrPixels/250.0) + 35) + (-20 * pow(2, -iJoueurs[K].Armees[L].NbrPixels/250.0) + 35)))
                            iJoueurs[I].Armees[J].Commandes.clear();

            envoi.append(QString::number(iJoueurs[I].Armees[J].aPosition.x()) + "\r" + QString::number((iJoueurs[I].Armees[J].aPosition.y())) + "\r"  + QString::number(iJoueurs[I].Armees[J].NbrPixels) + "\t");
        }
        envoi.append('\n');
    }

    emit SendGameSData(envoi);
}
