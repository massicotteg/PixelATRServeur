#ifndef BATAILLE_H
#define BATAILLE_H

#include <QtGui>
#include <joueur.h>

class Participant
{
public:
    Participant();
    Participant(Joueur *, Armee *);
    QList<Armee *> m_ListeArmees;
    Joueur *m_Proprietaire;
    double m_Efficacite;
};

class Bataille
{
public:
    Bataille();
    Bataille(Joueur *Joueur1, Armee *ArmeeJoueur1, Joueur *Joueur2, Armee *ArmeeJoueur2);
    QList<Participant> m_ListeParticipants;
    void Tick(QList<Joueur *>);
    void AjouterParticipant(Joueur *joueur, Armee *armee);
};

#endif // BATAILLE_H
