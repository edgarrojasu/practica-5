#ifndef PARTICULA_H
#define PARTICULA_H

#include <cmath>
#include <QGraphicsEllipseItem>

class Particula {
private:
    double x, y;
    double velX, velY;
    double velInY;       // velY inicial del tramo actual (para la fórmula cinemática)
    double g;
    double tiempoX, tiempoY;
    double ang;
    int dir;
    bool colX;

    QGraphicsEllipseItem* item; // representación visual

public:
    Particula(double xIn, double yIn, double velIn, double angIn, double gravedad,
              QGraphicsEllipseItem* itemIn);

    void actualizarPosicion(double dt, double ancho, double alto);
    void actualizarValores(double xIn, double yIn);
    bool estaQuieto() const;
    void setGravedad(double nuevaG);
};

#endif // PARTICULA_H
