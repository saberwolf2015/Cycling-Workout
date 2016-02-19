// Copyright (c) 2016 Tchernopyatov Alexey. Contacts: alexey@tchernopyatov.com
// Under MIT license, view LICENSE.txt

#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
