#include "pointmap.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>

#include <QDebug>

PointMap::PointMap()
    : QMap<qreal, qreal>()
      #ifndef NOSPLINE
      , _acc(0), _spline(0)
      #endif
{
}

PointMap::PointMap(const QMap<qreal, qreal> &other)
    : QMap<qreal, qreal>(other)
    #ifndef NOSPLINE
    , _acc(0), _spline(0)
    #endif
{
}

PointMap::~PointMap()
{
#ifndef NOSPLINE
    if (_spline != 0)
        gsl_spline_free(_spline);

    if (_acc != 0)
        gsl_interp_accel_free(_acc);
#endif
}

QList<QPointF> PointMap::toPointList() const
{
    QList<QPointF> ret;

    PointMap::const_iterator i = constBegin();
    for (; i != constEnd(); ++i) {
        ret << QPointF(i.key(), i.value());
    }

    return ret;
}

PointMap PointMap::fromPointList(const QList<QPointF> &other)
{
    PointMap ret;
    for (int i = other.size() - 1; i >= 0; --i) {
        ret[other[i].x()] = other[i].y();
    }
    return ret;
}

bool PointMap::loadFile(const QString &filePath, int xIndex, int yIndex, const QRegExp &sep)
{
    // ouvre le fichier
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "file.open error";
        return false;
    }

    // lit le fichier ligne par ligne
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();

        // découpe la ligne par les éspaces
        QStringList elements = line.split(sep);

        if (qMax(xIndex, yIndex) < elements.size()) {
            bool ok;
            qreal x, y;

            x = elements.at(xIndex).toDouble(&ok);
            if (ok == false) {
                qDebug() << "line skiped : " << line;
                continue;
            }

            y = elements.at(yIndex).toDouble(&ok);
            if (ok == false) {
                qDebug() << "line skiped : " << line;
                continue;
            }

            if (contains(x))
                qDebug() << "There is already a value with key : " << x;

            insert(x, y);
        } else {
            qDebug() << "line skiped : " << line;
        }
    }

    return true;
}

qreal PointMap::xMinimum() const
{
    if (isEmpty())
        return 0.0;

    PointMap::const_iterator i = constBegin();
    return i.key();
}

qreal PointMap::xMaximum() const
{
    if (isEmpty())
        return 0.0;

    PointMap::const_iterator i = constEnd();
    i--;
    return i.key();
}

qreal PointMap::yMinimum() const
{
    qreal min = 0.0;

    PointMap::const_iterator i = constBegin();
    if (i != constEnd()) {
        min = i.value();
    }

    for (; i != constEnd(); ++i) {
        min = qMin(min, i.value());
    }

    return min;
}

qreal PointMap::yMaximum() const
{
    qreal max = 0.0;

    PointMap::const_iterator i = constBegin();
    if (i != constEnd()) {
        max = i.value();
    }

    for (; i != constEnd(); ++i) {
        max = qMax(max, i.value());
    }

    return max;
}

qreal PointMap::interpolate(qreal x, PointMap::InterpolationType type) const
{
    if (contains(x))
        return value(x);

    // si la liste est vide on retourne 0
    if (isEmpty())
        return 0.0;

    if (type == Interpolation2 || size() < 4)
        return interpolate2(x);

    if (type == Interpolation4 || size() < 6)
        return interpolate4(x);

    // il ne reste plus que type == Interpolation6
    return interpolate6(x);
}

#ifndef NOSPLINE
void PointMap::calculateSpline()
{
    // supprime l'ancienne spline
    if (_spline != 0)
        gsl_spline_free(_spline);

    if (_acc != 0)
        gsl_interp_accel_free(_acc);

    _acc = gsl_interp_accel_alloc();
    _spline = gsl_spline_alloc(gsl_interp_cspline, size());

    QVector<qreal> x = keys().toVector();
    QVector<qreal> y = values().toVector();

    gsl_spline_init(_spline, x.constData(), y.constData(), size());
}

qreal PointMap::spline(qreal x)
{
    if (contains(x))
        return value(x);

    // si la liste est vide on retourne 0
    if (isEmpty())
        return 0.0;

    // retourne l'élément plus grand que x
    PointMap::const_iterator i = upperBound(x);

    // s'il n'y a pas d'éléments plus grand que x
    if (i == constEnd())
        return (--i).value();

    // si l'élément retourné est le premier élément de la liste
    if (i == constBegin())
        return i.value();


    if (_spline == 0 || _acc == 0)
        calculateSpline();

    return gsl_spline_eval(_spline, x, _acc);
}
#endif

qreal PointMap::integrate(qreal a, qreal b) const
{
    qreal result = 0.0;

    qreal x1 = a;
    qreal y1 = interpolate(a);

    qreal x2;
    qreal y2;

    PointMap::const_iterator i = upperBound(a);
    PointMap::const_iterator end = lowerBound(b);

    while (i != end) {
        x2 = i.key();
        y2 = i.value();

        result += (x2-x1) * (y1+y2) / 2.0;

        x1 = x2;
        y1 = y2;

        i++;
    }

    x2 = b;
    y2 = interpolate(b);

    return result + (x2-x1) * (y1+y2) / 2.0;
}

PointMap &PointMap::operator *=(const PointMap &other)
{
    return (*this = *this * other);
}

PointMap PointMap::operator *(const PointMap &other) const
{
    PointMap ret;

    for (PointMap::const_iterator i = constBegin(); i != constEnd(); ++i) {
        ret[i.key()] = i.value() * other.interpolate(i.key());
    }

    for (PointMap::const_iterator i = other.constBegin(); i != other.constEnd(); ++i) {
        ret[i.key()] = interpolate(i.key()) * i.value();
    }

    return ret;
}

PointMap &PointMap::operator /=(const PointMap &other)
{
    return (*this = *this / other);
}

PointMap PointMap::operator /(const PointMap &other) const
{
    PointMap ret;

    for (PointMap::const_iterator i = constBegin(); i != constEnd(); ++i) {
        ret[i.key()] = i.value() / other.interpolate(i.key());
    }

    for (PointMap::const_iterator i = other.constBegin(); i != other.constEnd(); ++i) {
        ret[i.key()] = interpolate(i.key()) / i.value();
    }

    return ret;
}

PointMap &PointMap::operator *=(qreal k)
{
    for (PointMap::iterator i = begin(); i != end(); ++i) {
        i.value() *= k;
    }

    return *this;
}

PointMap PointMap::operator *(qreal k) const
{
    PointMap ret = *this;

    return ret *= k;
}

PointMap &PointMap::operator /=(qreal k)
{
    for (PointMap::iterator i = begin(); i != end(); ++i) {
        i.value() /= k;
    }

    return *this;
}

PointMap PointMap::operator /(qreal k) const
{
    PointMap ret = *this;

    return ret /= k;
}

qreal PointMap::interpolate2(qreal x) const
{
    // retourne l'élément plus grand que x
    PointMap::const_iterator i = upperBound(x);

    // s'il n'y a pas d'éléments plus grand que x
    if (i == constEnd())
        return (--i).value();

    // si l'élément retourné est le premier élément de la liste
    if (i == constBegin())
        return i.value();

    qreal x1 = i.key();
    qreal y1 = i.value();

    i--;
    qreal x0 = i.key();
    qreal y0 = i.value();

    // la pente
    qreal m = (y1 - y0) / (x1 - x0);

    return m * (x - x0) + y0;
}

qreal PointMap::interpolate4(qreal x) const
{
    // retourne l'élément plus grand que x
    PointMap::const_iterator i = upperBound(x);

    // s'il n'y a pas d'éléments plus grand que x
    if (i == constEnd())
        return (--i).value();

    // si l'élément retourné est le premier élément de la liste
    if (i == constBegin())
        return i.value();


    // comme i pointe sur l'élément plus grand que x, i pointe sur le point no3

    // si i pointe sur le dernier élément, on le désincrémente
    if (i + 1 == constEnd())
        i--;

    // si i pointe sur le deuxième élément on l'incrémente
    if (i - 1 == constBegin())
        i++;

    qreal x3 = i.key();
    qreal y3 = i.value();

    i--;
    qreal x2 = i.key();
    qreal y2 = i.value();

    i--;
    qreal x1 = i.key();
    qreal y1 = i.value();

    i += 3;
    qreal x4 = i.key();
    qreal y4 = i.value();

    // merci Paul Bourke
    return
            y1 * (x-x2)*(x-x3)*(x-x4) / ((x1-x2)*(x1-x3)*(x1-x4)) +
            y2 * (x-x1)*(x-x3)*(x-x4) / ((x2-x1)*(x2-x3)*(x2-x4)) +
            y3 * (x-x1)*(x-x2)*(x-x4) / ((x3-x1)*(x3-x2)*(x3-x4)) +
            y4 * (x-x1)*(x-x2)*(x-x3) / ((x4-x1)*(x4-x2)*(x4-x3));
}

qreal PointMap::interpolate6(qreal x) const
{
    // retourne l'élément plus grand que x
    PointMap::const_iterator i = upperBound(x);

    // s'il n'y a pas d'éléments plus grand que x
    if (i == constEnd())
        return (--i).value();

    // si l'élément retourné est le premier élément de la liste
    if (i == constBegin())
        return i.value();


    // comme i pointe sur l'élément plus grand que x, i pointe sur le point no4

    // si i pointe sur le dernier ou avant-dernier élément, on le désincrémente
    if (i + 1 == constEnd())
        i--;
    if (i + 2 == constEnd())
        i--;

    // si i pointe sur le deuxième ou troisième élément on l'incrémente
    if (i - 1 == constBegin())
        i++;
    if (i - 2 == constBegin())
        i++;

    qreal x4 = i.key();
    qreal y4 = i.value();

    i--;
    qreal x3 = i.key();
    qreal y3 = i.value();

    i--;
    qreal x2 = i.key();
    qreal y2 = i.value();

    i--;
    qreal x1 = i.key();
    qreal y1 = i.value();

    i += 4;
    qreal x5 = i.key();
    qreal y5 = i.value();

    i++;
    qreal x6 = i.key();
    qreal y6 = i.value();

    // merci Paul Bourke
    return
            y1 * (x-x2)*(x-x3)*(x-x4)*(x-x5)*(x-x6) / ((x1-x2)*(x1-x3)*(x1-x4)*(x1-x5)*(x1-x6)) +
            y2 * (x-x1)*(x-x3)*(x-x4)*(x-x5)*(x-x6) / ((x2-x1)*(x2-x3)*(x2-x4)*(x2-x5)*(x2-x6)) +
            y3 * (x-x1)*(x-x2)*(x-x4)*(x-x5)*(x-x6) / ((x3-x1)*(x3-x2)*(x3-x4)*(x3-x5)*(x3-x6)) +
            y4 * (x-x1)*(x-x2)*(x-x3)*(x-x5)*(x-x6) / ((x4-x1)*(x4-x2)*(x4-x3)*(x4-x5)*(x4-x6)) +
            y5 * (x-x1)*(x-x2)*(x-x3)*(x-x4)*(x-x6) / ((x5-x1)*(x5-x2)*(x5-x3)*(x5-x4)*(x5-x6)) +
            y6 * (x-x1)*(x-x2)*(x-x3)*(x-x4)*(x-x5) / ((x6-x1)*(x6-x2)*(x6-x3)*(x6-x4)*(x6-x5));
}
