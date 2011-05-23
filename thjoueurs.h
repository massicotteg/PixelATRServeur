#ifndef THJOUEURS_H
#define THJOUEURS_H

#include <NameSpace.h>
#include <QThread>
#include <QtNetwork>

class thJoueurs : public QObject
{
    Q_OBJECT
public:
    explicit thJoueurs(QTcpSocket *socketClient);
    ~thJoueurs();
    QString m_Nom;

private:
    QByteArray ToQByteArray(int);
    int ToInt(QByteArray);
    QTcpSocket *m_SocketClient;
    bool m_PartieAssignee;

signals:
    void Disconnected(QObject *);

    void DemandeParties(thJoueurs *);
    void DemandeCreation(QList<QByteArray>);
    void DemandeJoindre(thJoueurs *, QList<QByteArray>);
    void MettrePret(QString);
    void DonneesPartie(QString, QByteArray);
    void QuitterPartie(QString);

public slots:
    void m_SocketClient_ReadyRead();
    void m_SocketClient_Disconnected();
    void ReponseADemandeParties(thJoueurs *, QString);

    void MetAJourJoueurs(QByteArray);
    void JoueursPartie(thJoueurs *, QString);
    void DebutPartie(QByteArray);
    void FinPartie(QByteArray, QString);
    void DonneesServeur(QByteArray);
};

#endif // THJOUEURS_H
