#ifndef THJEU_H
#define THJEU_H

#include <NameSpace.h>
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

private:
    int SearchPlayer(QString);

signals:
    void SendGameSData(thJeu *, QByteArray);
    void GameBegin();

public slots:
    void ExcludePlayer(QString);
    void CumReady(QString);
};

#endif // THJEU_H
