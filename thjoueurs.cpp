#include <thjoueurs.h>

thJoueurs::thJoueurs(QObject *parent, QTcpSocket *socketClient) :
    QThread(parent)
{
    SocketClient = socketClient;
    connect(SocketClient, SIGNAL(readyRead()), this, SLOT(SocketClient_ReadyRead()));
    connect(SocketClient, SIGNAL(disconnected()), this, SLOT(SocketClient_Disconnected()));

    GameAssigned = false;
}

void thJoueurs::SocketClient_ReadyRead()
{
    QByteArray Data = SocketClient->readAll();
    switch (Data[0])
    {
        case Ui::GameCData:
            emit GameData(Nom, Data.remove(0, 1));
            break;
        case Ui::GamesRequest:
            emit GamesRequest(this);
            break;
        case Ui::GameSetReady:
            emit SetReady(Nom);
            break;
        case Ui::GameJoin:
            emit JoinRequest(this, Data.remove(0,1).split('\n'));
            break;
        case Ui::GameCreate:
            emit CreateRequest(Data.remove(0,1).split('\n'));
            break;
        case Ui::GameQuit:
            emit GameQuit(Nom);
            Nom = "";
            GameAssigned = false;
            break;
    }
}

void thJoueurs::GamesRequestReply(thJoueurs *sender, QString Reply)
{
    if (this == sender)
    {
        QByteArray reply = QByteArray(1, Ui::GamesReply);
        reply.append(Reply);
        SocketClient->write(reply);
    }
}

void thJoueurs::PlayersReply(thJoueurs *sender, QString Reply)
{
    if (this == sender)
    {
        QByteArray reply = QByteArray(1, Ui::GamePlayers);
        reply.append(Reply);
        SocketClient->write(reply);
    }
}

void thJoueurs::GameBegin()
{
    SocketClient->write(QByteArray(1, Ui::GameBegin));
}

void thJoueurs::GameEnd()
{
    SocketClient->write(QByteArray(1, Ui::GameEnd));
}

void thJoueurs::GameSData(QByteArray Data)
{
    QByteArray reply = QByteArray(1, Ui::GameSData);
    reply.append(Data);
    SocketClient->write(reply);
}

void thJoueurs::SocketClient_Disconnected()
{
    emit GameQuit(Nom);
    emit Disconnected((this));
}

void thJoueurs::PlayersUpdate(QByteArray PlayersList)
{
    SocketClient->write(PlayersList);
}
