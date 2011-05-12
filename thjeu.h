#ifndef THJEU_H
#define THJEU_H

#include <NameSpace.h>
#include <QTimer>
#include <QThread>
#include <joueur.h>

class thJeu : public QThread
{
    Q_OBJECT
public:
    explicit thJeu(QList<QByteArray> InitData, QObject *parent = 0);
    QString NomPartie;
    int NoMap;
    QList<QString> Joueurs;
    QList<bool> Ready;
    QList<Joueur> iJoueurs;
    int SearchPlayer(QString);

private:
    void run();
    void InitGame();

signals:
    void Destroy(QThread *);

    void PlayersUpdate(QByteArray);
    void GameBegin();
    void GameEnd();
    void SendGameSData(QByteArray);

public slots:
    void ExcludePlayer(QString);
    void CumReady(QString);
    void PlayersUpdate();
    void PlayersData(QString, QByteArray);
    void EndGame(QString);
};

#endif // THJEU_H
