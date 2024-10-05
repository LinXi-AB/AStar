#include "gridwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GridWindow w;
    w.show();
    return a.exec();
}
