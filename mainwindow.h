#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <NameSpace.h>
#include <ui_mainwindow.h>
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
    QThread *tempthread;
    thJeu *SearchGame(QString);

signals:
    void GamesRequestReply(thJoueurs *, QString);

private slots:
    void TCPServeur_NewConnection();
    void CloseThreads(QThread *Thread);
    void thJoueurs_GamesRequest(thJoueurs *);
    void GameCreate(QList<QByteArray>);
    void GameJoin(thJoueurs *, QList<QByteArray>);
};

#endif // MAINWINDOW_H
