#include "PhysicalTherapyRobot.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PhysicalTherapyRobot w;
    w.show();
    return a.exec();
}
