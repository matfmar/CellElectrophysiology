
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

#include "control.h"

class MainWindow : public QMainWindow{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void onButtonJonyClicked();
    void onButtonTransporteryClicked();
    void onButtonParametryClicked();
    void onButtonZapisywanieClicked();
    void onButtonStartClicked();
    void onButtonZaczynamySymulacjeClicked();
    void onButtonWykresyClicked();

    void verificationOK();
    void verificationFAILED();
private:
    QWidget* mainWidget;
    QVBoxLayout* layout;
    QPushButton* buttonJony;
    QPushButton* buttonTransportery;
    QPushButton* buttonParametry;
    QPushButton* buttonZapisywanie;
    QPushButton* buttonStart;
    QPushButton* buttonZaczynamySymulacje;
    QPushButton* buttonWykresy;
    QLabel* copyLabel;

    Control* control;
};

#endif // MAINWINDOW_H
