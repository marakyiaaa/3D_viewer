#include <QApplication>
#include "mainwindow.h"
#include <QDebug>
#include "iostream"
#include <QFile>
#include <QDir>


void OpenFile(char *filename, DATA *data_object, int *check);

void OpenFile(char *filename, DATA *data_object, int *check){
    *check = ParsingFile(filename, data_object);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setMinimumSize(1700, 1300);
    w.setMaximumSize(1700, 1300);

        w.show();
    return a.exec();
}

