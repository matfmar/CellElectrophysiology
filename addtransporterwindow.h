
#ifndef ADDTRANSPORTERWINDOW_H
#define ADDTRANSPORTERWINDOW_H

#include <QDialog>
#include <QTableWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QGroupBox>
#include <QLabel>

#include "control.h"

class AddTransporterWindow : public QDialog {
    Q_OBJECT
public:
    AddTransporterWindow(QWidget* parent, Control* ctrl, QString trN, int selRow);
    ~AddTransporterWindow() {}
public slots:
    void onButtonDodajClicked();
signals:
    void transporterInserted(QString name);
private:
    QGroupBox* group;
    QLabel* label1;
    QTableWidget* table;
    QHBoxLayout* layoutButtons;
    QVBoxLayout* layout,* mainLayout;
    QPushButton* buttonDodaj,* buttonZamknij;

    Control* control;
    QString transporterName;
    int id;
    std::vector<double> transporterParameters;

    void wypelnijListeParametrow();
};

#endif // ADDTRANSPORTERWINDOW_H
