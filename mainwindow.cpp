#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    TCPServeur = new QTcpServer();
    connect(TCPServeur, SIGNAL(newConnection()), this, SLOT(TCPServeur_NewConnection()));
    TCPServeur->listen(QHostAddress::Any, 60123);

    ui->lbParties->addItem("Allo");
    ui->lbParties->addItem("Allo2");
}

MainWindow::~MainWindow()
{
    delete ui;
    delete TCPServeur;
}

void MainWindow::TCPServeur_NewConnection()
{
    tempthread = new thJoueurs(0,TCPServeur->nextPendingConnection());
    connect(tempthread, SIGNAL(Disconnected(thJoueurs *)), this, SLOT(CloseThreads(thJoueurs *)));
    connect(tempthread, SIGNAL(GamesRequest(thJoueurs *)), this, SLOT(thJoueurs_GamesRequest(thJoueurs*)));
    connect(this, SIGNAL(GamesRequestReply(thJoueurs *, QString)), tempthread, SLOT(GamesRequestReply(thJoueurs *, QString)));
}

void MainWindow::thJoueurs_GamesRequest(thJoueurs *Thread)
{
   QString Parties = "";
   for (int I = 0; I < ui->lbParties->count(); I++)
    Parties += ui->lbParties->item(I)->text() + "\n";
   emit GamesRequestReply(Thread, Parties);
}

void MainWindow::CloseThreads(thJoueurs *Thread)
{
    delete Thread;
}
