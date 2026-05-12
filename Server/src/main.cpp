#include "MainWindow.hpp"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);

    alt::MainWindow mainWindow;

    mainWindow.show();

    return application.exec();
}