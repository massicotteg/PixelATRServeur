#ifndef THJEU_H
#define THJEU_H

#include <NameSpace.h>
#include <QTimer>
#include <bataille.h>

class thJeu: public QObject
{
    Q_OBJECT
public:
    explicit thJeu(QList<QByteArray> InitData);
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
    void Destroy(QObject *);
    void PlayersUpdate(QByteArray);
    void GameBegin(QByteArray);
    void GameEnd(QByteArray, QString);
    void SendGameSData(QByteArray);

public slots:
    void InitGame();
    void TickTimeOut();
    void ExcludePlayer(QString);
    void CumReady(QString);
    void PlayersUpdate();
    void PlayersData(QString, QByteArray);
    void EndGame(QByteArray, QString);
};

#endif // THJEU_H
