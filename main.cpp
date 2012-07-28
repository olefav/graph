#include <QtGui/QApplication>
#include <QTextCodec>
#include <QTranslator>
#include <QLocale>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator Translator;
    Translator.load(QString(QApplication::applicationDirPath() + QString( "/graph_" ) + QLocale::system().name()) , ".");
    a.installTranslator(&Translator);

    QTranslator QtTranslator;
    QtTranslator.load(QString(QApplication::applicationDirPath() + QString( "/qt_" ) + QLocale::system().name()) , ".");
    a.installTranslator(&QtTranslator);

    MainWindow w;
    w.show();
    
    return a.exec();
}
