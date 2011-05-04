#include "mainwindow.h"

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
    while (I < Parties.count() && Parties[I]->NomPartie == Nom)
        I++;
    return Parties[I];
}

void MainWindow::TCPServeur_NewConnection()
{
    tempthread = new thJoueurs(0,TCPServeur->nextPendingConnection());
    connect(tempthread, SIGNAL(Disconnected(thJoueurs *)), this, SLOT(CloseThreads(thJoueurs *)));
    connect(tempthread, SIGNAL(GamesRequest(thJoueurs *)), this, SLOT(thJoueurs_GamesRequest(thJoueurs*)));
    connect(this, SIGNAL(GamesRequestReply(thJoueurs *, QString)), tempthread, SLOT(GamesRequestReply(thJoueurs *, QString)));
}

void MainWindow::GameCreate(QList<QByteArray>Data)
{
    Parties.append(new thJeu(Data));
}
void MainWindow::GameJoin(thJoueurs *Joueur, QList<QByteArray> Data)
{
    Joueur->Partie = SearchGame(Data[1]);
    Joueur->Partie->Joueurs.append(Data[0]);

    connect(Joueur->Partie, SIGNAL(GameBegin()), Joueur, SLOT(GameBegin()));
    connect(Joueur->Partie, SIGNAL(SendGameSData(thJeu*,QByteArray)), Joueur, SLOT(GameSData(thJeu*,QByteArray)));
    connect(Joueur, SIGNAL(GameQuit(QString)), Joueur->Partie, SLOT(ExcludePlayer(QString)));
    //connect(Joueur, SIGNAL(SetReady(QString)), Joueur->Partie, SLOT());
    //connect(Joueur, SIGNAL(GameData(QString,QByteArray)), Joueur->Partie, SLOT());
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
