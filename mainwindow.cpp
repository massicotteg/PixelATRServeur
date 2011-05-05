#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    TCPServeur = new QTcpServer();
    connect(TCPServeur, SIGNAL(newConnection()), this, SLOT(TCPServeur_NewConnection()));
    TCPServeur->listen(QHostAddress::Any, 60123);

    Parties = QList<thJeu *>();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete TCPServeur;
}

thJeu *MainWindow::SearchGame(QString Nom)
{
    int I = 0;
    while (I < Parties.count() && Parties[I]->NomPartie != Nom)
        I++;
    return Parties[I];
}

void MainWindow::TCPServeur_NewConnection()
{
    tempthread = new thJoueurs(0,TCPServeur->nextPendingConnection());
    connect(tempthread, SIGNAL(Disconnected(QThread *)), this, SLOT(CloseThreads(QThread *)));
    connect(tempthread, SIGNAL(GamesRequest(thJoueurs *)), this, SLOT(thJoueurs_GamesRequest(thJoueurs*)));
    connect(this, SIGNAL(GamesRequestReply(thJoueurs *, QString)), tempthread, SLOT(GamesRequestReply(thJoueurs *, QString)));
    connect(tempthread, SIGNAL(CreateRequest(QList<QByteArray>)), this, SLOT(GameCreate(QList<QByteArray>)));
    connect(tempthread, SIGNAL(JoinRequest(thJoueurs*,QList<QByteArray>)), this, SLOT(GameJoin(thJoueurs*,QList<QByteArray>)));
}

void MainWindow::GameCreate(QList<QByteArray>Data)
{
    Parties.append((new thJeu(Data)));
    ui->lbParties->addItem(Parties.last()->NomPartie);
}
void MainWindow::GameJoin(thJoueurs *Joueur, QList<QByteArray> Data)
{
    Joueur->Partie = SearchGame(Data[1]);
    Joueur->Partie->Joueurs.append(Data[0]);
    Joueur->Nom = Data[0];
    Joueur->Partie->Ready.append(false);

    connect(Joueur->Partie, SIGNAL(GameBegin()), Joueur, SLOT(GameBegin()));
    connect(Joueur->Partie, SIGNAL(SendGameSData(QByteArray)), Joueur, SLOT(GameSData(QByteArray)));
    connect(Joueur->Partie, SIGNAL(PlayersUpdate(QByteArray)), Joueur, SLOT(PlayersUpdate(QByteArray)));
    connect(Joueur, SIGNAL(GameQuit(QString)), Joueur->Partie, SLOT(ExcludePlayer(QString)));
    connect(Joueur, SIGNAL(SetReady(QString)), Joueur->Partie, SLOT(CumReady(QString)));
    connect(Joueur, SIGNAL(GameData(QString,QByteArray)), Joueur->Partie, SLOT(PlayersData(QString,QByteArray)));
}

void MainWindow::thJoueurs_GamesRequest(thJoueurs *Thread)
{
   QString Parties = "";
   for (int I = 0; I < ui->lbParties->count(); I++)
    Parties += ui->lbParties->item(I)->text() + "\n";
   emit GamesRequestReply(Thread, Parties);
}

void MainWindow::CloseThreads(QThread *Thread)
{
    delete Thread;
}
