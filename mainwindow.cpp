
#include "mainwindow.h"

#include "ionswindow.h"
#include "transporterswindow.h"
#include "parametrywindow.h"
#include "zapisywaniewindow.h"
#include "verificationwindow.h"
#include "simulationwindow.h"

#include <windows.h>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle(tr("Symulator komórki"));
    setWindowIcon(QIcon("favicon.ico"));
    mainWidget = new QWidget(this);

    buttonJony = new QPushButton(tr("Konfiguruj jony"), this);
    buttonTransportery = new QPushButton(tr("Konfiguruj transportery"), this);
    buttonParametry = new QPushButton(tr("Konfiguruj ogólne parametry systemu i symulacji"), this);
    buttonZapisywanie = new QPushButton(tr("Wybierz co zapisywać, a co nie"), this);
    buttonStart = new QPushButton(tr("Zbuduj system i zweryfikuj poprawność"), this);
    buttonZaczynamySymulacje = new QPushButton(tr("Zaczynamy symulację!"), this);
    buttonZaczynamySymulacje -> setEnabled(false);
    buttonWykresy = new QPushButton(tr("Rysuj wykresy z symulacji"), this);

    copyLabel = new QLabel("Copyright " + QString::fromUtf8("\u00A9") + " by M.Marzec & L.Gmurczyk", this);
    copyLabel -> setAlignment(Qt::AlignRight);

    buttonTransportery -> setEnabled(false);
    buttonParametry -> setEnabled(false);
    buttonZapisywanie -> setEnabled(false);
    buttonStart -> setEnabled(false);
    buttonZaczynamySymulacje -> setEnabled(false);
    //buttonWykresy -> setEnabled(false);

    layout = new QVBoxLayout(this);
    layout -> addWidget(buttonJony);
    layout -> addWidget(buttonTransportery);
    layout -> addWidget(buttonParametry);
    layout -> addWidget(buttonZapisywanie);
    layout -> addWidget(buttonStart);
    layout -> addWidget(buttonZaczynamySymulacje);
    layout -> addWidget(buttonWykresy);
    layout -> addWidget(copyLabel);

    mainWidget -> setLayout(layout);
    setCentralWidget(mainWidget);

    QObject::connect(buttonJony, SIGNAL(clicked()), this, SLOT(onButtonJonyClicked()));
    QObject::connect(buttonTransportery, SIGNAL(clicked()), this, SLOT(onButtonTransporteryClicked()));
    QObject::connect(buttonParametry, SIGNAL(clicked()), this, SLOT(onButtonParametryClicked()));
    QObject::connect(buttonZapisywanie, SIGNAL(clicked()), this, SLOT(onButtonZapisywanieClicked()));
    QObject::connect(buttonStart, SIGNAL(clicked()), this, SLOT(onButtonStartClicked()));
    QObject::connect(buttonZaczynamySymulacje, SIGNAL(clicked()), this, SLOT(onButtonZaczynamySymulacjeClicked()));
    QObject::connect(buttonWykresy, SIGNAL(clicked()), this, SLOT(onButtonWykresyClicked()));

    control = new Control();    //creates interface to library and initializes the library
}

void MainWindow::onButtonJonyClicked() {
    buttonTransportery -> setEnabled(true);
    IonsWindow* ionsWindow = new IonsWindow(control, this);
    ionsWindow -> show();
}

void MainWindow::onButtonTransporteryClicked() {
    buttonParametry -> setEnabled(true);
    TransportersWindow* transportersWindow = new TransportersWindow(control, this);
    transportersWindow -> show();
}

void MainWindow::onButtonParametryClicked() {
    buttonZapisywanie -> setEnabled(true);
    ParametryWindow* parametryWindow = new ParametryWindow(control, this);
    parametryWindow -> show();
}

void MainWindow::onButtonZapisywanieClicked() {
    buttonStart -> setEnabled(true);
    ZapisywanieWindow* zapisywanieWindow = new ZapisywanieWindow(control, this);
    zapisywanieWindow -> show();
}

void MainWindow::onButtonStartClicked() {
    buttonStart -> setEnabled(false);
    buttonJony -> setEnabled(false);
    buttonTransportery -> setEnabled(false);
    buttonParametry -> setEnabled(false);
    buttonZapisywanie -> setEnabled(false);
    if (!control -> CheckIfTablesAreFull()) {
        QMessageBox::information(this, tr("Błąd"), tr("Któraś z tabeli do symulacji jest pusta. Popraw się."), QMessageBox::StandardButton::Ok);
        return;
    }
    control -> CreateSystem();
    control -> CreateEverythingInTheSystem();
    //now verify
    VerificationWindow* verificationWindow = new VerificationWindow(this, control, this);
    verificationWindow -> show();
}

void MainWindow::verificationOK() {
    QMessageBox::information(this, tr("Informacja"), tr("Teraz kliknij <Zaczynamy symulację!>, aby rozpocząć symulację!"), QMessageBox::StandardButton::Ok);
    buttonZaczynamySymulacje -> setEnabled(true);
}

void MainWindow::verificationFAILED() {
    int status = QMessageBox::question(this, tr("Informacja"),
        tr("Coś jest nie tak z systemem. Czy chcesz wszystko usunąć i zacząć budować go od nowa?"),
        QMessageBox::No|QMessageBox::Default, QMessageBox::No|QMessageBox::Escape,
        QMessageBox::Yes);
    if (status == QMessageBox::Yes) {
        control -> ClearEverything();
        QMessageBox::information(this, tr("Informacja"), tr("Wyczyszczono wszystko. Możesz budować system od nowa"), QMessageBox::StandardButton::Ok);
    }
    buttonZaczynamySymulacje -> setEnabled(false);
}

void MainWindow::onButtonZaczynamySymulacjeClicked() {
    buttonZaczynamySymulacje -> setEnabled(false);
    buttonWykresy -> setEnabled(true);
    SimulationWindow* simulationWindow = new SimulationWindow(control, this);
    simulationWindow -> show();
}

void MainWindow::onButtonWykresyClicked() {
    system(".\\wykresy.exe");
}

MainWindow::~MainWindow() {
    delete control; control = NULL;
}



