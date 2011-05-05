#ifndef THJEU_H
#define THJEU_H

#include <NameSpace.h>
#include <QTimer>
#include <QThread>

class thJeu : public QThread
{
    Q_OBJECT
public:
    explicit thJeu(QList<QByteArray> InitData, QObject *parent = 0);
    QString NomPartie;
    int NoMap;
    QList<QString> Joueurs;
    QList<bool> Ready;
    QTimer *tPlayersUpdate;

private:
    int SearchPlayer(QString);

signals:
    void SendGameSData(QByteArray);
    void GameBegin();
    void PlayersUpdate(QByteArray);

public slots:
    void ExcludePlayer(QString);
    void CumReady(QString);
    void tPlayersUpdate_Timeout();
    void PlayersData(QString, QByteArray);
};

#endif // THJEU_H
