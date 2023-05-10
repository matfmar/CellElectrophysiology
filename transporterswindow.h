
#ifndef TRANSPORTERSWINDOW_H
#define TRANSPORTERSWINDOW_H


#include <QMainWindow>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>

#include "control.h"

class TransportersWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit TransportersWindow(Control* ctrl, QWidget *parent = nullptr);

    ~TransportersWindow();
public slots:
    void onButtonDodajClicked();
    void onButtonUsunClicked();
    void moveTransportersFromPossibleToUsed(QString name);
private:
    QWidget* mainWidget;
    QGroupBox* groupTransportersUsed,* groupTransportersPossible;
    QLabel* label1,* label2;
    QPushButton* buttonDodaj,* buttonUsun,* buttonZamknij;
    QVBoxLayout* layoutUsed,* layoutPossible,* layoutAll,* layoutButtons;
    QHBoxLayout* layout;
    QListWidget* listPossible,* listUsed;

    Control* control;

    struct TransporterFromList {
        QString name;
        int id;
    };
    std::vector<TransporterFromList>* tabTransportersFromList;

    void wypelnijListeMozliwychTransporterow();
    void wypelnijListeUzytychTransporterow();
};

#endif // TRANSPORTERSWINDOW_H
