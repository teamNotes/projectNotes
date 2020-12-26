#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    if(!w.connectDB())
        exit(1);
    w.display();                                    //пользовательская функция вызова главного окна

    return a.exec();
}
