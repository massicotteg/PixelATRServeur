#include <thjoueurs.h>

thJoueurs::thJoueurs(QTcpSocket *socketClient)
{
    m_SocketClient = socketClient;
    connect(m_SocketClient, SIGNAL(readyRead()), this, SLOT(m_SocketClient_ReadyRead()));
    connect(m_SocketClient, SIGNAL(disconnected()), this, SLOT(m_SocketClient_Disconnected()));

    m_PartieAssignee = false;
}
thJoueurs::~thJoueurs()
{
    if (m_Nom != "")
        emit QuitterPartie(m_Nom);
}

int thJoueurs::ToInt(QByteArray Donnees)
{
    int d[4];
    int Res = 0;
    d[0] = (((int)((uchar)Donnees[0])) << 24);
    d[1] = (((int)((uchar)Donnees[1])) << 16);
    d[2] = (((int)((uchar)Donnees[2])) << 8);
    d[3] = ((int)((uchar)Donnees[3]));
    for (int I = 0; I < 4; I++)
        Res += d[I];
    return Res;
}
void thJoueurs::m_SocketClient_ReadyRead()
{
    while (m_SocketClient->bytesAvailable())
    {
        QByteArray Donnees = m_SocketClient->read(ToInt(m_SocketClient->read(4)));
        switch (Donnees[0])
        {
            case Ui::DonneesClient:
                emit DonneesPartie(m_Nom, Donnees.remove(0, 1));
                break;
            case Ui::DemandeParties:
                emit DemandeParties(this);
                break;
            case Ui::MettrePret:
                emit MettrePret(m_Nom);
                break;
            case Ui::JoinPartie:
                if (!m_PartieAssignee)
                    emit DemandeJoindre(this, Donnees.remove(0,1).split('\n'));
                break;
            case Ui::CreePartie:
                emit DemandeCreation(Donnees.remove(0,1).split('\n'));
                break;
            case Ui::QuitterPartie:
                emit QuitterPartie(m_Nom);
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
void thJoueurs::ReponseADemandeParties(thJoueurs *Concerne, QString Reponse)
{
    if (this == Concerne || Concerne == 0)
    {
        QByteArray envoi = QByteArray(1, Ui::ReponseParties);
        envoi.append(Reponse);

        envoi.insert(0, ToQByteArray(envoi.count()));
        m_SocketClient->write(envoi);
        m_SocketClient->waitForBytesWritten();
    }
}

void thJoueurs::JoueursPartie(thJoueurs *Concerne, QString Reponse)
{
    if (this == Concerne)
    {
        QByteArray envoi = QByteArray(1, Ui::JoueursPartie);
        envoi.append(Reponse);

        envoi.insert(0, ToQByteArray(envoi.count()));
        m_SocketClient->write(envoi);
        m_SocketClient->waitForBytesWritten();
    }
}

void thJoueurs::DebutPartie(QByteArray Initialisation)
{
    QByteArray envoi = QByteArray(1, Ui::DebutPartie);

    envoi.insert(0, ToQByteArray(envoi.count()));
    m_SocketClient->write(envoi);
    m_SocketClient->waitForBytesWritten();
    envoi = QByteArray(1, Ui::DonneesServeur);
    envoi.append(Initialisation);

    envoi.insert(0, ToQByteArray(envoi.count()));
    m_SocketClient->write(envoi);
    m_SocketClient->waitForBytesWritten();
}

void thJoueurs::FinPartie(QByteArray Donnees, QString NomPartie)
{
    QByteArray envoi1 = QByteArray(1, Ui::DonneesServeur);
    envoi1.append(Donnees);
    envoi1.insert(0, ToQByteArray(envoi1.count()));

    QByteArray envoi2 = QByteArray(1, Ui::FinPartie);
    envoi2.insert(0, ToQByteArray(envoi2.count()));

    QByteArray envoi;
    if (Donnees != 0)
        envoi.append(envoi1);
    envoi.append(envoi2);
    m_PartieAssignee = false;

    m_SocketClient->write(envoi);
    m_SocketClient->waitForBytesWritten();
}

void thJoueurs::DonneesServeur(QByteArray Donnees)
{
    QByteArray envoi = QByteArray(1, Ui::DonneesServeur);
    envoi.append(Donnees);

    envoi.insert(0, ToQByteArray(envoi.count()));
    m_SocketClient->write(envoi);
    m_SocketClient->waitForBytesWritten();
}

void thJoueurs::m_SocketClient_Disconnected()
{
    emit Disconnected((this));
}

void thJoueurs::MetAJourJoueurs(QByteArray ListeJoueurs)
{
    ListeJoueurs.insert(0, ToQByteArray(ListeJoueurs.count()));
    m_SocketClient->write(ListeJoueurs);
    m_SocketClient->waitForBytesWritten();
}
