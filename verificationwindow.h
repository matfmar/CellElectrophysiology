
#ifndef VERIFICATIONWINDOW_H
#define VERIFICATIONWINDOW_H


#include <QMainWindow>
#include <QGroupBox>
#include <QListWidget>
#include <QVBoxLayout>
#include <QPushButton>

#include "control.h"


class VerificationWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit VerificationWindow(QWidget* invoker, Control* ctrl, QWidget *parent = nullptr);
    ~VerificationWindow();
public slots:
    void onButtonZamknijClicked();
    void onButtonCosNieTakClicked();
signals:
    void everythingOK();
    void somethingFuckedUp();
private:
    QWidget* mainWidget;
    QGroupBox* groupAllStructures,* groupAllStructuresToBeSaved;
    QListWidget* listAllStructures,* listAllStructuresToBeSaved;
    QVBoxLayout* layoutAll,* layoutToSave,* layout;
    QPushButton* buttonZamknij,* buttonCosNieTak;


    Control* control;
    std::vector<std::string>* allStructures;
    std::vector<std::string>* allStructuresToBeSaved;

    void wypelnijListy();
};

#endif // VERIFICATIONWINDOW_H
