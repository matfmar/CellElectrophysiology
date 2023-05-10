
#include "addionwindow.h"

#include <QMessageBox>

AddIonWindow::AddIonWindow(QWidget* parent, Control* ctrl, QString ionN, int selRow) {
    setWindowTitle(tr("Dodawanie nowego jonu"));

    labelTexts[0] = tr("Nazwa:");
    labelTexts[1] = tr("Ładunek:");
    labelTexts[2] = tr("Stężenie w komórce:");
    labelTexts[3] = tr("Stężenie poza komórką:");

    labelUnitsTexts[0] = tr(".");
    labelUnitsTexts[1] = tr("[1]");
    labelUnitsTexts[2] = tr("mmol/l");
    labelUnitsTexts[3] = tr("mmol/l");

    for (int i=0; i<4; ++i) {
        labels[i] = new QLabel(labelTexts[i], this);
        lineEdits[i] = new QLineEdit(this);
        labelsUnits[i] = new QLabel(labelUnitsTexts[i], this);
    }
    buttonDodaj = new QPushButton(tr("Dodaj"), this);
    buttonZamknij = new QPushButton(tr("Zamknij"), this);

    group = new QGroupBox(tr("Wprowadź parametry dodawanego jonu:"), this);
    layout = new QGridLayout(this);
    for (int i=0; i<4; ++i) {
        layout -> addWidget(labels[i], i+1, 0);
        layout -> addWidget(lineEdits[i], i+1, 1);
        layout -> addWidget(labelsUnits[i], i+1, 2);
    }
    layout -> addWidget(buttonDodaj, 5, 0);
    layout -> addWidget(buttonZamknij, 5, 1);
    layout -> setColumnStretch(0, 20);
    layout -> setColumnStretch(1, 20);
    group -> setLayout(layout);
    mainLayout = new QVBoxLayout(this);
    mainLayout -> addWidget(group);
    setLayout(mainLayout);

    QObject::connect(buttonZamknij, SIGNAL(clicked()), this, SLOT(close()));
    QObject::connect(buttonDodaj, SIGNAL(clicked()), this, SLOT(onButtonDodajClicked()));
    QObject::connect(this, SIGNAL(ionInserted(QString)), parent, SLOT(moveIonsFromPossibleToUsed(QString)));

    control = ctrl;
    ionName = ionN;
    id = selRow;

    QString s = ionName + " (id: " + QString::number(id) + ")";
    lineEdits[0] -> setText(s);
    lineEdits[0] -> setEnabled(false);
}

void AddIonWindow::onButtonDodajClicked() {
    for (int i=1; i<4; ++i) {
        if (lineEdits[i] -> text() == "") {
            QMessageBox::information(this, tr("Błąd"), tr("Nie wypełniono wszystkich pól!"), QMessageBox::StandardButton::Ok);
            return;
        }
    }
    bool result(true);
    double charge = (lineEdits[1] -> text()).toDouble(&result);
    if (!result) {
        QMessageBox::information(this, tr("Błąd"), tr("Źle podano dane ładunku!"), QMessageBox::StandardButton::Ok);
        return;
    }
    double cIn = (lineEdits[2]->text()).toDouble(&result);
    if (!result) {
        QMessageBox::information(this, tr("Błąd"), tr("Źle podano dane C_in!"), QMessageBox::StandardButton::Ok);
        return;
    }
    double cOut = (lineEdits[3]->text()).toDouble(&result);
    if (!result) {
        QMessageBox::information(this, tr("Błąd"), tr("Źle podano dane C_in!"), QMessageBox::StandardButton::Ok);
        return;
    }
    if (control -> AddIonToDoStworzenia(id, cIn, cOut, charge)) {
        emit ionInserted(ionName);
    }
    else {
        QMessageBox::critical(this, tr("Błąd"), tr("Błąd dodawania nowego jonu. Być może został już dodany"), QMessageBox::StandardButton::Ok);
    }
    close();
}
