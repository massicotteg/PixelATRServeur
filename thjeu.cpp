#include "thjeu.h"

thJeu::thJeu(QList<QByteArray> InitData)
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

    if (tTick->isActive())
    {
        int I = 0;
        while (I < iJoueurs.count() && iJoueurs[I]->Nom != Player)
               I++;
        if (I < iJoueurs.count())
        {
            iJoueurs[I]->Armees.clear();
            iJoueurs[I]->jBase->ProductRate = 0;
            iJoueurs[I]->jBase->NbrPixels = 0;
        }
    }
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
    if (BeginReady && I > 1)
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

void thJeu::EndGame(QByteArray Donnees, QString Partie)
{
    if (NomPartie == Partie || NomPartie + " Terminé" == Partie)
    {
        emit GameEnd(Donnees, NomPartie);
        for (int I = 0; I < iJoueurs.count(); I++)
        {
            delete iJoueurs[I];
            iJoueurs.removeAt(I);
        }
        emit Destroy(this);
    }
}

void thJeu::InitGame()
{
    QByteArray envoi;
    envoi.append("0\n\n");
    iJoueurs = QList<Joueur *>();

    qDebug("InitPlayers");
    iJoueurs.append(new Joueur(Joueurs[0], QPoint(250 + pow(-1,0) * 100, 250 + pow(-1,0) * 100), 2, (Qt::GlobalColor)(7)));
    iJoueurs.append(new Joueur(Joueurs[1], QPoint(250 + pow(-1,1) * 100, 250 + pow(-1,1) * 100), 2, (Qt::GlobalColor)(8)));
    if (Joueurs.count() >= 3)
    {
        iJoueurs.append(new Joueur(Joueurs[2], QPoint(250 + pow(-1,0) * 100, 250 + pow(-1,1) * 100), 2, (Qt::GlobalColor)(9)));
        if (Joueurs.count() >= 4)
            iJoueurs.append(new Joueur(Joueurs[3], QPoint(250 + pow(-1,1) * 100, 250 + pow(-1,0) * 100), 2, (Qt::GlobalColor)(10)));
    }

    qDebug("InitArmees");
    for (int I = 0; I < iJoueurs.count(); I++)
    {
        envoi.append(Joueurs[I] + "\t");
        envoi.append(QString::number((Qt::GlobalColor)(I+7)) + "\t");
        envoi.append(QString::number(iJoueurs[I]->jBase->aPosition.x()) + "\r" + QString::number((iJoueurs[I]->jBase->aPosition.y())) + "\r"  + QString::number(iJoueurs[I]->jBase->NbrPixels) + "\t");
        iJoueurs[I]->Armees.append(new Armee(iJoueurs[I]->jBase->aPosition + QPoint(50,50)));
        iJoueurs[I]->Armees.append(new Armee(iJoueurs[I]->jBase->aPosition + QPoint(-50,50)));
        iJoueurs[I]->Armees.append(new Armee(iJoueurs[I]->jBase->aPosition - QPoint(50,50)));
        iJoueurs[I]->Armees.append(new Armee(iJoueurs[I]->jBase->aPosition + QPoint(50,-50)));
        for (int J = 0; J < iJoueurs[I]->Armees.count(); J++)
            envoi.append(QString::number(iJoueurs[I]->Armees[J]->aPosition.x()) + "\r" + QString::number((iJoueurs[I]->Armees[J]->aPosition.y())) + "\r"  + QString::number(iJoueurs[I]->Armees[J]->NbrPixels) + "\t");
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
    while (I < iJoueurs.count() && PlayerName != iJoueurs[I]->Nom)
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
                iJoueurs[I]->jBase->Commandes.clear();
                for (int J = 0; J < ListePoints.count(); J+=2)
                    iJoueurs[I]->jBase->Commandes.append(QPoint(ListePoints[J].toInt(), ListePoints[J+1].toInt()));
            }
            else
            {
                iJoueurs[I]->Armees[TrameData[1].toInt()-1]->Commandes.clear();
                for (int J = 0; J < ListePoints.count(); J+=2)
                    iJoueurs[I]->Armees[TrameData[1].toInt()-1]->Commandes.append(QPoint(ListePoints[J].toInt(), ListePoints[J+1].toInt()));
            }
        }
    }
}

void thJeu::TickTimeOut()
{
    qDebug("TickTimeOut");
    QByteArray envoi;
    envoi.append("1");

    for (int I = 0; I < iJoueurs.count(); I++)
    {
        iJoueurs[I]->jBase->Move();
        iJoueurs[I]->jBase->NbrPixels += iJoueurs[I]->jBase->ProductRate;
        if (iJoueurs[I]->jBase->NbrPixels > 150)
        {
            iJoueurs[I]->Armees.append(new Armee(iJoueurs[I]->jBase->aPosition));
            iJoueurs[I]->jBase->NbrPixels = 50;
        }

        for (int J = 0; J < iJoueurs[I]->Armees.count(); J++)
        {
            iJoueurs[I]->Armees[J]->Move();

            for (int K = 0; K < iJoueurs.count(); K++)
                for (int L = 0; L < iJoueurs[K]->Armees.count(); L++)
                    if (iJoueurs.at(K)->Armees.at(L)->NbrPixels != 0)
                        if (!(K == I && J == L))
                        {
                            double distancePoints = sqrt(pow(iJoueurs.at(I)->Armees.at(J)->pPosition.y() - iJoueurs.at(K)->Armees.at(L)->pPosition.y(), 2) + pow(iJoueurs.at(I)->Armees.at(J)->pPosition.x() - iJoueurs.at(K)->Armees.at(L)->pPosition.x(), 2));
                            if (distancePoints <= ((-20 * pow(2, -iJoueurs.at(I)->Armees.at(J)->NbrPixels/250.0) + 35) + (-20 * pow(2, -iJoueurs.at(K)->Armees.at(L)->NbrPixels/250.0) + 35)))
                            {
                                if (K!=I)
                                {
                                        if (iJoueurs.at(I)->Armees.at(J)->BatailleEngagee == -1)
                                        {
                                            iJoueurs.at(I)->Armees.at(J)->Commandes.clear();

                                            if (iJoueurs.at(K)->Armees.at(L)->BatailleEngagee == -1)
                                            {
                                                iJoueurs.at(K)->Armees.at(L)->Commandes.clear();
                                                iJoueurs.at(K)->Armees.at(L)->BatailleEngagee = ListeBataille.count();
                                                qDebug("Bataille");
                                                ListeBataille.append(Bataille(iJoueurs.at(I), iJoueurs.at(I)->Armees.at(J), iJoueurs.at(K), iJoueurs.at(K)->Armees.at(L)));
                                            }
                                            else
                                            {
                                                qDebug("Ajout De Participant");
                                                ListeBataille[iJoueurs.at(K)->Armees.at(L)->BatailleEngagee].AjouterParticipant(iJoueurs.at(I), iJoueurs.at(I)->Armees.at(J));
                                            }
                                            iJoueurs.at(I)->Armees.at(J)->BatailleEngagee = iJoueurs.at(K)->Armees.at(L)->BatailleEngagee;
                                        }
                                }
                                else
                                    if (iJoueurs.at(I)->Armees.at(J)->NbrPixels + iJoueurs.at(K)->Armees.at(L)->NbrPixels < 1000 && distancePoints <= ((-15 * pow(2, -iJoueurs.at(I)->Armees.at(J)->NbrPixels/250.0) + 15) + (-15 * pow(2, -iJoueurs.at(K)->Armees.at(L)->NbrPixels/250.0) + 15)))
                                    {
                                        if (iJoueurs.at(K)->Armees.at(L)->BatailleEngagee == -1)
                                        {
                                            qDebug("Combinaison de deux armees");
                                            iJoueurs.at(I)->Armees.at(J)->NbrPixels += iJoueurs.at(K)->Armees.at(L)->NbrPixels;
                                            if (iJoueurs.at(K)->Armees.at(L)->Commandes.count() == 0)
                                                iJoueurs.at(I)->Armees.at(J)->Commandes = iJoueurs.at(K)->Armees.at(L)->Commandes;
                                            iJoueurs.at(K)->Armees.at(L)->NbrPixels = 0;
                                        }
                                    }
                            }
                        }
        }
    }

    for (int I = 0; I < iJoueurs.count(); I++)
        for (int J = 0; J < iJoueurs[I]->Armees.count(); J++)
            if (iJoueurs.at(I)->Armees.at(J)->NbrPixels == 0)
            {
                if (iJoueurs.at(I)->Armees[J]->BatailleEngagee == -1)
                {
                    delete iJoueurs.at(I)->Armees[J];
                    iJoueurs.at(I)->Armees.removeAt(J--);
                }
            }

    for (int I = 0; I < ListeBataille.count(); I++)
    {
        ListeBataille[I].Tick(iJoueurs);
        if (ListeBataille[I].ListeParticipants.count() <= 1)
        {
            ListeBataille.removeAt(I);
            for (int J = I--; J < ListeBataille.count(); J++)
                for (int K = 0; K < ListeBataille[J].ListeParticipants.count(); K++)
                    for (int L = 0; L < ListeBataille[J].ListeParticipants[K].ListeArmees.count(); L++)
                        ListeBataille[J].ListeParticipants[K].ListeArmees[L]->BatailleEngagee--;

        }
    }

    for (int I = 0; I < iJoueurs.count(); I++)
    {
        envoi.append(QString::number(iJoueurs[I]->jBase->aPosition.x()) + "\r" + QString::number((iJoueurs[I]->jBase->aPosition.y())) + "\r"  + QString::number(iJoueurs[I]->jBase->NbrPixels) + "\t");

        for (int J = 0; J < iJoueurs[I]->Armees.count(); J++)
            envoi.append(QString::number(iJoueurs[I]->Armees[J]->aPosition.x()) + "\r" + QString::number((iJoueurs[I]->Armees[J]->aPosition.y())) + "\r"  + QString::number(iJoueurs[I]->Armees[J]->NbrPixels) + "\t");

        envoi.append('\n');
    }

    for (int I = 0; I < iJoueurs.count(); I++)
        if (iJoueurs[I]->Armees.count() == 0)
        {
            iJoueurs[I]->jBase->ProductRate = 0;
            iJoueurs[I]->jBase->NbrPixels = 0;
            ExcludePlayer(iJoueurs[I]->Nom);
        }
    if (Joueurs.count() <= 1)
        GameEnd(envoi, NomPartie);
    else
        emit SendGameSData(envoi);
}
