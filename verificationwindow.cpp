
#include "verificationwindow.h"
#include <QMessageBox>

VerificationWindow::VerificationWindow(QWidget* invoker, Control* ctrl, QWidget *parent)   : QMainWindow{parent} {
    setWindowTitle(tr("Weryfikacja budowy systemu"));

    mainWidget = new QWidget(this);
    groupAllStructures = new QGroupBox(tr("Struktury systemu"), this);
    listAllStructures = new QListWidget(this);
    layoutAll = new QVBoxLayout(this);
    layoutAll -> addWidget(listAllStructures);
    groupAllStructures -> setLayout(layoutAll);
    groupAllStructuresToBeSaved = new QGroupBox(tr("Struktury zapisywane"), this);
    listAllStructuresToBeSaved = new QListWidget(this);
    layoutToSave = new QVBoxLayout(this);
    layoutToSave -> addWidget(listAllStructuresToBeSaved);
    groupAllStructuresToBeSaved -> setLayout(layoutToSave);
    buttonZamknij = new QPushButton(tr("Jest OK"), this);
    buttonCosNieTak = new QPushButton(tr("Jest coś nie tak!"), this);
    layout = new QVBoxLayout(this);
    layout -> addWidget(groupAllStructures);
    layout -> addWidget(groupAllStructuresToBeSaved);
    layout -> addWidget(buttonZamknij);
    layout -> addWidget(buttonCosNieTak);
    mainWidget -> setLayout(layout);
    setCentralWidget(mainWidget);

    QObject::connect(buttonZamknij, SIGNAL(clicked()), this, SLOT(onButtonZamknijClicked()));
    QObject::connect(buttonCosNieTak, SIGNAL(clicked()), this, SLOT(onButtonCosNieTakClicked()));
    QObject::connect(this, SIGNAL(everythingOK()), invoker, SLOT(verificationOK()));
    QObject::connect(this, SIGNAL(somethingFuckedUp()), invoker, SLOT(verificationFAILED()));

    control = ctrl;

    allStructures = new std::vector<std::string>;
    allStructuresToBeSaved = new std::vector<std::string>;

    wypelnijListy();
}

void VerificationWindow::wypelnijListy() {
    bool result (false);
    control -> DescribeTheSystem(allStructures, allStructuresToBeSaved, result);
    if (!result) {
        QMessageBox::critical(this, tr("Błąd"), tr("Ładunki całkowite w komórce lub ECF nie bilansują się - popraw to."), QMessageBox::StandardButton::Ok);
        buttonZamknij -> setEnabled(false);
        return;
    }
    for (auto x : *allStructures) {
        listAllStructures -> addItem(QString::fromStdString(x));
    }
    for (auto x : *allStructuresToBeSaved) {
        listAllStructuresToBeSaved -> addItem(QString::fromStdString(x));
    }
}

void VerificationWindow::onButtonZamknijClicked() {
    emit everythingOK();
    close();
}

void VerificationWindow::onButtonCosNieTakClicked() {
    emit somethingFuckedUp();
    close();
}

VerificationWindow::~VerificationWindow() {
    if (allStructures != NULL) {
        delete allStructures;
        allStructures = NULL;
    }
    if (allStructuresToBeSaved != NULL) {
        delete allStructuresToBeSaved;
        allStructuresToBeSaved = NULL;
    }
}
