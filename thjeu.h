#ifndef THJEU_H
#define THJEU_H

#include <NameSpace.h>
#include <QTimer>
#include <QThread>
#include <bataille.h>

class thJeu : public QThread
{
    Q_OBJECT
public:
    explicit thJeu(QList<QByteArray> InitData, QObject *parent = 0);
    QString NomPartie;
    int NoMap;
    int Tick;
    QList<QString> Joueurs;
    QList<bool> Ready;
    QList<Joueur *> iJoueurs;
    int SearchPlayer(QString);
    QTimer *tTick;

private:
    QList<Bataille> ListeBataille;

signals:
    void Destroy(QThread *);
    void PlayersUpdate(QByteArray);
    void GameBegin(QByteArray);
    void GameEnd();
    void SendGameSData(QByteArray);

public slots:
    void InitGame();
    void TickTimeOut();
    void ExcludePlayer(QString);
    void CumReady(QString);
    void PlayersUpdate();
    void PlayersData(QString, QByteArray);
    void EndGame(QString);
};

#endif // THJEU_H
