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

        PointMap p;

        QList<QPointF> l;
        l << QPointF(1, 2);
        l << QPointF(2, 4);

        p = PointMap::fromPointList(l);
        qDebug() << p;

        if (!p.loadFile("test.csv")) {
            qDebug() << "load error !";
            return 1;
        }

        qDebug() << p;
        return 0;
        // interpole 10 fois entre chaque points
        qreal step = (p.xMaximum() - p.xMinimum()) / (p.size() * 10.0);

        for (qreal x = p.xMinimum(); x <= p.xMaximum(); x += step) {
            std::cout << x
                      << " " << p.interpolate(x, PointMap::Interpolation2)
                      << " " << p.interpolate(x, PointMap::Interpolation4)
                      << " " << p.interpolate(x, PointMap::Interpolation6)
                      << " " << p.spline(x)
                      << std::endl;
        }

        p.clear();
        p[0] = 1;
        p[5] = 2;
        p.insertMulti(5, 3);
        p[10] = 1;
        qDebug() << p;

        qDebug() << "itegral = " << p.integrate(0, 10);
    }
    return 0;
}
