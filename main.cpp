#include <QtCore/QCoreApplication>
#include <QDebug>
#include "pointmap.h"
#include "gslfunction.h"

#include <iostream>
#include <gsl/gsl_integration.h>

#include <QSettings>

class MyClass : public GslFunction
{
public:
    double function(double d, void *v) {
        double *r = static_cast<double *>(v);
        double dA = sin(d) * 2.0 * M_PI * gsl_pow_2(r[0]);
        return dA;
    }
};

int main()
{
    /*{
        gsl_integration_workspace *w = gsl_integration_workspace_alloc (1000);

        double result, error;
        double alpha = M_PI_4;
        double r = 10.0;
        double expected = -2.0 * M_PI * r*r * (cos(alpha) - 1.0);

        MyClass a;
        gsl_function f = a.functor(&r);

        gsl_integration_qag(&f, 0, alpha, 0, 1e-7, 1000, GSL_INTEG_GAUSS31, w, &result, &error);

        printf ("result          = % .18f\n", result);
        printf ("exact result    = % .18f\n", expected);
        printf ("estimated error = % .18f\n", error);
        printf ("actual error    = % .18f\n", result - expected);
        printf ("intervals =  %ld\n", w->size);

        gsl_integration_workspace_free (w);


        return 0;
    }
    {
        QSettings set("config.txt", QSettings::IniFormat);

        qDebug() << set.value("test", "caca").toString();

        return 0;
    }*/
    {
        PointMap pl;

        if (!pl.loadFile("test.csv")) {
            qDebug() << "load error !";
            return 1;
        }

        qDebug() << pl;
        return 0;
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
    }
    return 0;
}
