
#ifndef SIMULATIONWINDOW_H
#define SIMULATIONWINDOW_H


#include <QMainWindow>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QLabel>
#include <QPushButton>
#include <QProgressBar>

#include <QThread>
#include <QMutex>

#include "control.h"

//SIMULATION THREAD
class SimulationThread : public QObject {
    Q_OBJECT
public:
    SimulationThread(Control* ctrl);
    ~SimulationThread(){}
public slots:
    void runHard();
signals:
    void finished();
private:
    Control* control;
};

//FILLING THREAD
class FillingThread : public QObject {
    Q_OBJECT
public:
    FillingThread(QMutex* mt, std::vector<QString>* vS, bool* lms, bool* vr, double* ods);
    void setModuloDoZapisywania(int a) {moduloDoZapisywania = a;}
    ~FillingThread(){}
public slots:
    void runHard();
signals:
    void readyToFillTheTable();
    void finished();
private:
    std::vector<QString>* vecShared;
    QMutex* mutex;
    bool* letMeStop;
    bool* vecReady;
    double* odsetek;
    int moduloDoZapisywania;
};

//WINDOW
class SimulationWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit SimulationWindow(Control* ctrl, QWidget *parent = nullptr);
    ~SimulationWindow();

    bool* stoppingVariablePtr;
    std::vector<QString>* vecShared;
    bool* letMeStop;
    QMutex* mutex;
    bool* vecReady;
    double* odsetek;
public slots:
    void onButtonStartClicked();
    void onButtonStopClicked();
    void fillTheTable();
    void simulationFinished();
private:
    QGroupBox* group;
    QTableWidget* table;
    QPushButton* buttonStart,* buttonStop,* buttonZamknij;
    QLabel* label1;
    QVBoxLayout* layoutGroup,* layoutMain;
    QProgressBar* progressBar;
    QWidget* mainWidget;

    Control* control;
    SimulationThread* simThread;
    FillingThread* filThread;
    QThread* thread1,* thread2;

    void wypelnijTabele();
    void wyliczCzasy();
};

#endif // SIMULATIONWINDOW_H
