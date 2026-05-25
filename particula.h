#ifndef PARTICULA_H
#define PARTICULA_H

#include <cmath>
<<<<<<< HEAD
#include <fstream>
using namespace std;
=======
#include <QGraphicsEllipseItem>
>>>>>>> widget

class Particula {
private:
    double x, y;
    double velX, velY;
<<<<<<< HEAD
=======
    double velInY;       // velY inicial del tramo actual (para la fórmula cinemática)
>>>>>>> widget
    double g;
    double tiempoX, tiempoY;
    double ang;
    int dir;
    bool colX;

<<<<<<< HEAD
public:
    Particula(double xIn, double yIn, double velIn, double angIn, double gravedad);
    void actualizarPosicion(double dt, double ancho, double alto,ofstream &archivo);
    void actualizarValores(double xIn, double yIn);
    bool estaQuieto() const;
=======
    QGraphicsEllipseItem* item; // representación visual

public:
    Particula(double xIn, double yIn, double velIn, double angIn, double gravedad,
              QGraphicsEllipseItem* itemIn);

    void actualizarPosicion(double dt, double ancho, double alto);
    void actualizarValores(double xIn, double yIn);
    bool estaQuieto() const;
    void setGravedad(double nuevaG);
>>>>>>> widget
};

#endif // PARTICULA_H
