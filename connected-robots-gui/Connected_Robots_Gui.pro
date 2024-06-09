QT       += core gui network quick quickwidgets

CONFIG += c++14

INCLUDEPATH += qmqtt/src/mqtt

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH += 3d

SOURCES += \
    instruction.cpp \
    main.cpp \
    mainwindow.cpp \
    subscriber.cpp

RESOURCES += \
    main.qrc

HEADERS += \
    instruction.h \
    mainwindow.h \
    subscriber.h

FORMS += \
    mainwindow.ui

win32: LIBS += -L/lib/ -lQt6Qmqtt

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
