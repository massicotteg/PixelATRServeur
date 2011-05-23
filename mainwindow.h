#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <NameSpace.h>
#include <ui_mainwindow.h>
#include <thjeu.h>
#include <thjoueurs.h>
#include <QMainWindow>
#include <QtNetwork>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QList<thJeu *> m_Parties;

private:
    Ui::MainWindow *ui;
    QTcpServer *m_TCPServeur;
    thJoueurs *m_tempthread;
    thJeu *CherchePartie(QString);
    bool m_AccepteNouvellesConnexions;

signals:
    void ReponseADemandeParties(thJoueurs *, QString);
    void KickPartie(QByteArray, QString);
    void JoueurAjoute();
    void TerminePartie(QByteArray, QString);

private slots:
    void m_TCPServeur_NewConnection();
    void FermeObjets(QObject *objet);

    void thJoueurs_DemandeParties(thJoueurs *);
    void CreePartie(QList<QByteArray>);
    void JoinPartie(thJoueurs *, QList<QByteArray>);
    void on_btnDemarrer_clicked();
    void on_btnDemarrerArreter_clicked();
    void SupPartie(QByteArray, QString);
    void on_btnKick_clicked();
    void on_lbParties_currentRowChanged(int currentRow);
};

#endif // MAINWINDOW_H
