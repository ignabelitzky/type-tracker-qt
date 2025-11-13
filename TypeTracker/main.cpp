#include "typetracker.h"
#include <QApplication>
#include <QString>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TypeTracker w;
    w.show();
    return a.exec();

}

