#ifndef THJOUEURS_H
#define THJOUEURS_H

#include <NameSpace.h>
#include <QThread>
#include <QtNetwork>

class thJoueurs : public QThread
{
    Q_OBJECT
public:
    explicit thJoueurs(QObject *parent = 0, QTcpSocket *socketClient=0);
    ~thJoueurs();
    QString Nom;

private:
    QByteArray ToQByteArray(int);
    QTcpSocket *SocketClient;
    bool GameAssigned;

signals:
    void Disconnected(QThread *);

    void GamesRequest(thJoueurs *);
    void CreateRequest(QList<QByteArray>);
    void JoinRequest(thJoueurs *, QList<QByteArray>);
    void SetReady(QString);
    void GameData(QString, QByteArray);
    void GameQuit(QString);

public slots:
    void SocketClient_ReadyRead();
    void SocketClient_Disconnected();
    void GamesRequestReply(thJoueurs *, QString);

    void PlayersUpdate(QByteArray);
    void PlayersReply(thJoueurs *, QString);
    void GameBegin(QByteArray);
    void GameEnd();
    void GameSData(QByteArray);
};

#endif // THJOUEURS_H
