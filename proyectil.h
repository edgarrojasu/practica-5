#ifndef PROYECTIL_H
#define PROYECTIL_H

#include <QGraphicsEllipseItem>
#include <vector>
#include "obstaculo_juego.h"

class Proyectil {
private:
    double x, y;
    double velX, velY;
    double masa;
    double radio;
    double g;
    int    rebotes;
    int    maxRebotes;
    bool   activo;
    QGraphicsEllipseItem* item;

public:
    Proyectil(double masa, double radio, double g, int maxRebotes,
              QGraphicsEllipseItem* item);

    void lanzar(double xIn, double yIn, double vel, double angGrados);
    void actualizar(double dt, double ancho, double alto,
                    std::vector<ObstaculoJuego*>& obstaculos);
    void detener();

    bool   estaActivo() const;
    double getX()       const;
    double getY()       const;
    double getMasa()    const;
};

#endif
