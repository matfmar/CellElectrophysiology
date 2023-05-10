
#include "ionswindow.h"
#include "addionwindow.h"

#include <QMessageBox>

IonsWindow::IonsWindow(Control* ctrl, QWidget *parent) : QMainWindow{parent} {
    setWindowTitle(tr("Edycja jonów"));
    mainWidget = new QWidget(this);

    label1 = new QLabel(tr("Jony w Twoim systemie:"), this);
    label2 = new QLabel(tr("Jony dostępne do wykorzystania:"), this);
    listUsedIons = new QListWidget(this);
    listPossibleIons = new QListWidget(this);
    listPossibleIons -> setSelectionMode(QAbstractItemView::SingleSelection);
    listUsedIons -> setSelectionMode(QAbstractItemView::SingleSelection);
    buttonDodaj = new QPushButton(tr("Dodaj jon"), this);
    buttonUsun = new QPushButton(tr("Usuń jon"), this);
    buttonUsun -> setEnabled(true);
    buttonZamknij = new QPushButton(tr("Zamknij"), this);

    buttonsLayout = new QVBoxLayout(this);
    listsLayout = new QVBoxLayout(this);
    layout = new QHBoxLayout(this);
    buttonsLayout -> addWidget(buttonDodaj);
    buttonsLayout -> addWidget(buttonUsun);
    buttonsLayout -> addWidget(buttonZamknij);
    listsLayout -> addWidget(label1);
    listsLayout -> addWidget(listUsedIons);
    listsLayout -> addWidget(label2);
    listsLayout -> addWidget(listPossibleIons);
    layout -> addLayout(listsLayout);
    layout -> addLayout(buttonsLayout);

    mainWidget->setLayout(layout);
    setCentralWidget(mainWidget);

    QObject::connect(buttonZamknij, SIGNAL(clicked()), this, SLOT(close()));
    QObject::connect(buttonDodaj, SIGNAL(clicked()), this, SLOT(onButtonDodajClicked()));
    QObject::connect(buttonUsun, SIGNAL(clicked()), this, SLOT(onButtonUsunClicked()));

    control = ctrl;
    tabIonsFromList = new std::vector<IonFromList>;
    wypelnijListeMozliwychJonow();
    wypelnijListeUzytychJonow();
}

void IonsWindow::wypelnijListeMozliwychJonow() {
    std::vector<QString> list = control -> GetListOfIonsFromLibrary();
    IonFromList ifl;
    int iterator(0);
    for (auto x : list) {
        ifl.id = iterator;
        ifl.name = x;
        tabIonsFromList -> push_back(ifl);
        listPossibleIons -> addItem(x);
        iterator++;
    }
}

void IonsWindow::wypelnijListeUzytychJonow() {
    std::vector<QString> list = control -> GetListOfUsedIonsFromLibrary();
    int iterator(0);
    for (auto x : list) {
        listUsedIons -> addItem(x);
    }
}

void IonsWindow::onButtonDodajClicked() {
    if (listPossibleIons->selectedItems().size() == 0) {
        QMessageBox::information(this, tr("Błąd"), tr("Nic nie wybrano!"), QMessageBox::StandardButton::Ok);
        return;
    }
    QString selectedItem = listPossibleIons->selectedItems()[0]->text();
    int selectedRow = listPossibleIons -> row(listPossibleIons -> selectedItems()[0]);
    AddIonWindow* addIonWindow = new AddIonWindow(this, control, selectedItem, selectedRow);
    addIonWindow -> show();
}

void IonsWindow::moveIonsFromPossibleToUsed(QString name) {
    listUsedIons -> addItem(name);
    buttonUsun -> setEnabled(true);
}

void IonsWindow::onButtonUsunClicked() {
    if (listUsedIons->selectedItems().size() == 0) {
        QMessageBox::information(this, tr("Błąd"), tr("Nic nie wybrano!"), QMessageBox::StandardButton::Ok);
        return;
    }
    QString selectedItem = listUsedIons->selectedItems()[0]->text();
    int selectedId(-1);
    for (auto x : *tabIonsFromList) {
        if (x.name == selectedItem) {
            selectedId = x.id;
            break;
        }
    }
    if (selectedId == -1)    //taki straszak
        return;
    if (!control -> RemoveIonFromDoStworzenia(selectedId)) {
        QMessageBox::critical(this, tr("Błąd"), tr("Błąd w usuwaniu jonu z listy!"), QMessageBox::StandardButton::Ok);
        return;
    }
    qDeleteAll(listUsedIons -> selectedItems());
}

IonsWindow::~IonsWindow() {
    if (tabIonsFromList != NULL) {
        delete tabIonsFromList;
        tabIonsFromList = NULL;
    }
}
