#ifndef PARTICULA_H
#define PARTICULA_H

#include <cmath>
#include <string>
#include "obstaculo.h"
#include <vector>

class Particula {
private:
    double x, y;
    double velX, velY;
    double masa;
    double radio;
    double g;
    bool activa;
    int id;

public:
    Particula(int id, double xIn, double yIn, double velIn, double angIn,
              double masa, double gravedad);

    void actualizarPosicion(double dt, double ancho, double alto);
    bool verificarColisionObstaculos(const std::vector<Obstaculo*>& obstaculos);
    bool verificarColisionParticula(Particula* otra);

    void setGravedad(double nuevaG);
    bool estaActiva() const { return activa; }

    double getX()    const { return x; }
    double getY()    const { return y; }
    double getVelX() const { return velX; }
    double getVelY() const { return velY; }
    double getMasa() const { return masa; }
    double getRadio() const { return radio; }
    int    getId()   const { return id; }
};

#endif
