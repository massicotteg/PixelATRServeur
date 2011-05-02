#include "thjoueurs.h"

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
    if (Data[0] == (char)0)
        emit GamesRequest(this);
    else
        if (Data[0] == (char)1)
            emit CreateRequest(this);
        else
            if (Data[0] == (char)2)
                emit JoinRequest(this);
            else
                if (Data[0] == (char)3)
                    emit SetReady(this);
                else
                    emit GameData(this);
}

void thJoueurs::GamesRequestReply(thJoueurs *sender, QString Reply)
{
    if (this == sender)
    {
        QByteArray reply;
        reply.append(Reply);
        SocketClient->write(reply);
    }
}

void thJoueurs::SocketClient_Disconnected()
{
    emit Disconnected((this));
}
