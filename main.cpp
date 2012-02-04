#include <QtCore/QCoreApplication>
#include <QDebug>
#include "pointmap.h"

#include <iostream>

int main()
{
    PointMap pl;

    if (!pl.loadFile("test.txt")) {
        qDebug() << "load error !";
        return 1;
    }

    qDebug() << pl;

    // interpole 10 fois entre chaque points
    qreal step = (pl.xMaximum() - pl.xMinimum()) / (pl.size() * 10.0);

    for (qreal x = pl.xMinimum(); x <= pl.xMaximum(); x += step) {
        std::cout << x
                  << " " << pl.interpolate(x, PointMap::Interpolation2)
                  << " " << pl.interpolate(x, PointMap::Interpolation4)
                  << " " << pl.interpolate(x, PointMap::Interpolation6)
                  << " " << pl.spline(x)
                  << std::endl;
    }

    return 0;
}
