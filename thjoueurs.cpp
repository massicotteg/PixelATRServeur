#include <thjoueurs.h>

thJoueurs::thJoueurs(QObject *parent, QTcpSocket *socketClient) :
    QThread(parent)
{
    SocketClient = socketClient;
    connect(SocketClient, SIGNAL(readyRead()), this, SLOT(SocketClient_ReadyRead()));
    connect(SocketClient, SIGNAL(disconnected()), this, SLOT(SocketClient_Disconnected()));

    GameAssigned = false;
}
thJoueurs::~thJoueurs()
{
    if (Nom != "")
        emit GameQuit(Nom);
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
            if (!GameAssigned)
                emit JoinRequest(this, Data.remove(0,1).split('\n'));
            break;
        case Ui::GameCreate:
            emit CreateRequest(Data.remove(0,1).split('\n'));
            break;
        case Ui::GameQuit:
            emit GameQuit(Nom);
            break;
    }
}

void thJoueurs::GamesRequestReply(thJoueurs *sender, QString Reply)
{
    if (this == sender || sender == 0)
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
        SocketClient->waitForBytesWritten();
    }
}

void thJoueurs::GameBegin()
{
    SocketClient->write(QByteArray(1, Ui::GameBegin));
    SocketClient->waitForBytesWritten();
}

void thJoueurs::GameEnd()
{
    GameAssigned = false;
    SocketClient->write(QByteArray(1, Ui::GameEnd));
    SocketClient->waitForBytesWritten();
}

void thJoueurs::GameSData(QByteArray Data)
{
    QByteArray reply = QByteArray(1, Ui::GameSData);
    reply.append(Data);
    SocketClient->write(reply);
    SocketClient->waitForBytesWritten();
}

void thJoueurs::SocketClient_Disconnected()
{
    emit Disconnected((this));
}

void thJoueurs::PlayersUpdate(QByteArray PlayersList)
{
    SocketClient->write(PlayersList);
    SocketClient->waitForBytesWritten();
}
