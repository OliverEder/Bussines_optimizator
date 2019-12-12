#include "mainwindow.h"
#include <QApplication>
#include "treemodel.h"
#include <QFile>
#include <QTreeView>
#include <QSplashScreen>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    QPixmap pixmap("splash.png");
    QSplashScreen *splash = new QSplashScreen(pixmap);
    splash->show();
    splash->showMessage("Loaded modules");
    MainWindow w;
    w.show();
    splash->hide();


    return a.exec();
}
