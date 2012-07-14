#include <QtGui/QApplication>
#include <QTextCodec>
#include <QTranslator>
#include <QLocale>+
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator Translator(0);
    Translator.load(QString(QApplication::applicationDirPath() + QString( "/graph_" ) + QLocale::system().name()) , ".");
    a.installTranslator(&Translator);

    MainWindow w;
    w.show();
    
    return a.exec();
}
