
#ifndef ZAPISYWANIEWINDOW_H
#define ZAPISYWANIEWINDOW_H


#include <QMainWindow>
#include <QListWidget>
#include <QLabel>
#include <QGroupBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "control.h"

class ZapisywanieWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit ZapisywanieWindow(Control* ctrl, QWidget *parent = nullptr);
    ~ZapisywanieWindow();
public slots:
    void onButtonDodajClicked();
    void onButtonUsunWszystkoClicked();
private:
    QWidget* mainWidget;
    QGroupBox* groupIons,* groupTransporters,* groupParameters;
    QPushButton* buttonDodaj,* buttonZamknij,* buttonUsunWszystko;
    QLabel* label1,* label2,* label3;
    QListWidget* listIons,* listTransporters,* listParameters;
    QVBoxLayout* layoutGroup1,* layoutGroup2,* layoutGroup3,* mainLayout;
    QHBoxLayout* layoutButtons;

    Control* control;
    std::vector<Control::DoZapisywania>* allIonsDoZapisywania;
    std::vector<Control::DoZapisywania>* allTransportersDoZapisywania;
    std::vector<Control::DoZapisywania>* allParametersDoZapisywania;

    void wypelnijListeJonow();
    void wypelnijListeTransporterow();
    void wypelnijListeParametrow();

    void dodajJonyDoZapisywania();
    void dodajTransporteryDoZapisywania();
    void dodajParametryDoZapisywania();
};

#endif // ZAPISYWANIEWINDOW_H
