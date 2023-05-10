
#ifndef PARAMETRYWINDOW_H
#define PARAMETRYWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QGroupBox>
#include <QLabel>

#include "control.h"

class ParametryWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit ParametryWindow(Control* ctrl, QWidget *parent = nullptr);
    ~ParametryWindow() {}
public slots:
    void onButtonDodajClicked();
private:
    QGroupBox* group;
    QLabel* label1;
    QTableWidget* table;
    QHBoxLayout* layoutButtons;
    QVBoxLayout* layout,* mainLayout;
    QPushButton* buttonDodaj,* buttonZamknij;

    Control* control;

    void wypelnijTabeleParametrami();
    void wypelnijTabeleWartosciamiUzupelnionymi();
};

#endif // PARAMETRYWINDOW_H
