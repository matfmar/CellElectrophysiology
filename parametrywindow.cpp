
#include "parametrywindow.h"
#include <QHeaderView>
#include <QMessageBox>

ParametryWindow::ParametryWindow(Control* ctrl, QWidget *parent) : QMainWindow{parent}{
    setWindowTitle(tr("Edycja parametrów symulacji"));
    resize(700,300);

    group = new QGroupBox(tr("Parametry ogólne"), this);
    label1 = new QLabel(tr("Uzupełnij poniższe parametry"), this);
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
    QHeaderView* header = table ->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::Stretch);

    layout = new QVBoxLayout(this);
    layout -> addWidget(label1);
    layout -> addWidget(table);
    layout -> addWidget(buttonDodaj);
    layout -> addWidget(buttonZamknij);
    group -> setLayout(layout);

    setCentralWidget(group);

    QObject::connect(buttonZamknij, SIGNAL(clicked()), this, SLOT(close()));
    QObject::connect(buttonDodaj, SIGNAL(clicked()), this, SLOT(onButtonDodajClicked()));

    control = ctrl;

    wypelnijTabeleParametrami();
    wypelnijTabeleWartosciamiUzupelnionymi();
}

void ParametryWindow::wypelnijTabeleParametrami() {
    std::vector<QString> params;
    std::vector<QString> units;
    int ileParametrow(0);
    control -> GetParameterNamesAndUnits(&params, &units, ileParametrow);
    for (int i=0; i<ileParametrow; ++i) {
        table ->insertRow(table -> rowCount());
        for (int j=0; j<3; ++j) {
            switch (j) {
            case 0: {
                QTableWidgetItem* item = new QTableWidgetItem();
                item -> setText(params[i]);
                item->setFlags(item->flags() &  ~Qt::ItemIsEditable);
                table -> setItem(table->rowCount()-1, j, item);
            }
            break;
            case 1: {
                QTableWidgetItem* item = new QTableWidgetItem();
                item -> setText(units[i]);
                item->setFlags(item->flags() &  ~Qt::ItemIsEditable);
                table -> setItem(table->rowCount()-1, j, item);
            }
            break;
            case 2: {
                QTableWidgetItem* item = new QTableWidgetItem();
                if (i == 5) {
                    item -> setText("[tak/nie]");
                }
                else if (i == 0) {
                    item -> setText("0.0");
                    item->setFlags(item->flags() &  ~Qt::ItemIsEnabled);
                }
                else {
                    item -> setText("");
                }
                table -> setItem(table->rowCount()-1, j, item);
            }
            break;
            default:
                break;
            }
        }
    }
}

void ParametryWindow::wypelnijTabeleWartosciamiUzupelnionymi() {
    std::vector<QString> wartosciWprowadzone;
    std::vector<int> idsWprowadzone;
    control -> GetInsertedParameters(&wartosciWprowadzone, &idsWprowadzone);
    int i(0), id(0);
    QString dane("");
    for (auto x : idsWprowadzone) {
        dane = wartosciWprowadzone[i];
        id = idsWprowadzone[i];
        QTableWidgetItem* item = new QTableWidgetItem();
        if (id == 5) {
            if (dane.toDouble() == 1.0)
                item -> setText("tak");
            else if (dane.toDouble() == 0.0)
                item -> setText("nie");
            else
                item -> setText("tak/nie");
        }
        else if (id == 0) {
            item -> setText("0.0");
            item->setFlags(item->flags() &  ~Qt::ItemIsEnabled);
        }
        else {
            item -> setText(wartosciWprowadzone[i]);
        }
        table -> setItem(id, 2, item);
        i++;
    }
}

void ParametryWindow::onButtonDodajClicked() {
    control ->ClearParametryDoStworzenia();
    QString item("");
    double value(0.0);
    bool result(true);
    for (int i=0; i<table->rowCount(); ++i) {
        item = table -> item(i, 2)->text();
        if (item.isEmpty()) {
            QMessageBox::information(this, tr("Błąd"), (tr("Nie wypełniono wiersza: ") + QString::number(i)), QMessageBox::StandardButton::Ok);
            return;
        }
        if (i == 5) {
            result = true;
            if (item == "tak")
                value = 1.0;
            else if (item == "nie")
                value = 0.0;
            else
                result = false;
        }
        else {
            value = item.toDouble(&result);
        }
        if (!result) {
            QMessageBox::critical(this, tr("Błąd"), tr("Nieprawidłowe dane wejściowe: ") + QString::number(i), QMessageBox::StandardButton::Ok);
            return;
        }
        if (!control -> AddParameterToDoStworzenia(i, value)) {
            QMessageBox::critical(this, tr("Błąd"), tr("Błąd wprowadzania danych!"), QMessageBox::StandardButton::Ok);
            return;
        }
    }
    close();
}
