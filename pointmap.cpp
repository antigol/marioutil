#include "pointmap.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>

#include <QDebug>

PointMap::PointMap()
    : QMap<qreal, qreal>()
{
}

PointMap::PointMap(const QMap<qreal, qreal> &other)
    : QMap<qreal, qreal>(other)
{
}

bool PointMap::loadFile(const QString &filePath)
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
        QStringList elements = line.split(QRegExp("\\s+"));

        if (elements.size() == 2) {
            bool ok;
            qreal x, y;

            x = elements.first().toDouble(&ok);
            if (ok == false) continue;

            y = elements.last().toDouble(&ok);
            if (ok == false) continue;

            if (contains(x))
                qDebug() << "There is already a value with key : " << x;

            insert(x, y);
        }
    }

    return true;
}

qreal PointMap::interpolate(qreal x, PointMap::InterpolationType type)
{
    if (contains(x))
        return value(x);

    // si la liste est vide on retourne 0
    if (isEmpty())
        return 0.0;

    if (type == Interpolation2 || size() <= 2)
        return interpolate2(x);

    if (type == Interpolation3 || size() <= 3)
        return interpolate3(x);

    // il ne reste plus que type == Interpolation4
    return interpolate4(x);
}

qreal PointMap::interpolate2(qreal x)
{
    // retourne l'élément plus grand que x
    PointMap::iterator i = upperBound(x);

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

qreal PointMap::interpolate3(qreal x)
{
    // retourne l'élément plus grand que x
    PointMap::iterator i = upperBound(x);

    // s'il n'y a pas d'éléments plus grand que x
    if (i == constEnd())
        return (--i).value();

    // si l'élément retourné est le premier élément de la liste
    if (i == constBegin())
        return i.value();

    // on fait en sorte que i pointe sur le plus grand des quatres éléments (x3,y3)
    i++;
    if (i == constEnd())
        i--;

    qreal x3 = i.key();
    qreal y3 = i.value();

    i--;
    qreal x2 = i.key();
    qreal y2 = i.value();

    i--;
    qreal x1 = i.key();
    qreal y1 = i.value();

    return y1 * (x-x2)*(x-x3) / ((x1-x2)*(x1-x3))
            + y2 * (x-x1)*(x-x3) / ((x2-x1)*(x2-x3))
            + y3 * (x-x1)*(x-x2) / ((x3-x1)*(x3-x2));
}

qreal PointMap::interpolate4(qreal x)
{
    // retourne l'élément plus grand que x
    PointMap::iterator i = upperBound(x);

    // s'il n'y a pas d'éléments plus grand que x
    if (i == constEnd())
        return (--i).value();

    // si l'élément retourné est le premier élément de la liste
    if (i == constBegin())
        return i.value();


    // on fait en sorte que i pointe sur le plus grand des quatres éléments (x4,y4)
    i++;
    if (i == constEnd())
        i--;

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

    // merci Paul Bourke
    return y1 * (x-x2)*(x-x3)*(x-x4) / ((x1-x2)*(x1-x3)*(x1-x4))
            + y2 * (x-x1)*(x-x3)*(x-x4) / ((x2-x1)*(x2-x3)*(x2-x4))
            + y3 * (x-x1)*(x-x2)*(x-x4) / ((x3-x1)*(x3-x2)*(x3-x4))
            + y4 * (x-x1)*(x-x2)*(x-x3) / ((x4-x1)*(x4-x2)*(x4-x3));
}
