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
    void run();

signals:
    void Destroy(QThread *);

    void PlayersUpdate(QByteArray);
    void GameBegin();
    void GameEnd();
    void SendGameSData(QByteArray);

public slots:
    void ExcludePlayer(QString);
    void CumReady(QString);
    void tPlayersUpdate_Timeout();
    void PlayersData(QString, QByteArray);
    void EndGame(QString);
};

#endif // THJEU_H
