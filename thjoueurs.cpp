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

int thJoueurs::ToInt(QByteArray Data)
{
    int d[4];
    int Res = 0;
    d[0] = (((int)((uchar)Data[0])) << 24);
    d[1] = (((int)((uchar)Data[1])) << 16);
    d[2] = (((int)((uchar)Data[2])) << 8);
    d[3] = ((int)((uchar)Data[3]));
    for (int I = 0; I < 4; I++)
        Res += d[I];
    return Res;
}
void thJoueurs::SocketClient_ReadyRead()
{
    while (SocketClient->bytesAvailable())
    {
        QByteArray Data = SocketClient->read(ToInt(SocketClient->read(4)));
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
}

QByteArray thJoueurs::ToQByteArray(int Longueur)
{
    QByteArray Res = QByteArray(1, ((char)(Longueur >> 24)));
    Res.append(QByteArray(1, ((char)(Longueur >> 16))));
    Res.append(QByteArray(1, ((char)(Longueur >> 8))));
    Res.append(QByteArray(1, (char)(Longueur)));
    return  Res;
}
void thJoueurs::GamesRequestReply(thJoueurs *sender, QString Reply)
{
    if (this == sender || sender == 0)
    {
        QByteArray reply = QByteArray(1, Ui::GamesReply);
        reply.append(Reply);

        reply.insert(0, ToQByteArray(reply.count()));
        SocketClient->write(reply);
        SocketClient->waitForBytesWritten();
    }
}

void thJoueurs::PlayersReply(thJoueurs *sender, QString Reply)
{
    if (this == sender)
    {
        QByteArray reply = QByteArray(1, Ui::GamePlayers);
        reply.append(Reply);

        reply.insert(0, ToQByteArray(reply.count()));
        SocketClient->write(reply);
        SocketClient->waitForBytesWritten();
    }
}

void thJoueurs::GameBegin(QByteArray Initialisation)
{
    QByteArray reply = QByteArray(1, Ui::GameBegin);

    reply.insert(0, ToQByteArray(reply.count()));
    SocketClient->write(reply);
    SocketClient->waitForBytesWritten();
    reply = QByteArray(1, Ui::GameSData);
    reply.append(Initialisation);

    reply.insert(0, ToQByteArray(reply.count()));
    SocketClient->write(reply);
    SocketClient->waitForBytesWritten();
}

void thJoueurs::GameEnd()
{
    QByteArray reply = QByteArray(1, Ui::GameEnd);
    GameAssigned = false;

    reply.insert(0, ToQByteArray(reply.count()));
    SocketClient->write(reply);
    SocketClient->waitForBytesWritten();
}

void thJoueurs::GameSData(QByteArray Data)
{
    QByteArray reply = QByteArray(1, Ui::GameSData);
    reply.append(Data);

    reply.insert(0, ToQByteArray(reply.count()));
    SocketClient->write(reply);
    SocketClient->waitForBytesWritten();
}

void thJoueurs::SocketClient_Disconnected()
{
    emit Disconnected((this));
}

void thJoueurs::PlayersUpdate(QByteArray PlayersList)
{
    PlayersList.insert(0, ToQByteArray(PlayersList.count()));
    SocketClient->write(PlayersList);
    SocketClient->waitForBytesWritten();
}
