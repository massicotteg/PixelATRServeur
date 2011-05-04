#ifndef NAMESPACE_H
#define NAMESPACE_H

namespace Ui
{
    class MainWindow;
    class thJoueurs;
    class thJeu;

    enum CodesClients { GamesRequest, GameCreate, GameJoin, GameSetReady, GameCData, GameQuit};
    enum CodesServeurs { GamesReply, GamePlayers, GameBegin, GameSData, GameEnd };
}

#endif // NAMESPACE_H
