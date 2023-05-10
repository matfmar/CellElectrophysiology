
#include "simulationwindow.h"

#include <QMessageBox>
#include <QHeaderView>

SimulationWindow::SimulationWindow(Control* ctrl, QWidget *parent) : QMainWindow{parent} {
    setWindowTitle(tr("Symulacja"));
    resize(800, 800);

    group = new QGroupBox(tr("Zmienne symulacji"), this);
    label1 = new QLabel(tr("Naciśnij START, zeby rozpocząć symulację"), this);
    buttonStart = new QPushButton(tr("START"), this);

    table = new QTableWidget(this);
    table -> setSelectionMode(QAbstractItemView::NoSelection);
    table -> setColumnCount(3);
    QStringList headers;
    headers << tr("Zmienna") << tr("Wartość") << tr("Jednostka");
    table -> setHorizontalHeaderLabels(headers);
    table -> setShowGrid(true);
    table -> setSelectionBehavior(QAbstractItemView::SelectItems);
    QHeaderView* header = table ->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::Stretch);

    buttonStop = new QPushButton(tr("STOP"), this);
    buttonStop -> setEnabled(false);
    buttonZamknij = new QPushButton(tr("Zamknij"), this);

    progressBar = new QProgressBar(this);
    progressBar -> setRange(0, 1000);
    progressBar -> setValue(0);

    layoutGroup = new QVBoxLayout(this);
    layoutGroup -> addWidget(table);
    layoutGroup -> addWidget(progressBar);
    layoutGroup -> addWidget(buttonStop);
    group -> setLayout(layoutGroup);
    layoutMain = new QVBoxLayout(this);
    layoutMain -> addWidget(label1);
    layoutMain -> addWidget(buttonStart);
    layoutMain -> addWidget(group);
    layoutMain -> addWidget(buttonZamknij);
    mainWidget = new QWidget(this);
    mainWidget -> setLayout(layoutMain);
    setCentralWidget(mainWidget);

    control = ctrl;
    stoppingVariablePtr = new bool(false);
    letMeStop = new bool(false);
    vecShared = new std::vector<QString>;
    mutex = new QMutex();
    vecReady = new bool(false);
    odsetek = new double(0.0);

    thread1 = new QThread();
    thread2 = new QThread();
    filThread = new FillingThread(mutex, vecShared, letMeStop, vecReady, odsetek);
    simThread = new SimulationThread(control);
    filThread -> moveToThread(thread2);
    simThread -> moveToThread(thread1);

    QObject::connect(buttonStart, SIGNAL(clicked()), this, SLOT(onButtonStartClicked()));
    QObject::connect(buttonStop, SIGNAL(clicked()), this, SLOT(onButtonStopClicked()));
    QObject::connect(buttonZamknij, SIGNAL(clicked()), this, SLOT(close()));

    QObject::connect(thread1, SIGNAL(started()), simThread, SLOT(runHard()));
    QObject::connect(simThread, SIGNAL(finished()), thread1, SLOT(quit()));
    //QObject::connect(simThread, SIGNAL(finished()), thread1, SLOT(deleteLater()));
    //QObject::connect(thread1, SIGNAL(finished()), thread1, SLOT(deleteLater()));

    QObject::connect(thread2, SIGNAL(started()), filThread, SLOT(runHard()));
    QObject::connect(filThread, SIGNAL(finished()), thread2, SLOT(quit()));
    //QObject::connect(filThread, SIGNAL(finished()), thread2, SLOT(deleteLater()));
    //QObject::connect(thread2, SIGNAL(finished()), thread2, SLOT(deleteLater()));

    QObject::connect(filThread, SIGNAL(readyToFillTheTable()), this, SLOT(fillTheTable()), Qt::QueuedConnection);
    QObject::connect(simThread, SIGNAL(finished()), this, SLOT(simulationFinished()));

    wyliczCzasy();
    wypelnijTabele();
}

void SimulationWindow::wypelnijTabele() {
    std::vector<QString> data = control -> PrepareDataToBeShown();
    std::vector<QString> units = control -> PrepareUnitsToBeShown();
    QTableWidgetItem* item(NULL);
    int iterator(-1);
    for (auto x : data) {
        iterator++;
        table -> insertRow(table -> rowCount());
        //opis
        item = new QTableWidgetItem();
        item -> setText(x);
        item->setFlags(item->flags() &  ~Qt::ItemIsEditable);
        table -> setItem(table->rowCount()-1, 0, item);
        //wartosc
        item = new QTableWidgetItem();
        item -> setText("");
        item->setFlags(item->flags() &  ~Qt::ItemIsEditable);
        table -> setItem(table->rowCount()-1, 1, item);
        //jednostka
        item = new QTableWidgetItem();
        item -> setText(units[iterator]);
        item->setFlags(item->flags() &  ~Qt::ItemIsEditable);
        table -> setItem(table->rowCount()-1, 2, item);
    }
    for (int i=0; i<table -> rowCount(); ++i) { //wypelniamy vecShared pustymi miejscami - do wypelnienia
        vecShared -> push_back("");
    }
}

void SimulationWindow::wyliczCzasy() {
    double dt(0.0), tmax(0.0), coile(0.0);
    control -> GetTimeVariables(dt, tmax, coile);
    double ileIteracji = tmax / dt;
    double ileZapisywan = ileIteracji / coile;
    int ileRazyAktualizowacEkran = (int)(ileZapisywan / 100.0);
    filThread -> setModuloDoZapisywania(ileRazyAktualizowacEkran);
}

void SimulationWindow::onButtonStartClicked() {
    buttonStart -> setEnabled(false);
    buttonStop -> setEnabled(true);
    control -> CreateSimulation(stoppingVariablePtr);
    control -> SetMutexAndVecShared(mutex, vecShared, vecReady, odsetek, letMeStop);  //sets system with mutex and vecShared to share some data with filling thread

    thread2 -> start();
    thread1 -> start();

    //QMessageBox::information(this, tr("Informacja"), tr("Czekaj na komunikat o zakończeniu symulacji. W razie konieczności naciśniej STOP"), QMessageBox::StandardButton::Ok);

    //progressBar -> setValue(1000);
    //QMessageBox::information(this, tr("Informacja"), tr("Symulacja została zakończona"), QMessageBox::StandardButton::Ok);
}

void SimulationWindow::onButtonStopClicked() {  //zatrzymujemy oba wątki
    *stoppingVariablePtr = true;
    *letMeStop = true;
}

void SimulationWindow::simulationFinished() {
    progressBar -> setValue(1000);
    QMessageBox::information(this, tr("Informacja"), tr("Symulacja zakończyła się."), QMessageBox::StandardButton::Ok);
}

SimulationWindow::~SimulationWindow() {
    if (simThread != NULL) {
        delete simThread;
        simThread = NULL;
    }
    if (stoppingVariablePtr != NULL) {
        delete stoppingVariablePtr;
        stoppingVariablePtr = NULL;
    }
    if (letMeStop != NULL) {
        delete letMeStop;
        letMeStop = NULL;
    }
    if (vecShared != NULL) {
        delete vecShared;
        vecShared= NULL;
    }
    if (mutex != NULL) {
        delete mutex;
        mutex = NULL;
    }
    if (odsetek != NULL) {
        delete odsetek;
        odsetek = NULL;
    }
    if (filThread != NULL) {
        delete filThread;
        filThread = NULL;
    }
    if (thread1 != NULL) {
        if (thread1 -> isRunning()) {
            thread1 -> terminate();
            thread1 -> wait();
        }
        delete thread1;
        thread1 = NULL;
    }
    if (thread2 != NULL) {
        if (thread2 -> isRunning()) {
            thread2 -> terminate();
            thread2 -> wait();
        }
        delete thread2;
        thread2 = NULL;
    }
}

void SimulationWindow::fillTheTable() {
    int iterator(0);
    QTableWidgetItem* item;
    for (auto x : *vecShared) {
        item = table -> item(iterator, 1);
        item -> setText(x);
        iterator++;
    }
    progressBar -> setValue((int)(1000.0 * *odsetek));
}

//SIMULATION THREAD

SimulationThread::SimulationThread(Control* ctrl) {
    control = ctrl;
}

void SimulationThread::runHard() {
    control -> RunSimulation();
    emit finished();
}

//FILLING THREAD

FillingThread::FillingThread(QMutex* mt, std::vector<QString>* vS, bool* lms, bool* vr,double* ods) {
    vecShared = vS;
    letMeStop = lms;
    mutex = mt;
    vecReady = vr;
    odsetek = ods;
}

void FillingThread::runHard() {
    int iteracje(0);
    while (!(*letMeStop)) { //true wyłacza pętlę
        mutex -> lock();
        if (*letMeStop) {
            mutex -> unlock();
            return;
        }
        if (*vecReady) {
            if (iteracje % moduloDoZapisywania == 0) {
                emit readyToFillTheTable();
            }
            iteracje++;
            *vecReady = false;
        }
        mutex -> unlock();
    }
    emit finished();
}
