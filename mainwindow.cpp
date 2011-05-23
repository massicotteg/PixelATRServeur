#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_Parties = QList<thJeu *>();
    m_AccepteNouvellesConnexions = true;

    m_TCPServeur = new QTcpServer();
    connect(m_TCPServeur, SIGNAL(newConnection()), this, SLOT(m_TCPServeur_NewConnection()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_TCPServeur;
}

thJeu *MainWindow::CherchePartie(QString Nom)
{
    int I = 0;
    while (I < m_Parties.count() && m_Parties[I]->m_NomPartie != Nom && m_Parties[I]->m_NomPartie != Nom + "D")
        I++;
    if (I < m_Parties.count())
        return m_Parties[I];
    else
        return 0;
}

void MainWindow::m_TCPServeur_NewConnection()
{
    if (m_AccepteNouvellesConnexions)
    {
        m_tempthread = new thJoueurs(m_TCPServeur->nextPendingConnection());
        connect(m_tempthread, SIGNAL(Disconnected(QObject *)), this, SLOT(FermeObjets(QObject *)));
        connect(m_tempthread, SIGNAL(DemandeParties(thJoueurs *)), this, SLOT(thJoueurs_DemandeParties(thJoueurs*)));
        connect(this, SIGNAL(ReponseADemandeParties(thJoueurs *, QString)), m_tempthread, SLOT(ReponseADemandeParties(thJoueurs *, QString)));
        connect(m_tempthread, SIGNAL(DemandeCreation(QList<QByteArray>)), this, SLOT(CreePartie(QList<QByteArray>)));
        connect(m_tempthread, SIGNAL(DemandeJoindre(thJoueurs*,QList<QByteArray>)), this, SLOT(JoinPartie(thJoueurs*,QList<QByteArray>)));
        connect(this, SIGNAL(TerminePartie(QByteArray,QString)), m_tempthread, SLOT(FinPartie(QByteArray,QString)));
    }
    else
        m_TCPServeur->nextPendingConnection()->abort();
}

void MainWindow::CreePartie(QList<QByteArray>Data)
{
    if (CherchePartie(Data[0]) == 0 && (ui->txtMaxParties->text().toInt() == 0 || ui->txtMaxParties->text().toInt() > ui->lbParties->count()))
    {
        thJeu *Temp = new thJeu(Data);
        m_Parties.append(Temp);
        ui->lbParties->addItem(Temp->m_NomPartie);

        connect(this, SIGNAL(JoueurAjoute()), Temp, SLOT(MetAJourJoueurs()));
        connect(this, SIGNAL(KickPartie(QByteArray, QString)), Temp, SLOT(TerminePartie(QByteArray, QString)));
        connect(Temp, SIGNAL(Detruire(QObject*)), this, SLOT(FermeObjets(QObject*)));
    }
}

void MainWindow::JoinPartie(thJoueurs *Joueur, QList<QByteArray> Data)
{
    thJeu *Partie = CherchePartie(Data[1]);

    if (Partie->ChercheJoueur(Data[0]) == Partie->m_NomJoueurs.count() && Partie->m_NomJoueurs.count() < 4)
    {
        Partie->m_NomJoueurs.append(Data[0]);
        Joueur->m_Nom = Data[0];
        Partie->m_Pret.append(false);

        connect(Partie, SIGNAL(DebutPartie(QByteArray)), Joueur, SLOT(DebutPartie(QByteArray)));
        connect(Partie, SIGNAL(EnvoiDonneesServeur(QByteArray)), Joueur, SLOT(DonneesServeur(QByteArray)));
        connect(Partie, SIGNAL(MetAJourJoueurs(QByteArray)), Joueur, SLOT(MetAJourJoueurs(QByteArray)));
        connect(Joueur, SIGNAL(QuitterPartie(QString)), Partie, SLOT(ExclureJoueur(QString)));
        connect(Joueur, SIGNAL(MettrePret(QString)), Partie, SLOT(CumPret(QString)));
        connect(Joueur, SIGNAL(DonneesPartie(QString,QByteArray)), Partie, SLOT(DonneesJoueurs(QString,QByteArray)));
        connect(Partie, SIGNAL(FinPartie(QByteArray, QString)), Joueur, SLOT(FinPartie(QByteArray, QString)));
        connect(Partie, SIGNAL(FinPartie(QByteArray, QString)), this, SLOT(SupPartie(QByteArray, QString)));

        emit JoueurAjoute();
    }
    else
        emit TerminePartie(0,"");
}

void MainWindow::thJoueurs_DemandeParties(thJoueurs *Thread)
{
   QString Parties = "";
   for (int I = 0; I < ui->lbParties->count(); I++)
       if (CherchePartie(ui->lbParties->item(I)->text()) != 0)
            Parties += ui->lbParties->item(I)->text() + "\n";
   emit ReponseADemandeParties(Thread, Parties);
}

void MainWindow::FermeObjets(QObject *objet)
{
    delete objet;
}

void MainWindow::on_btnDemarrer_clicked()
{
    if (ui->txtPort->text().toInt() != 0 && m_TCPServeur->listen(QHostAddress::Any, ui->txtPort->text().toInt()))
    {
        ui->btnDemarrer->setEnabled(false);
        ui->txtPort->setEnabled(false);
        ui->txtMaxParties->setEnabled(false);
    }
    else
        QMessageBox::warning(this, "Attention !!", "Le Serveur n'a pu demarrer sur le port choisi ! Veuillez en choisir un autre.", QMessageBox::Ok, 0);
}

void MainWindow::on_btnDemarrerArreter_clicked()
{
    if (ui->btnDemarrerArreter->text() == "Refuser Nouvelles Connections")
        ui->btnDemarrerArreter->setText("Accepter Nouvelles Connections");
    else
        ui->btnDemarrerArreter->setText("Refuser Nouvelles Connections");

    m_AccepteNouvellesConnexions = !m_AccepteNouvellesConnexions;
}

void MainWindow::on_btnKick_clicked()
{
    emit KickPartie(0, ui->lbParties->item(ui->lbParties->currentRow())->text());
    m_Parties.removeAt(ui->lbParties->currentRow());
    delete ui->lbParties->currentItem();
}

void MainWindow::on_lbParties_currentRowChanged(int currentRow)
{
    if (currentRow != -1)
        ui->btnKick->setEnabled(true);
    else
        ui->btnKick->setEnabled(false);
}

void MainWindow::SupPartie(QByteArray Donnees, QString NomPartie)
{
    QList<QListWidgetItem *> temp = ui->lbParties->findItems(NomPartie.remove(NomPartie.length()-1,1), Qt::MatchCaseSensitive);
    if (temp.count() > 0)
        temp[0]->setText(NomPartie+" Termine");
}
