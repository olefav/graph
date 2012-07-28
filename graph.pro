#-------------------------------------------------
#
# Project created by QtCreator 2012-04-28T15:50:57
#
#-------------------------------------------------

QT       += core gui

TARGET = graph
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    graphscene.cpp \
    node.cpp \
    link.cpp \
    sizedialog.cpp \
    adjmatrixmodel.cpp \
    adjmatrdialog.cpp \
    formmatrixgrid.cpp \
    twonodesdialog.cpp

HEADERS  += mainwindow.h \
    graphscene.h \
    node.h \
    link.h \
    sizedialog.h \
    adjmatrixmodel.h \
    adjmatrdialog.h \
    formmatrixgrid.h \
    twonodesdialog.h

FORMS    += mainwindow.ui \
    sizedialog.ui \
    adjmatrdialog.ui \
    formmatrixgrid.ui \
    twonodesdialog.ui

RESOURCES += \
    resources.qrc

TRANSLATIONS += graph_ru.ts qt_ru.ts
