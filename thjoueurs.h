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
    QString Nom;
    thJeu *Partie;

private:
    QTcpSocket *SocketClient;
    bool GameAssigned;

signals:
    void Disconnected(thJoueurs *);
    void GamesRequest(thJoueurs *);
    void PlayersRequest(thJoueurs *, thJeu *);
    void CreateRequest(QList<QByteArray>);
    void JoinRequest(thJoueurs *, QList<QByteArray>);
    void SetReady(QString);
    void GameData(QString, QByteArray);
    void GameQuit(QString);

public slots:
    void SocketClient_ReadyRead();
    void SocketClient_Disconnected();
    void GamesRequestReply(thJoueurs *, QString);

    void PlayersReply(thJoueurs *, QString);
    void GameBegin();
    void GameEnd();
    void GameSData(QByteArray);
};

#endif // THJOUEURS_H
