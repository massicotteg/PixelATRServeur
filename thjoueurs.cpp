#include <thjoueurs.h>

thJoueurs::thJoueurs(QObject *parent, QTcpSocket *socketClient) :
    QThread(parent)
{
    SocketClient = socketClient;
    connect(SocketClient, SIGNAL(readyRead()), this, SLOT(SocketClient_ReadyRead()));
    connect(SocketClient, SIGNAL(disconnected()), this, SLOT(SocketClient_Disconnected()));

}

void thJoueurs::SocketClient_ReadyRead()
{
    QByteArray Data = SocketClient->readAll();
    switch (Data[0])
    {
        case thJeu::GameCData:
            emit GameData(this);
            break;
        case thJeu::GamesRequest:
            emit GamesRequest(this);
            break;
        case thJeu::GameSetReady:
            emit SetReady(this);
            break;
        case thJeu::GameJoin:
            emit JoinRequest(this);
            break;
        case thJeu::GameCreate:
            emit CreateRequest(this);
            break;
    }
}

void thJoueurs::GamesRequestReply(thJoueurs *sender, QString Reply)
{
    if (this == sender)
    {
        QByteArray reply;
        reply.append(thJeu::GamesReply);
        reply.append(Reply);
        SocketClient->write(reply);
    }
}

void thJoueurs::PlayersReply(thJoueurs *sender, QString Reply)
{
    if (this == sender)
    {
        QByteArray reply;
        reply.append((char)1 + Reply);
        SocketClient->write(reply);
    }
}

void thJoueurs::GameBegin(int NoPartie)
{
    if (NoPartie == 1)
        SocketClient->write(QByteArray(1, (char)2));
}

void thJoueurs::GameEnd(int NoPartie)
{
    if (NoPartie == 1)
        SocketClient->write(QByteArray(1, (char)4));
}

void thJoueurs::GameSData(int NoPartie, QByteArray Data)
{
    if (NoPartie == 1)
    {
        QByteArray reply = QByteArray(1, (char)3);
        reply.append(Data);
        SocketClient->write(reply);
    }
}

void thJoueurs::SocketClient_Disconnected()
{
    emit Disconnected((this));
}
