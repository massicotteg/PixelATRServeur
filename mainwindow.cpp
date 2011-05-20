#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Parties = QList<thJeu *>();
    AcceptNewConnections = true;

    TCPServeur = new QTcpServer();
    connect(TCPServeur, SIGNAL(newConnection()), this, SLOT(TCPServeur_NewConnection()));
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
    if (I < Parties.count())
        return Parties[I];
    else
        return 0;
}

void MainWindow::TCPServeur_NewConnection()
{
    if (AcceptNewConnections)
    {
        tempthread = new thJoueurs(TCPServeur->nextPendingConnection());
        connect(tempthread, SIGNAL(Disconnected(QObject *)), this, SLOT(CloseThreads(QObject *)));
        connect(tempthread, SIGNAL(GamesRequest(thJoueurs *)), this, SLOT(thJoueurs_GamesRequest(thJoueurs*)));
        connect(this, SIGNAL(GamesRequestReply(thJoueurs *, QString)), tempthread, SLOT(GamesRequestReply(thJoueurs *, QString)));
        connect(tempthread, SIGNAL(CreateRequest(QList<QByteArray>)), this, SLOT(GameCreate(QList<QByteArray>)));
        connect(tempthread, SIGNAL(JoinRequest(thJoueurs*,QList<QByteArray>)), this, SLOT(GameJoin(thJoueurs*,QList<QByteArray>)));
        connect(this, SIGNAL(EndGame(QByteArray,QString)), tempthread, SLOT(GameEnd(QByteArray,QString)));
    }
    else
        TCPServeur->nextPendingConnection()->abort();
}

void MainWindow::GameCreate(QList<QByteArray>Data)
{
    if (SearchGame(Data[0]) == 0)
    {
        thJeu *Temp = new thJeu(Data);
        Parties.append(Temp);
        ui->lbParties->addItem(Temp->NomPartie);

        connect(this, SIGNAL(PlayerAdded()), Temp, SLOT(PlayersUpdate()));
        connect(this, SIGNAL(KickGame(QByteArray, QString)), Temp, SLOT(EndGame(QByteArray, QString)));
        connect(Temp, SIGNAL(Destroy(QObject*)), this, SLOT(CloseThreads(QObject*)));
    }
}

void MainWindow::GameJoin(thJoueurs *Joueur, QList<QByteArray> Data)
{
    thJeu *Partie = SearchGame(Data[1]);

    if (Partie->SearchPlayer(Data[0]) == Partie->Joueurs.count() && Partie->Joueurs.count() < 4)
    {
        Partie->Joueurs.append(Data[0]);
        Joueur->Nom = Data[0];
        Partie->Ready.append(false);

        connect(Partie, SIGNAL(GameBegin(QByteArray)), Joueur, SLOT(GameBegin(QByteArray)));
        connect(Partie, SIGNAL(SendGameSData(QByteArray)), Joueur, SLOT(GameSData(QByteArray)));
        connect(Partie, SIGNAL(PlayersUpdate(QByteArray)), Joueur, SLOT(PlayersUpdate(QByteArray)));
        connect(Joueur, SIGNAL(GameQuit(QString)), Partie, SLOT(ExcludePlayer(QString)));
        connect(Joueur, SIGNAL(SetReady(QString)), Partie, SLOT(CumReady(QString)));
        connect(Joueur, SIGNAL(GameData(QString,QByteArray)), Partie, SLOT(PlayersData(QString,QByteArray)));
        connect(Partie, SIGNAL(GameEnd(QByteArray, QString)), Joueur, SLOT(GameEnd(QByteArray, QString)));
        connect(Partie, SIGNAL(GameEnd(QByteArray, QString)), this, SLOT(DelGame(QByteArray, QString)));

        emit PlayerAdded();
    }
    else
        emit EndGame(0,"");
}

void MainWindow::thJoueurs_GamesRequest(thJoueurs *Thread)
{
   QString Parties = "";
   for (int I = 0; I < ui->lbParties->count(); I++)
    Parties += ui->lbParties->item(I)->text() + "\n";
   emit GamesRequestReply(Thread, Parties);
}

void MainWindow::CloseThreads(QObject *objet)
{
    delete objet;
}

void MainWindow::on_btnAppliquer_clicked()
{

}

void MainWindow::on_btnDemarrer_clicked()
{
    ui->btnDemarrer->setEnabled(false);
    ui->txtPort->setEnabled(false);
    ui->txtMaxParties->setEnabled(false);

    TCPServeur->listen(QHostAddress::Any, ui->txtPort->text().toInt());
}

void MainWindow::on_btnDemarrerArreter_clicked()
{
    AcceptNewConnections = !AcceptNewConnections;
}

void MainWindow::on_btnKick_clicked()
{
    emit KickGame(0, ui->lbParties->item(ui->lbParties->currentRow())->text());
    Parties.removeAt(ui->lbParties->currentRow());
    delete ui->lbParties->currentItem();
}

void MainWindow::on_lbParties_currentRowChanged(int currentRow)
{
    if (currentRow != -1)
        ui->btnKick->setEnabled(true);
    else
        ui->btnKick->setEnabled(false);
}

void MainWindow::DelGame(QByteArray Donnees, QString NomPartie)
{
    QList<QListWidgetItem *> temp = ui->lbParties->findItems(NomPartie, Qt::MatchCaseSensitive);
    if (temp.count() > 0)
        temp[0]->setText(NomPartie+" Terminé");
}
