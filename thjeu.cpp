#include "thjeu.h"

thJeu::thJeu(QList<QByteArray> DonneesInitialisation)
{
    m_NomPartie = DonneesInitialisation[0];
    DonneesInitialisation[1].number(m_NoMap);
    m_NomJoueurs = QList<QString>();
    m_Pret = QList<bool>();
    m_tTick = new QTimer();

    connect(m_tTick, SIGNAL(timeout()), this, SLOT(TickTimeOut()));
}

int thJeu::ChercheJoueur(QString Joueur)
{
    int I = 0;
    while (I < m_NomJoueurs.count() && m_NomJoueurs[I] != Joueur)
           I++;
    return I;
}

void thJeu::ExclureJoueur(QString Joueur)
{
    int I = ChercheJoueur(Joueur);
    m_NomJoueurs.removeAt(I);
    m_Pret.removeAt(I);

    if (m_tTick->isActive())
    {
        int I = 0;
        while (I < m_Joueurs.count() && m_Joueurs[I]->m_Nom != Joueur)
               I++;
        if (I < m_Joueurs.count())
        {
            m_Joueurs[I]->m_Armees.clear();
            m_Joueurs[I]->m_jBase->TauxProduction = 0;
            m_Joueurs[I]->m_jBase->m_NbrPixels = 0;
        }
    }
}

void thJeu::CumPret(QString Joueur)
{
    bool PretACommencer = true;
    int I = ChercheJoueur(Joueur);
    m_Pret[I] = !m_Pret[I];
    I = 0;
    while (I < m_Pret.count() && PretACommencer)
    {
        PretACommencer = m_Pret[I];
        I++;
    }
    if (PretACommencer && I > 1)
        InitPartie();
    MetAJourJoueurs();
}

void thJeu::MetAJourJoueurs()
{
    QByteArray ListeJoueurs = QByteArray(1, Ui::JoueursPartie);
    for (int I = 0; I < m_NomJoueurs.count(); I++)
        ListeJoueurs.append(m_NomJoueurs[I] + (m_Pret[I]?" (Pret)":"") + '\n');
    emit MetAJourJoueurs(ListeJoueurs);
}

void thJeu::TerminePartie(QByteArray Donnees, QString Partie)
{
    if (m_NomPartie == Partie || m_NomPartie == Partie + "D" || m_NomPartie.remove(m_NomPartie.length()-1,1) + " Termine" == Partie)
    {
        emit FinPartie(Donnees, m_NomPartie);
        for (int I = 0; I < m_Joueurs.count(); I++)
        {
            delete m_Joueurs[I];
            m_Joueurs.removeAt(I);
        }
        emit Detruire(this);
    }
}

void thJeu::InitPartie()
{
    m_NomPartie += "D";

    QByteArray envoi;
    envoi.append("0\n\n");
    m_Joueurs = QList<Joueur *>();

    qDebug("InitPlayers");
    m_Joueurs.append(new Joueur(m_NomJoueurs[0], QPoint(250 + pow(-1,0) * 100, 250 + pow(-1,0) * 100), 2, (Qt::GlobalColor)(7)));
    m_Joueurs.append(new Joueur(m_NomJoueurs[1], QPoint(250 + pow(-1,1) * 100, 250 + pow(-1,1) * 100), 2, (Qt::GlobalColor)(8)));
    if (m_NomJoueurs.count() >= 3)
    {
        m_Joueurs.append(new Joueur(m_NomJoueurs[2], QPoint(250 + pow(-1,0) * 100, 250 + pow(-1,1) * 100), 2, (Qt::GlobalColor)(9)));
        if (m_NomJoueurs.count() >= 4)
            m_Joueurs.append(new Joueur(m_NomJoueurs[3], QPoint(250 + pow(-1,1) * 100, 250 + pow(-1,0) * 100), 2, (Qt::GlobalColor)(10)));
    }

    qDebug("InitArmees");
    for (int I = 0; I < m_Joueurs.count(); I++)
    {
        envoi.append(m_NomJoueurs[I] + "\t");
        envoi.append(QString::number((Qt::GlobalColor)(I+7)) + "\t");
        envoi.append(QString::number(m_Joueurs[I]->m_jBase->m_aPosition.x()) + "\r" + QString::number((m_Joueurs[I]->m_jBase->m_aPosition.y())) + "\r"  + QString::number(m_Joueurs[I]->m_jBase->m_NbrPixels) + "\t");
        m_Joueurs[I]->m_Armees.append(new Armee(m_Joueurs[I]->m_jBase->m_aPosition + QPoint(50,50)));
        m_Joueurs[I]->m_Armees.append(new Armee(m_Joueurs[I]->m_jBase->m_aPosition + QPoint(-50,50)));
        m_Joueurs[I]->m_Armees.append(new Armee(m_Joueurs[I]->m_jBase->m_aPosition - QPoint(50,50)));
        m_Joueurs[I]->m_Armees.append(new Armee(m_Joueurs[I]->m_jBase->m_aPosition + QPoint(50,-50)));
        for (int J = 0; J < m_Joueurs[I]->m_Armees.count(); J++)
            envoi.append(QString::number(m_Joueurs[I]->m_Armees[J]->m_aPosition.x()) + "\r" + QString::number((m_Joueurs[I]->m_Armees[J]->m_aPosition.y())) + "\r"  + QString::number(m_Joueurs[I]->m_Armees[J]->m_NbrPixels) + "\t");
        envoi.append("\n");
    }

    emit DebutPartie(envoi);
    m_tTick->start(100);
}

void thJeu::DonneesJoueurs(QString NomJoueur, QByteArray Donnees)
{
    QList<QByteArray> TrameDonnees;
    QList<QByteArray> ListePoints;
    int I = 0;
    while (I < m_Joueurs.count() && NomJoueur != m_Joueurs[I]->m_Nom)
        I++;

    if (I != m_Joueurs.count())
    {
        TrameDonnees = Donnees.split('\n');

        if (TrameDonnees.count() > 2)
        {
            ListePoints = TrameDonnees[2].split('\t');
            ListePoints.removeLast();
            if (TrameDonnees[1].toInt() == 0)
            {
                m_Joueurs[I]->m_jBase->m_Commandes.clear();
                for (int J = 0; J < ListePoints.count(); J+=2)
                    m_Joueurs[I]->m_jBase->m_Commandes.append(QPoint(ListePoints[J].toInt(), ListePoints[J+1].toInt()));
            }
            else
            {
                m_Joueurs[I]->m_Armees[TrameDonnees[1].toInt()-1]->m_Commandes.clear();
                for (int J = 0; J < ListePoints.count(); J+=2)
                    m_Joueurs[I]->m_Armees[TrameDonnees[1].toInt()-1]->m_Commandes.append(QPoint(ListePoints[J].toInt(), ListePoints[J+1].toInt()));
            }
        }
    }
}

void thJeu::TickTimeOut()
{
    qDebug("TickTimeOut");
    QByteArray envoi;
    envoi.append("1");

    for (int I = 0; I < m_Joueurs.count(); I++)
    {
        m_Joueurs[I]->m_jBase->Bouge();
        m_Joueurs[I]->m_jBase->m_NbrPixels += m_Joueurs[I]->m_jBase->TauxProduction;
        if (m_Joueurs[I]->m_jBase->m_NbrPixels > 150)
        {
            m_Joueurs[I]->m_Armees.append(new Armee(m_Joueurs[I]->m_jBase->m_aPosition));
            m_Joueurs[I]->m_jBase->m_NbrPixels = 50;
        }

        for (int J = 0; J < m_Joueurs[I]->m_Armees.count(); J++)
        {
            m_Joueurs[I]->m_Armees[J]->Bouge();

            for (int K = 0; K < m_Joueurs.count(); K++)
                for (int L = 0; L < m_Joueurs[K]->m_Armees.count(); L++)
                    if (m_Joueurs.at(K)->m_Armees.at(L)->m_NbrPixels != 0)
                        if (!(K == I && J == L))
                        {
                            double distancePoints = sqrt(pow(m_Joueurs.at(I)->m_Armees.at(J)->m_pPosition.y() - m_Joueurs.at(K)->m_Armees.at(L)->m_pPosition.y(), 2) + pow(m_Joueurs.at(I)->m_Armees.at(J)->m_pPosition.x() - m_Joueurs.at(K)->m_Armees.at(L)->m_pPosition.x(), 2));
                            if (distancePoints <= ((-20 * pow(2, -m_Joueurs.at(I)->m_Armees.at(J)->m_NbrPixels/250.0) + 35) + (-20 * pow(2, -m_Joueurs.at(K)->m_Armees.at(L)->m_NbrPixels/250.0) + 35)))
                            {
                                if (K!=I)
                                {
                                        if (m_Joueurs.at(I)->m_Armees.at(J)->m_BatailleEngagee == -1)
                                        {
                                            m_Joueurs.at(I)->m_Armees.at(J)->m_Commandes.clear();

                                            if (m_Joueurs.at(K)->m_Armees.at(L)->m_BatailleEngagee == -1)
                                            {
                                                m_Joueurs.at(K)->m_Armees.at(L)->m_Commandes.clear();
                                                m_Joueurs.at(K)->m_Armees.at(L)->m_BatailleEngagee = m_ListeBataille.count();
                                                qDebug("Bataille");
                                                m_ListeBataille.append(Bataille(m_Joueurs.at(I), m_Joueurs.at(I)->m_Armees.at(J), m_Joueurs.at(K), m_Joueurs.at(K)->m_Armees.at(L)));
                                            }
                                            else
                                            {
                                                qDebug("Ajout De Participant");
                                                m_ListeBataille[m_Joueurs.at(K)->m_Armees.at(L)->m_BatailleEngagee].AjouterParticipant(m_Joueurs.at(I), m_Joueurs.at(I)->m_Armees.at(J));
                                            }
                                            m_Joueurs.at(I)->m_Armees.at(J)->m_BatailleEngagee = m_Joueurs.at(K)->m_Armees.at(L)->m_BatailleEngagee;
                                        }
                                }
                                else
                                    if (m_Joueurs.at(I)->m_Armees.at(J)->m_NbrPixels + m_Joueurs.at(K)->m_Armees.at(L)->m_NbrPixels < 1000 && distancePoints <= ((-15 * pow(2, -m_Joueurs.at(I)->m_Armees.at(J)->m_NbrPixels/250.0) + 15) + (-15 * pow(2, -m_Joueurs.at(K)->m_Armees.at(L)->m_NbrPixels/250.0) + 15)))
                                    {
                                        if (m_Joueurs.at(K)->m_Armees.at(L)->m_BatailleEngagee == -1)
                                        {
                                            qDebug("Combinaison de deux armees");
                                            m_Joueurs.at(I)->m_Armees.at(J)->m_NbrPixels += m_Joueurs.at(K)->m_Armees.at(L)->m_NbrPixels;
                                            if (m_Joueurs.at(K)->m_Armees.at(L)->m_Commandes.count() == 0)
                                                m_Joueurs.at(I)->m_Armees.at(J)->m_Commandes = m_Joueurs.at(K)->m_Armees.at(L)->m_Commandes;
                                            m_Joueurs.at(K)->m_Armees.at(L)->m_NbrPixels = 0;
                                        }
                                    }
                            }
                        }
        }
    }

    for (int I = 0; I < m_Joueurs.count(); I++)
        for (int J = 0; J < m_Joueurs[I]->m_Armees.count(); J++)
            if (m_Joueurs.at(I)->m_Armees.at(J)->m_NbrPixels == 0)
            {
                if (m_Joueurs.at(I)->m_Armees[J]->m_BatailleEngagee == -1)
                {
                    delete m_Joueurs.at(I)->m_Armees[J];
                    m_Joueurs.at(I)->m_Armees.removeAt(J--);
                }
            }

    for (int I = 0; I < m_ListeBataille.count(); I++)
    {
        m_ListeBataille[I].Tick(m_Joueurs);
        if (m_ListeBataille[I].m_ListeParticipants.count() <= 1)
        {
            m_ListeBataille.removeAt(I);
            for (int J = I--; J < m_ListeBataille.count(); J++)
                for (int K = 0; K < m_ListeBataille[J].m_ListeParticipants.count(); K++)
                    for (int L = 0; L < m_ListeBataille[J].m_ListeParticipants[K].m_ListeArmees.count(); L++)
                        m_ListeBataille[J].m_ListeParticipants[K].m_ListeArmees[L]->m_BatailleEngagee--;

        }
    }

    for (int I = 0; I < m_Joueurs.count(); I++)
    {
        envoi.append(QString::number(m_Joueurs[I]->m_jBase->m_aPosition.x()) + "\r" + QString::number((m_Joueurs[I]->m_jBase->m_aPosition.y())) + "\r"  + QString::number(m_Joueurs[I]->m_jBase->m_NbrPixels) + "\t");

        for (int J = 0; J < m_Joueurs[I]->m_Armees.count(); J++)
            envoi.append(QString::number(m_Joueurs[I]->m_Armees[J]->m_aPosition.x()) + "\r" + QString::number((m_Joueurs[I]->m_Armees[J]->m_aPosition.y())) + "\r"  + QString::number(m_Joueurs[I]->m_Armees[J]->m_NbrPixels) + "\t");

        envoi.append('\n');
    }

    for (int I = 0; I < m_Joueurs.count(); I++)
        if (m_Joueurs[I]->m_Armees.count() == 0)
        {
            m_Joueurs[I]->m_jBase->TauxProduction = 0;
            m_Joueurs[I]->m_jBase->m_NbrPixels = 0;
            ExclureJoueur(m_Joueurs[I]->m_Nom);
        }
    if (m_NomJoueurs.count() <= 1)
        FinPartie(envoi, m_NomPartie);
    else
        emit EnvoiDonneesServeur(envoi);
}
