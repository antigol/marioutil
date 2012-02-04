#ifndef POINTMAP_H
#define POINTMAP_H

#include <QMap>

class PointMap : public QMap<qreal, qreal>
{
public:
    PointMap();
    PointMap(const QMap<qreal, qreal> &other);

    // charge un fichier à deux colonnes séparées par un espace
    bool loadFile(const QString &filePath);


    enum InterpolationType {

        // interpolation linéaire
        Interpolation2,

        // interpolation à l'aide de 3 éléments (ne sert qui si il y a que 3 éléments dans la map)
        Interpolation3,

        // interpolation cublique, utilise 4 éléments
        Interpolation4
    };

    // interpole la valeur y à partir de la valeur x.
    // utilise la méthode type à condition qu'il y aie assez d'éléments
    // si la valeur x n'est pas comprise dans les bornes, retourne la valeur à l'extrémité du map
    qreal interpolate(qreal x, InterpolationType type = Interpolation2);



private:
    qreal interpolate2(qreal x);
    qreal interpolate3(qreal x);
    qreal interpolate4(qreal x);
};

#endif // POINTMAP_H
