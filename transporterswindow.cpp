
#include "transporterswindow.h"
#include "addtransporterwindow.h"

#include <QMessageBox>

TransportersWindow::TransportersWindow(Control* ctrl, QWidget *parent) : QMainWindow{parent} {
    setWindowTitle(tr("Edycja transporterów"));
    mainWidget = new QWidget(this);

    label1 = new QLabel(tr("Kliknij dwukrotnie jeden z poniższych transporterów, aby go edytować:"), this);
    label2 = new QLabel(tr("Wybierz z poniższych transporterów:"), this);
    listUsed = new QListWidget(this);
    listPossible = new QListWidget(this);
    listUsed -> setSelectionMode(QAbstractItemView::SingleSelection);
    listPossible -> setSelectionMode(QAbstractItemView::SingleSelection);
    groupTransportersPossible = new QGroupBox(tr("Możliwe transportery do użycia"), this);
    groupTransportersUsed = new QGroupBox(tr("Transportery uzywane w symulacji"), this);
    layoutPossible = new QVBoxLayout(this);
    layoutUsed = new QVBoxLayout(this);
    layoutPossible -> addWidget(label2);
    layoutPossible -> addWidget(listPossible);
    layoutUsed -> addWidget(label1);
    layoutUsed -> addWidget(listUsed);
    groupTransportersUsed -> setLayout(layoutUsed);
    groupTransportersPossible -> setLayout(layoutPossible);
    layoutAll = new QVBoxLayout(this);
    layoutAll -> addWidget(groupTransportersUsed);
    layoutAll -> addWidget(groupTransportersPossible);
    buttonDodaj = new QPushButton(tr("Dodaj"), this);
    buttonUsun = new QPushButton(tr("Usuń"), this);
    buttonUsun -> setEnabled(true);
    buttonZamknij = new QPushButton(tr("Zamknij"), this);
    layoutButtons = new QVBoxLayout(this);
    layoutButtons -> addWidget(buttonDodaj);
    layoutButtons -> addWidget(buttonUsun);
    layoutButtons -> addWidget(buttonZamknij);
    layout = new QHBoxLayout(this);
    layout -> addLayout(layoutAll);
    layout -> addLayout(layoutButtons);

    mainWidget -> setLayout(layout);
    setCentralWidget(mainWidget);

    QObject::connect(buttonZamknij, SIGNAL(clicked()), this, SLOT(close()));
    QObject::connect(buttonDodaj, SIGNAL(clicked()), this, SLOT(onButtonDodajClicked()));
    QObject::connect(buttonUsun, SIGNAL(clicked()), this, SLOT(onButtonUsunClicked()));

    control = ctrl;
    tabTransportersFromList = new std::vector<TransporterFromList>;
    wypelnijListeMozliwychTransporterow();
    wypelnijListeUzytychTransporterow();
}

void TransportersWindow::wypelnijListeMozliwychTransporterow() {    //wypelnia liste mozliwymi transporterami
    std::vector<QString> list = control -> GetListOfTransportersFromLibrary();
    TransporterFromList tfl;
    int iterator(0);
    for (auto x : list) {
        tfl.id = iterator;
        tfl.name = x;
        tabTransportersFromList -> push_back(tfl);
        listPossible -> addItem(x);
        iterator++;
    }
}

void TransportersWindow::wypelnijListeUzytychTransporterow() {
    std::vector<QString> list = control -> GetListOfUsedTransportersFromLibrary();
    int iterator(0);
    for (auto x : list) {
        listUsed -> addItem(x + ":::");
    }
}

void TransportersWindow::onButtonDodajClicked() {
    if (listPossible->selectedItems().size() == 0) {
        QMessageBox::information(this, tr("Błąd"), tr("Nic nie wybrano!"), QMessageBox::StandardButton::Ok);
        return;
    }
    QString selectedItem = listPossible->selectedItems()[0]->text();
    int selectedRow = listPossible -> row(listPossible -> selectedItems()[0]);
    AddTransporterWindow* addTransporterWindow = new AddTransporterWindow(this, control, selectedItem, selectedRow);
    addTransporterWindow -> show();
}

void TransportersWindow::onButtonUsunClicked() {
    if (listUsed->selectedItems().size() == 0) {
        QMessageBox::information(this, tr("Błąd"), tr("Nic nie wybrano!"), QMessageBox::StandardButton::Ok);
        return;
    }
    QString selectedItem = listUsed->selectedItems()[0]->text();
    int pos = selectedItem.indexOf(":::", 0, Qt::CaseInsensitive);  //i wyodrębniamy nazwę transportera żeby było do wyszukiwania
    if (pos == -1)
        return;
    QString derivedTransporterName = selectedItem.mid(0,pos);
    int selectedId(-1);
    for (auto x : *tabTransportersFromList) {
        if (x.name == derivedTransporterName) {
            selectedId = x.id;
            break;
        }
    }
    if (selectedId == -1)    //taki straszak
        return;
    if (!control -> RemoveTransporterFromDoStworzenia(selectedId)) {
        QMessageBox::critical(this, tr("Błąd"), tr("Błąd w usuwaniu transportera z listy!"), QMessageBox::StandardButton::Ok);
        return;
    }
    qDeleteAll(listUsed -> selectedItems());
}

void TransportersWindow::moveTransportersFromPossibleToUsed(QString name) {
    listUsed -> addItem(name);
    buttonUsun -> setEnabled(true);
}

TransportersWindow::~TransportersWindow() {
    if (tabTransportersFromList != NULL) {
        delete tabTransportersFromList;
        tabTransportersFromList = NULL;
    }
}
