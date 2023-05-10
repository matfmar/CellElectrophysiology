
#include "zapisywaniewindow.h"

#include <QMessageBox>

ZapisywanieWindow::ZapisywanieWindow(Control* ctrl, QWidget *parent) : QMainWindow{parent} {
    setWindowTitle(tr("Wybór zapisywanych struktur"));

    groupIons = new QGroupBox(tr("Dostępne stężenia jonów"), this);
    label1 = new QLabel(tr("Zaznacz, które stężenia chcesz zapisywać:"), this);
    listIons = new QListWidget(this);
    listIons -> setSelectionMode(QAbstractItemView::MultiSelection);
    layoutGroup1 = new QVBoxLayout(this);
    layoutGroup1 -> addWidget(label1);
    layoutGroup1 -> addWidget(listIons);
    groupIons -> setLayout(layoutGroup1);

    groupTransporters = new QGroupBox(tr("Dostępne transportery"), this);
    label2 = new QLabel(tr("Zaznacz, prądy których transporterów chcesz zapisywać:"), this);
    listTransporters = new QListWidget(this);
    listTransporters -> setSelectionMode(QAbstractItemView::MultiSelection);
    layoutGroup2 = new QVBoxLayout(this);
    layoutGroup2 -> addWidget(label2);
    layoutGroup2 -> addWidget(listTransporters);
    groupTransporters -> setLayout(layoutGroup2);

    groupParameters = new QGroupBox(tr("Dostępne parametry"), this);
    label3 = new QLabel(tr("Zaznacz, które parametry chcesz zapisywać:"), this);
    listParameters = new QListWidget(this);
    listParameters -> setSelectionMode(QAbstractItemView::MultiSelection);
    layoutGroup3 = new QVBoxLayout(this);
    layoutGroup3 -> addWidget(label3);
    layoutGroup3 -> addWidget(listParameters);
    groupParameters -> setLayout(layoutGroup3);

    buttonDodaj = new QPushButton(tr("Dodaj"), this);
    buttonZamknij = new QPushButton(tr("Zamknij"), this);
    layoutButtons = new QHBoxLayout(this);
    layoutButtons -> addWidget(buttonDodaj);
    layoutButtons -> addWidget(buttonZamknij);

    buttonUsunWszystko = new QPushButton(tr("Usuń wszystko"), this);

    mainWidget = new QWidget(this);
    mainLayout = new QVBoxLayout(this);
    mainLayout -> addWidget(buttonUsunWszystko);
    mainLayout -> addWidget(groupIons);
    mainLayout -> addWidget(groupTransporters);
    mainLayout -> addWidget(groupParameters);
    mainLayout -> addLayout(layoutButtons);
    mainWidget -> setLayout(mainLayout);
    setCentralWidget(mainWidget);

    QObject::connect(buttonZamknij, SIGNAL(clicked()), this, SLOT(close()));
    QObject::connect(buttonDodaj, SIGNAL(clicked()), this, SLOT(onButtonDodajClicked()));
    QObject::connect(buttonUsunWszystko, SIGNAL(clicked()), this, SLOT(onButtonUsunWszystkoClicked()));

    control = ctrl;
    allIonsDoZapisywania = new std::vector<Control::DoZapisywania>;
    allTransportersDoZapisywania = new std::vector<Control::DoZapisywania>;
    allParametersDoZapisywania = new std::vector<Control::DoZapisywania>;

    onButtonUsunWszystkoClicked();  //wszystko usuwamy na wszelki wypadek

    wypelnijListeJonow();
    wypelnijListeTransporterow();
    wypelnijListeParametrow();
}

void ZapisywanieWindow::wypelnijListeJonow() {
    control -> PrepareListOfUsedIons(allIonsDoZapisywania);
    for (auto x : *allIonsDoZapisywania) {
        listIons -> addItem(x.name);
    }
    listIons -> clearSelection();
}

void ZapisywanieWindow::wypelnijListeTransporterow() {
    control -> PrepareListOfUsedTransporters(allTransportersDoZapisywania);
    for (auto x : *allTransportersDoZapisywania) {
        listTransporters -> addItem(x.name);
    }
    listParameters -> clearSelection();
}

void ZapisywanieWindow::wypelnijListeParametrow() {
    control -> PrepareListOfUsedParameters(allParametersDoZapisywania);
    for (auto x : *allParametersDoZapisywania) {
        listParameters -> addItem(x.name);
    }
    listParameters -> clearSelection();
}

void ZapisywanieWindow::onButtonDodajClicked() {
    dodajJonyDoZapisywania();
    dodajTransporteryDoZapisywania();
    dodajParametryDoZapisywania();
    QMessageBox::information(this, tr("Informacja"), tr("Dodano wszystkie struktury do zapisywania"), QMessageBox::StandardButton::Ok);
}

void ZapisywanieWindow::dodajJonyDoZapisywania() {
    int foundId(-1);
    QString selectedString("");
    for (auto x : listIons -> selectedItems()) {
        selectedString = x -> text();
        for (auto y : *allIonsDoZapisywania) {
            if (y.name == selectedString) {
                foundId = y.id;
                break;
            }
        }
        if (foundId == -1)  //straszak
            return;
        control -> AddIonDoZapisywania(foundId);
    }
}

void ZapisywanieWindow::dodajTransporteryDoZapisywania() {
    int foundId(-1);
    QString selectedString("");
    for (auto x : listTransporters -> selectedItems()) {
        selectedString = x -> text();
        for (auto y : *allTransportersDoZapisywania) {
            if (y.name == selectedString) {
                foundId = y.id;
                break;
            }
        }
        if (foundId == -1)  //straszak
            return;
        control -> AddTransporterDoZapisywania(foundId);
    }
}

void ZapisywanieWindow::dodajParametryDoZapisywania () {
    int foundId(-1);
    QString selectedString("");
    for (auto x : listParameters -> selectedItems()) {
        selectedString = x -> text();
        for (auto y : *allParametersDoZapisywania) {
            if (y.name == selectedString) {
                foundId = y.id;
                break;
            }
        }
        if (foundId == -1)  //straszak
            return;
        control -> AddParameterDoZapisywania(foundId);
    }
}

void ZapisywanieWindow::onButtonUsunWszystkoClicked() {
    control ->ClearTableDoZapisywania();
    listIons -> clearSelection();
    listParameters -> clearSelection();
    listTransporters -> clearSelection();
    QMessageBox::information(this, tr("Informacja"), tr("Wyczyszczono wszystkie dane o zapisywaniu czegokolwiek"), QMessageBox::StandardButton::Ok);
}

ZapisywanieWindow::~ZapisywanieWindow() {
    if (allIonsDoZapisywania != NULL) {
        delete allIonsDoZapisywania;
        allIonsDoZapisywania = NULL;
    }
    if (allTransportersDoZapisywania != NULL) {
        delete allTransportersDoZapisywania;
        allTransportersDoZapisywania = NULL;
    }
    if (allParametersDoZapisywania != NULL) {
        delete allParametersDoZapisywania;
        allParametersDoZapisywania = NULL;
    }
}
