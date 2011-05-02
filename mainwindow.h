#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <NameSpace.h>
#include <QMainWindow>
#include <QtNetwork>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTcpServer *TCPServeur;
    QThread *tempthread;

signals:
    void GamesRequestReply(thJoueurs *, QString);

private slots:
    void TCPServeur_NewConnection();
    void CloseThreads(thJoueurs *Thread);
    void thJoueurs_GamesRequest(thJoueurs *);
};

#endif // MAINWINDOW_H
