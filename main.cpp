#include <QtCore/QCoreApplication>
#include <QDebug>
#include "pointmap.h"

#include <iostream>
#include <gsl/gsl_complex.h>
#include <gsl/gsl_complex_math.h>

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

    pl.clear();
    pl[0] = 1;
    pl[5] = 2;
    pl.insertMulti(5, 3);
    pl[10] = 1;
    qDebug() << pl;

    qDebug() << "itegral = " << pl.integrate(0, 10);

    return 0;
}
