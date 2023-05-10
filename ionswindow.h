
#ifndef IONSWINDOW_H
#define IONSWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "control.h"

class IonsWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit IonsWindow(Control* ctrl, QWidget *parent = nullptr);
    ~IonsWindow();
public slots:
    void onButtonDodajClicked();
    void onButtonUsunClicked();
    void moveIonsFromPossibleToUsed(QString name);
private:
    QWidget* mainWidget;
    QLabel* label1,* label2;
    QListWidget* listUsedIons,* listPossibleIons;
    QPushButton* buttonDodaj,* buttonUsun,* buttonZamknij;
    QVBoxLayout* listsLayout,* buttonsLayout;
    QHBoxLayout* layout;

    Control* control;

    struct IonFromList {
        QString name;
        int id;
    };
    std::vector<IonFromList>* tabIonsFromList;

    void wypelnijListeMozliwychJonow();
    void wypelnijListeUzytychJonow();
};

#endif // IONSWINDOW_H
