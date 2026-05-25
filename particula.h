#ifndef PARTICULA_H
#define PARTICULA_H

#include <cmath>
#include <QGraphicsEllipseItem>
#include "obstaculo.h"
#include <vector>

class Particula {
private:
    double x, y;
    double velX, velY;
    double masa;
    double radio;
    double g;
    bool activa; // false si fue absorbida por otra partícula
    QGraphicsEllipseItem* item;

public:
    Particula(double xIn, double yIn, double velIn, double angIn,
              double masa, double gravedad, QGraphicsEllipseItem* itemIn);

    void actualizarPosicion(double dt, double ancho, double alto);
    void verificarColisionObstaculos(const std::vector<Obstaculo*>& obstaculos);

    // Colisión completamente inelástica con otra partícula
    // Retorna true si hubo colisión, y absorbe a 'otra' (la desactiva)
    bool verificarColisionParticula(Particula* otra);

    void setGravedad(double nuevaG);
    bool estaActiva() const { return activa; }

    double getX()    const { return x; }
    double getY()    const { return y; }
    double getVelX() const { return velX; }
    double getVelY() const { return velY; }
    double getMasa() const { return masa; }
    double getRadio() const { return radio; }
};

#endif
