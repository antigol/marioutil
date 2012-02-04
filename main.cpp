#include <QtCore/QCoreApplication>
#include <QDebug>
#include "pointmap.h"

#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    PointMap pl;

    pl.loadFile("test.txt");

    //    pl.insert(0,1);
    //    pl.insert(2,3);
    //    pl.insert(3,6);
    //    pl.insert(5,7);

    qDebug() << pl;

    qreal x = 6;
    qreal y = pl.interpolate(x, PointMap::Interpolation4);
    qDebug() << x << "," << y;

    for (qreal x = -5; x <= 6; x += 0.1) {
        std::cout << x << " " << pl.interpolate(x, PointMap::Interpolation4) << std::endl;
    }

    return 0;
}
