#ifndef THJOUEURS_H
#define THJOUEURS_H

#include <NameSpace.h>
#include <QThread>
#include <QtNetwork>

class thJoueurs : public QObject
{
    Q_OBJECT
public:
    explicit thJoueurs(QTcpSocket *socketClient=0);
    ~thJoueurs();
    QString Nom;

private:
    QByteArray ToQByteArray(int);
    int ToInt(QByteArray);
    QTcpSocket *SocketClient;
    bool GameAssigned;

signals:
    void Disconnected(QObject *);

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
    void GameEnd(QByteArray, QString);
    void GameSData(QByteArray);
};

#endif // THJOUEURS_H
