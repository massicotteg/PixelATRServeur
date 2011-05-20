#include "bataille.h"

Participant::Participant(){}
Bataille::Bataille(){}
Participant::Participant(Joueur *joueur, Armee *armee)
{
    Proprietaire = joueur;
    ListeArmees = QList<Armee *>();
    ListeArmees.append(armee);
    Efficacite = 1;
}

Bataille::Bataille(Joueur *Joueur1, Armee *ArmeeJoueur1, Joueur *Joueur2, Armee *ArmeeJoueur2)
{
    ListeParticipants = QList<Participant>();
    ListeParticipants.append(Participant(Joueur1, ArmeeJoueur1));
    ListeParticipants.append(Participant(Joueur2, ArmeeJoueur2));

    double dif = ListeParticipants[0].ListeArmees[0]->NbrPixels - ListeParticipants[1].ListeArmees[0]->NbrPixels / 1000.0;
    ListeParticipants[0].Efficacite += dif;
    ListeParticipants[1].Efficacite -= dif;
}

void Bataille::AjouterParticipant(Joueur *joueur, Armee *armee)
{
    int I = 0;
    while (I < ListeParticipants.count() && ListeParticipants[I].Proprietaire != joueur)
        I++;
    if (I == ListeParticipants.count())
        ListeParticipants.append(Participant(joueur, armee));
    else
    {
        int J = 0;
        while (J < ListeParticipants[I].ListeArmees.count() && ListeParticipants[I].ListeArmees[J] != armee)
            J++;
        if (J == ListeParticipants[I].ListeArmees.count())
            ListeParticipants[I].ListeArmees.append(armee);
    }

    //Calcul de l'efficacité
}

void Bataille::Tick(QList<Joueur *> Joueurs)
{
    for (int I = 0; I < ListeParticipants.count(); I++)
        for (int J = 0; J < ListeParticipants[I].ListeArmees.count(); J++)
            if (ListeParticipants[I].ListeArmees[J]->NbrPixels - 5 / ListeParticipants[I].Efficacite > 0)
                ListeParticipants[I].ListeArmees[J]->NbrPixels -= 5 / ListeParticipants[I].Efficacite;
            else
            {
                Joueurs[Joueurs.indexOf(ListeParticipants[I].Proprietaire)]->Armees.removeOne(ListeParticipants[I].ListeArmees[J]);
                delete ListeParticipants[I].ListeArmees[J];
                ListeParticipants[I].ListeArmees.removeAt(J--);
            }

    for (int I = 0; I < ListeParticipants.count(); I++)
        if (ListeParticipants[I].ListeArmees.count() == 0)
            ListeParticipants.removeAt(I--);

    if (ListeParticipants.count() == 1)
        for (int I = 0; I < Joueurs[Joueurs.indexOf(ListeParticipants[0].Proprietaire)]->Armees.count(); I++)
            Joueurs[Joueurs.indexOf(ListeParticipants[0].Proprietaire)]->Armees[I]->BatailleEngagee = -1;
}
