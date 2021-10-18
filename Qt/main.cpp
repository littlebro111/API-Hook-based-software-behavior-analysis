#include "mainwindow.h"
#include "exeloadview.h"
#include <QApplication>
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    exeLoadView w;
    w.show();
    return a.exec();
}
