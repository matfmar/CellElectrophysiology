QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Simulation.cpp \
    System.cpp \
    UsefulFunctions.cpp \
    addionwindow.cpp \
    addtransporterwindow.cpp \
    control.cpp \
    ionswindow.cpp \
    main.cpp \
    mainwindow.cpp \
    parametrywindow.cpp \
    simulationwindow.cpp \
    transporterswindow.cpp \
    verificationwindow.cpp \
    zapisywaniewindow.cpp

HEADERS += \
    Simulation.h \
    System.h \
    UsefulFunctions.h \
    addionwindow.h \
    addtransporterwindow.h \
    control.h \
    ionswindow.h \
    mainwindow.h \
    parametrywindow.h \
    simulationwindow.h \
    transporterswindow.h \
    verificationwindow.h \
    zapisywaniewindow.h

#FORMS += \
#    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32: LIBS += -L$$PWD/libs/ -lCompsLib

INCLUDEPATH += $$PWD/libs
DEPENDPATH += $$PWD/libs

RC_ICONS = favicon.ico
