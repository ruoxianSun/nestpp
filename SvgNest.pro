#-------------------------------------------------
#
# Project created by QtCreator 2020-05-19T13:43:46
#
#-------------------------------------------------

QT       += core-private gui-private quickwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets-private


TARGET = SvgNest
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11
include(geometry/geometry.pri)
include(RectangleBinPack/binpack.pri)
include(thirds/thirds.pri)
include(core3d/core3d.pri)
SOURCES += \
        CGlWidget.cpp \
        CListWidget.cpp \
        CMyMessageBox.cpp \
        CMyStyle.cpp \
        CPushButton.cpp \
        CTreeView.cpp \
        form.cpp \
        formitem.cpp \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        CGlWidget.h \
        CListWidget.h \
        CMyMessageBox.h \
        CMyStyle.h \
        CPushButton.h \
        CTreeView.h \
        ctest.hpp \
        form.h \
        formitem.h \
        mainwindow.h

FORMS += \
        CMyMessageBox.ui \
        form.ui \
        formitem.ui \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource/image.qrc \
    resource/shader.qrc \
    resource/style.qrc

