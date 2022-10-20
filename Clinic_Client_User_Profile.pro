QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    c_clientprocessconnection.cpp \
    c_parser.cpp \
    cv_processdata.cpp \
    main.cpp \
    w_logswindow.cpp \
    w_mainwindow.cpp

HEADERS += \
    c_clientprocessconnection.h \
    c_myStructures.h \
    c_parser.h \
    cv_processdata.h \
    w_logswindow.h \
    w_mainwindow.h

FORMS += \
    w_logswindow.ui \
    w_mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icons.qrc
