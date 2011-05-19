#include "bataille.h"

Participant::Participant(){}
Bataille::Bataille(){}
Participant::Participant(Joueur joueur, Armee armee)
{
    Proprietaire = joueur;
    ListeArmees = QList<Armee>();
    ListeArmees.append(armee);
    Efficacite = 1;
}

Bataille::Bataille(Joueur Joueur1, Armee ArmeeJoueur1, Joueur Joueur2, Armee ArmeeJoueur2)
{
    ListeParticipants = QList<Participant>();
    ListeParticipants.append(Participant(Joueur1, ArmeeJoueur1));
    ListeParticipants.append(Participant(Joueur2, ArmeeJoueur2));
}
