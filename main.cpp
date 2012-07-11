#include <QtGui/QApplication>
#include <QTextCodec>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec *RusCodec = QTextCodec::codecForName("CP1251");
    QTextCodec::setCodecForTr(RusCodec);

    MainWindow w;
    w.show();
    
    return a.exec();
}
