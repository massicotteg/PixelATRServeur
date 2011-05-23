#ifndef NAMESPACE_H
#define NAMESPACE_H

namespace Ui
{
    class MainWindow;
    class thJoueurs;
    class thJeu;

    enum CodesClients { DemandeParties, CreePartie, JoinPartie, MettrePret, DonneesClient, QuitterPartie};
    enum CodesServeurs { ReponseParties, JoueursPartie, DebutPartie, DonneesServeur, FinPartie };
}

#endif // NAMESPACE_H
