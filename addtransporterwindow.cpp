
#include "addtransporterwindow.h"

#include <QMessageBox>

AddTransporterWindow::AddTransporterWindow(QWidget* parent, Control* ctrl, QString trN, int selRow) {
    setWindowTitle(tr("Dodawanie nowego transportera"));
    resize(500,300);

    group = new QGroupBox(tr("Parametry transportera"), this);
    label1 = new QLabel(tr("Uzupełnij poniższe pola"), this);
    buttonDodaj = new QPushButton(tr("Dodaj"), this);
    buttonZamknij = new QPushButton(tr("Zamknij"), this);

    table = new QTableWidget(this);
    table -> setColumnCount(3);
    QStringList headers;
    headers << tr("Parametr") << tr("Jednostka") << tr("Wartość");
    table -> setHorizontalHeaderLabels(headers);
    table -> setShowGrid(true);
    table -> setSelectionMode(QAbstractItemView::SingleSelection);
    table -> setSelectionBehavior(QAbstractItemView::SelectItems);

    layout = new QVBoxLayout(this);
    layout -> addWidget(label1);
    layout -> addWidget(table);
    layout -> addWidget(buttonDodaj);
    layout -> addWidget(buttonZamknij);
    group -> setLayout(layout);

    mainLayout = new QVBoxLayout(this);
    mainLayout -> addWidget(group);
    setLayout(mainLayout);

    QObject::connect(buttonZamknij, SIGNAL(clicked()), this, SLOT(close()));
    QObject::connect(buttonDodaj, SIGNAL(clicked()), this, SLOT(onButtonDodajClicked()));
    QObject::connect(this, SIGNAL(transporterInserted(QString)), parent, SLOT(moveTransportersFromPossibleToUsed(QString)));

    control = ctrl;
    transporterName = trN;
    id = selRow;

    wypelnijListeParametrow();
}

void AddTransporterWindow::wypelnijListeParametrow() {
    std::vector<QString> parameterNames;
    std::vector<QString> parameterUnits;
    int parameterNumber;
    control -> GetListOfTransporterParameters(&parameterNames, &parameterUnits, parameterNumber, id);
    table -> setRowCount(parameterNumber);
    for (int i=0; i<parameterNumber; ++i) {
        //table ->insertRow(table -> rowCount());
        for (int j=0; j<3; ++j) {
            switch (j) {
            case 0: {
                QTableWidgetItem* item = new QTableWidgetItem();
                item -> setText(parameterNames[i]);
                item->setFlags(item->flags() &  ~Qt::ItemIsEditable);
                table -> setItem(table->rowCount()-1, j, item);
            }
                break;
            case 1: {
                QTableWidgetItem* item = new QTableWidgetItem();
                item -> setText(parameterUnits[i]);
                item->setFlags(item->flags() &  ~Qt::ItemIsEditable);
                table -> setItem(table->rowCount()-1, j, item);
            }
                break;
            case 2: {
                QTableWidgetItem* item = new QTableWidgetItem();
                item -> setText("");
                table -> setItem(table->rowCount()-1, j, item);
            }
                break;
            default:
                break;
            }
        }
        if (i != parameterNumber-1)
            table ->insertRow(table -> rowCount());
    }
    qDebug() << table ->rowCount();
}

void AddTransporterWindow::onButtonDodajClicked() {
    QString item(""), nameFinal(transporterName), a(""), b("");
    nameFinal += "::: ";
    bool result(true);
    double value(0.0);
    std::vector<double> doubles;
    for (int i=0; i<table->rowCount(); ++i) {
        a = table -> item(i, 0)-> text();
        b = table -> item(i, 1) -> text();
        item = table -> item(i, 2)->text();
        if (item.isEmpty()) {
            QMessageBox::information(this, tr("Błąd"), (tr("Nie wypełniono wiersza: ") + QString::number(i)), QMessageBox::StandardButton::Ok);
            return;
        }
        value = item.toDouble(&result);
        if (!result) {
            QMessageBox::critical(this, tr("Błąd"), tr("Nieprawidłowe dane wejściowe !"), QMessageBox::StandardButton::Ok);
            return;
        }
        nameFinal += (a + "=" + item + b + ";");
        doubles.push_back(value);
    }
    if (control -> AddTransporterToDoStworzenia(id, doubles)) {
        emit transporterInserted(nameFinal);
    }
    else {
        QMessageBox::critical(this, tr("Błąd"), tr("Błąd dodawania nowego transportera!"), QMessageBox::StandardButton::Ok);
    }
    close();
}

