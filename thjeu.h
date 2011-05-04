#ifndef THJEU_H
#define THJEU_H

#include <NameSpace.h>
#include <QThread>

class thJeu : public QThread
{
    Q_OBJECT
public:
    explicit thJeu(QObject *parent = 0);
    enum CodesClients { GamesRequest, GameCreate, GameJoin, GameSetReady, GameCData };
    enum CodesServeurs { GamesReply, GamePlayers, GameBegin, GameSData, GameEnd };

signals:

public slots:

};

#endif // THJEU_H
