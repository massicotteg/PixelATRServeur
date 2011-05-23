#include "bataille.h"

Participant::Participant(){}
Bataille::Bataille(){}
Participant::Participant(Joueur *joueur, Armee *armee)
{
    m_Proprietaire = joueur;
    m_ListeArmees = QList<Armee *>();
    m_ListeArmees.append(armee);
    m_Efficacite = 1;
}

Bataille::Bataille(Joueur *Joueur1, Armee *ArmeeJoueur1, Joueur *Joueur2, Armee *ArmeeJoueur2)
{
    m_ListeParticipants = QList<Participant>();
    m_ListeParticipants.append(Participant(Joueur1, ArmeeJoueur1));
    m_ListeParticipants.append(Participant(Joueur2, ArmeeJoueur2));

    double dif = (m_ListeParticipants[0].m_ListeArmees[0]->m_NbrPixels - m_ListeParticipants[1].m_ListeArmees[0]->m_NbrPixels) / 1000.0;
    m_ListeParticipants[0].m_Efficacite += dif;
    m_ListeParticipants[1].m_Efficacite -= dif;
}

void Bataille::AjouterParticipant(Joueur *joueur, Armee *armee)
{
    int I = 0;
    while (I < m_ListeParticipants.count() && m_ListeParticipants[I].m_Proprietaire != joueur)
        I++;
    if (I == m_ListeParticipants.count())
    {
        qDebug("Ajout du nouveau participant");
        m_ListeParticipants.append(Participant(joueur, armee));
    }
    else
    {
        int J = 0;
        while (J < m_ListeParticipants[I].m_ListeArmees.count() && m_ListeParticipants[I].m_ListeArmees[J] != armee)
            J++;
        if (J == m_ListeParticipants[I].m_ListeArmees.count())
        {
            qDebug("Ajout de son armee");
            m_ListeParticipants[I].m_ListeArmees.append(armee);
        }
    }
}

void Bataille::Tick(QList<Joueur *> Joueurs)
{
    for (int I = 0; I < m_ListeParticipants.count(); I++)
        for (int J = 0; J < m_ListeParticipants[I].m_ListeArmees.count(); J++)
            if (m_ListeParticipants[I].m_ListeArmees[J]->m_NbrPixels - 5 / m_ListeParticipants[I].m_Efficacite > 0)
                m_ListeParticipants[I].m_ListeArmees[J]->m_NbrPixels -= 5 / m_ListeParticipants[I].m_Efficacite;
            else
            {
                Joueurs[Joueurs.indexOf(m_ListeParticipants[I].m_Proprietaire)]->m_Armees.removeOne(m_ListeParticipants[I].m_ListeArmees[J]);
                m_ListeParticipants[I].m_ListeArmees.removeAt(J--);
            }

    for (int I = 0; I < m_ListeParticipants.count(); I++)
        if (m_ListeParticipants[I].m_ListeArmees.count() == 0)
            m_ListeParticipants.removeAt(I--);

    if (m_ListeParticipants.count() == 1)
        for (int I = 0; I < Joueurs[Joueurs.indexOf(m_ListeParticipants[0].m_Proprietaire)]->m_Armees.count(); I++)
            Joueurs[Joueurs.indexOf(m_ListeParticipants[0].m_Proprietaire)]->m_Armees[I]->m_BatailleEngagee = -1;
}
