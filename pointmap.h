#ifndef POINTMAP_H
#define POINTMAP_H

#include <QMap>
#include <QRegExp>
#include <gsl/gsl_spline.h>

#include <QList>
#include <QPointF>

class PointMap : public QMap<qreal, qreal>
{
public:
    PointMap();
    PointMap(const QMap<qreal, qreal> &other);
    ~PointMap();

    QList<QPointF> toPointList() const;

    // charge un fichier à colonnes séparées par un espace
    // xIndex et yIndex sont les numéros des colonnes
    bool loadFile(const QString &filePath, int xIndex = 0, int yIndex = 1, const QRegExp &sep = QRegExp("\\s+"));


    // la plus petite valeur x, retourne 0.0 si vide
    qreal xMinimum() const;

    // la plus grande valeur x
    qreal xMaximum() const;

    // la plus petite valeur en y
    qreal yMinimum() const;

    // la plus grande valeur en y
    qreal yMaximum() const;

    enum InterpolationType {

        // interpolation linéaire
        Interpolation2,

        // interpolation cublique, utilise 4 éléments
        Interpolation4,

        // interpolation qui utilise 6 éléments
        Interpolation6
    };

    // interpole la valeur y à partir de la valeur x.
    // utilise la méthode type à condition qu'il y aie assez d'éléments
    // si la valeur x n'est pas comprise dans les bornes, retourne la valeur à l'extrémité du map
    qreal interpolate(qreal x, InterpolationType type = Interpolation2) const;


    // calculateSpline calcule la courbe spline avec les éléments actuels
    void calculateSpline();

    // spline retourne chaque valeur de la spline calculée à l'aide de calculateSpline
    // si aucune spline n'a été calulée il lance calculateSpline automatiquement
    qreal spline(qreal x);


    // intègre sur l'intervale [a,b]
    // interpole linéairement pour integré (relie les point par des droites pour calculer l'aire)
    qreal integrate(qreal a, qreal b) const;

private:
    qreal interpolate2(qreal x) const;
    qreal interpolate4(qreal x) const;
    qreal interpolate6(qreal x) const;

    gsl_interp_accel *_acc;
    gsl_spline *_spline;
};

#endif // POINTMAP_H
