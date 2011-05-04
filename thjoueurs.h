#ifndef THJOUEURS_H
#define THJOUEURS_H

#include <NameSpace.h>
#include <thjeu.h>
#include <QThread>
#include <QtNetwork>

class thJoueurs : public QThread
{
    Q_OBJECT
public:
    explicit thJoueurs(QObject *parent = 0, QTcpSocket *socketClient=0);

private:
    QTcpSocket *SocketClient;

signals:
    void Disconnected(thJoueurs *);
    void GamesRequest(thJoueurs *);
    void CreateRequest(thJoueurs *);
    void JoinRequest(thJoueurs *);
    void SetReady(thJoueurs *);
    void GameData(thJoueurs *);

public slots:
    void SocketClient_ReadyRead();
    void SocketClient_Disconnected();
    void GamesRequestReply(thJoueurs *, QString);

    void PlayersReply(thJoueurs *, QString);
    void GameBegin(int);
    void GameEnd(int);
    void GameSData(int, QByteArray);
};

#endif // THJOUEURS_H
