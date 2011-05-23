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
    QString m_NomPartie;
    int m_NoMap;
    int m_Tick;
    QList<QString> m_NomJoueurs;
    QList<bool> m_Pret;
    QList<Joueur *> m_Joueurs;
    int ChercheJoueur(QString);
    QTimer *m_tTick;

private:
    QList<Bataille> m_ListeBataille;

signals:
    void Detruire(QObject *);
    void MetAJourJoueurs(QByteArray);
    void DebutPartie(QByteArray);
    void FinPartie(QByteArray, QString);
    void EnvoiDonneesServeur(QByteArray);

public slots:
    void InitPartie();
    void TickTimeOut();
    void ExclureJoueur(QString);
    void CumPret(QString);
    void MetAJourJoueurs();
    void DonneesJoueurs(QString, QByteArray);
    void TerminePartie(QByteArray, QString);
};

#endif // THJEU_H
