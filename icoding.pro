QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    login.cpp \
    mainwindow.cpp \
    myframe.cpp \
    mywidget.cpp \
    users.cpp

HEADERS += \
    login.h \
    mainwindow.h \
    myframe.h \
    mywidget.h \
    users.h

FORMS += \
    login.ui \
    mainwindow.ui \
    myframe.ui \
    mywidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    images.qrc

DISTFILES += \
    images/1.png \
    images/2.png \
    images/3.png \
    images/4.png \
    images/about.png \
    images/bookmark.svg \
    images/close.png \
    images/close.svg \
    images/drop-down.png \
    images/drop-down.svg \
    images/home.svg \
    images/list.svg \
    images/max.png \
    images/min.png \
    images/min.svg \
    images/search.png \
    images/setting.svg
