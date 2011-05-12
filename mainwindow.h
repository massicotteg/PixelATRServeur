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

    QList<thJeu *> Parties;

private:
    Ui::MainWindow *ui;
    QTcpServer *TCPServeur;
    thJoueurs *tempthread;
    thJeu *SearchGame(QString);
    bool AcceptNewConnections;
    QString SelectedGame;

signals:
    void GamesRequestReply(thJoueurs *, QString);
    void KickGame(QString);
    void PlayerAdded();
    void EndGame();

private slots:
    void TCPServeur_NewConnection();
    void CloseThreads(QThread *Thread);

    void thJoueurs_GamesRequest(thJoueurs *);
    void GameCreate(QList<QByteArray>);
    void GameJoin(thJoueurs *, QList<QByteArray>);
    void on_btnAppliquer_clicked();
    void on_btnDemarrer_clicked();
    void on_btnDemarrerArreter_clicked();
    void on_btnKick_clicked();
    void on_lbParties_currentRowChanged(int currentRow);
};

#endif // MAINWINDOW_H
