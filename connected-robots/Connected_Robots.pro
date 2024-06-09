QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += qmqtt/src/mqtt

SOURCES += \
    config.cpp \
    instruction.cpp \
    main.cpp \
    mainwindow.cpp \
    robot.cpp \
    subscriber.cpp \
    tcp_client.cpp

HEADERS += \
    config.h \
    instruction.h \
    mainwindow.h \
    robot.h \
    subscriber.h \
    tcp_client.h

FORMS += \
    mainwindow.ui

win32: LIBS += -L/lib/ -lQt6Qmqtt

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    main.qrc
