#ifndef POINTMAP_H
#define POINTMAP_H

#include <QMap>
#include <gsl/gsl_spline.h>

class PointMap : public QMap<qreal, qreal>
{
public:
    PointMap();
    PointMap(const QMap<qreal, qreal> &other);
    ~PointMap();

    // charge un fichier à deux colonnes séparées par un espace
    bool loadFile(const QString &filePath);


    // la plus petite valeur x
    qreal xMinimum() const;

    // la plus grande valeur x
    qreal xMaximum() const;

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
    qreal interpolate(qreal x, InterpolationType type = Interpolation2);


    // calculateSpline calcule la courbe spline avec les éléments actuels
    void calculateSpline();

    // spline retourne chaque valeur de la spline calculée à l'aide de calculateSpline
    // si aucune spline n'a été calulée il lance calculateSpline automatiquement
    qreal spline(qreal x);

private:
    qreal interpolate2(qreal x);
    qreal interpolate4(qreal x);
    qreal interpolate6(qreal x);

    gsl_interp_accel *_acc;
    gsl_spline *_spline;
};

#endif // POINTMAP_H
