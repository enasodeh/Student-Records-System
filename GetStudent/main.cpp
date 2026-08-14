#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow window;
    window.setWindowTitle("GetStudent");
    window.resize(400, 220);
    window.show();

    return app.exec();
}
