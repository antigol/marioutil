#ifndef POINTMAP_H
#define POINTMAP_H

#include <QMap>
#include <QRegExp>

#ifndef NOSPLINE
#   include <gsl/gsl_spline.h>
#endif

#include <QList>
#include <QPointF>

class PointMap : public QMap<qreal, qreal>
{
public:
    PointMap();
    PointMap(const QMap<qreal, qreal> &other);
    ~PointMap();

    /*     TO POINT LIST
      QList<QPointF> pl = myMap.toPointList();
     */
    QList<QPointF> toPointList() const;

    /*     FROM POINT LIST
      PointMap pm = PointMap::fromPointList(myList);
     */
    static PointMap fromPointList(const QList<QPointF> &other);


    /*     LOAD FILE
      Charge un fichier à colonnes séparées par un espace
      xIndex et yIndex sont les numéros des colonnes

      ml.loadFile("example.csv", 0, 1, QRegExp(";"));
     */
    bool loadFile(const QString &filePath, int xIndex = 0, int yIndex = 1, const QRegExp &sep = QRegExp("\\s+"));


    // La plus petite valeur x, retourne 0.0 si vide
    qreal xMinimum() const;

    // La plus grande valeur x, retourne 0.0 si vide
    qreal xMaximum() const;

    // La plus petite valeur en y, retourne 0.0 si vide
    qreal yMinimum() const;

    // La plus grande valeur en y, retourne 0.0 si vide
    qreal yMaximum() const;

    enum InterpolationType {
        Interpolation2, // interpolation linéaire
        Interpolation4, // interpolation cublique, utilise 4 éléments
        Interpolation6  // interpolation qui utilise 6 éléments
    };

    /*     INTERPOLATE
      Interpole la valeur y à partir de la valeur x.
      Utilise la méthode type à condition qu'il y aie assez d'éléments
      Si la valeur x n'est pas comprise dans les bornes, retourne la valeur à l'extrémité du map
     */
    qreal interpolate(qreal x, InterpolationType type = Interpolation2) const;



#ifndef NOSPLINE
    // calculateSpline calcule la courbe spline avec les éléments actuels
    void calculateSpline();

    // spline retourne chaque valeur de la spline calculée à l'aide de calculateSpline
    // si aucune spline n'a été calulée il lance calculateSpline automatiquement
    qreal spline(qreal x);
#endif


    /*     INTEGRATE
      Intègre sur l'intervale [a,b]
      Effectue la somme des aires parallèlogrammes rectangles formés entre les points
     */
    qreal integrate(qreal a, qreal b) const;


    /*     OPERATOR *
      Crée un nouveau PointMap en multipliant chaque points de chaqu'un des deux PointMap ensemble par interpolation.
      Le produit a en principe plus de valeurs que les deux facteurs.

      [(1,0)|(3,10)] * [(2,5)] = [(1,0)|(2,25)|(3,50)]
     */
    PointMap &operator *=(const PointMap &other); // overload of *
    PointMap operator *(const PointMap &other) const;

    /*     OPERATOR /
      Comme le * mais divise
     */
    PointMap &operator /=(const PointMap &other); // overload of /
    PointMap operator /(const PointMap &other) const;

    /*     OPERATOR *k
      Multiplie chaque valeur par k
     */
    PointMap &operator *=(qreal k);
    PointMap operator *(qreal k) const; // overload of *=

    /*     OPERATOR /k
      Divise chaque valeur par k
     */
    PointMap &operator /=(qreal k);
    PointMap operator /(qreal k) const; // overload of /=

private:
    qreal interpolate2(qreal x) const;
    qreal interpolate4(qreal x) const;
    qreal interpolate6(qreal x) const;

#ifndef NOSPLINE
    gsl_interp_accel *_acc;
    gsl_spline *_spline;
#endif
};

#endif // POINTMAP_H
