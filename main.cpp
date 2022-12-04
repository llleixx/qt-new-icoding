#include "login.h"
#include "mainwindow.h"

#include <QApplication>


QString username = "0";
MainWindow *mainwindow;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Login w;
    w.show();
    return a.exec();
}
