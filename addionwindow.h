
#ifndef ADDIONWINDOW_H
#define ADDIONWINDOW_H

#include <QDialog>
#include <QGroupBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include "control.h"

class AddIonWindow : public QDialog {
    Q_OBJECT
public:
    AddIonWindow(QWidget* parent, Control* ctrl, QString ionN, int selRow);
public slots:
    void onButtonDodajClicked();
signals:
    void ionInserted(QString name);
private:
    QString labelTexts[4];
    QString labelUnitsTexts[4];

    QGroupBox* group;
    QGridLayout* layout;
    QLabel* labels[4];
    QLabel* labelsUnits[4];
    QLineEdit* lineEdits[4];
    QPushButton* buttonDodaj,* buttonZamknij;
    QVBoxLayout* mainLayout;

    Control* control;
    QString ionName;
    int id;

};

#endif // ADDIONWINDOW_H
