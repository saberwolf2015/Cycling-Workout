// Copyright (c) 2016 Tchernopyatov Alexey. Contacts: alexey@tchernopyatov.com
// Under MIT license, view LICENSE.txt

#include "mainwindow.h"
#include <QApplication>
#include <QFileInfo>
#include <QDir>

int main(int argc, char *argv[])
{
    QString appPath = QFileInfo(argv[0]).dir().path();

    QStringList paths = QCoreApplication::libraryPaths();
    qDebug() << paths;
    paths.append(".");
    paths.append(appPath);
    paths.append(appPath + "/platforms");
    //paths.append(appPath + "/platform");
    QCoreApplication::setLibraryPaths(paths);
    qDebug() << paths;


    QApplication a(argc, argv);

    a.setOrganizationName("Alexey Tchernopyatov");
    a.setOrganizationDomain("tchernopyatov.com");
    a.setApplicationName("Cycling Workout");

    MainWindow w;
    w.show();

    return a.exec();
}
