#ifndef PROYECTIL_H
#define PROYECTIL_H

#include <cmath>
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
              QGraphicsEllipseItem* item)
        : x(0), y(0), velX(0), velY(0),
          masa(masa), radio(radio), g(g),
          rebotes(0), maxRebotes(maxRebotes),
          activo(false), item(item)
    {
        item->setVisible(false);
    }

    void lanzar(double xIn, double yIn, double vel, double angGrados) {
        x = xIn; y = yIn;
        velX =  vel * cos(angGrados * M_PI / 180.0);
        velY = -vel * sin(angGrados * M_PI / 180.0);
        rebotes = 0;
        activo  = true;
        item->setVisible(true);
        item->setPos(x, y);
    }

    void actualizar(double dt, double ancho, double alto,
                    std::vector<ObstaculoJuego*>& obstaculos)
    {
        if (!activo) return;

        velY += g * dt;
        x += velX * dt;
        y += velY * dt;

        // Paredes — elásticas
        if (x > ancho && velX > 0) { x = ancho; velX *= -1; rebotes++; }
        if (x < 0     && velX < 0) { x = 0;     velX *= -1; rebotes++; }

        // Suelo/techo — elásticos con corrección de penetración
        if (y > alto && velY > 0) {
            double pen = y - alto; y = alto - pen; velY *= -1; rebotes++;
        }
        if (y < 0 && velY < 0) {
            double pen = -y; y = pen; velY *= -1; rebotes++;
        }

        // Obstáculos — inelásticos
        for (auto* obs : obstaculos)
            obs->verificarColision(x, y, radio, velX, velY, masa);

        // Parar si superó rebotes máximos
        if (rebotes >= maxRebotes) detener();

        item->setPos(x - radio, y - radio);
    }

    void detener() {
        activo = false;
        item->setVisible(false);
    }

    bool estaActivo()  const { return activo; }
    double getX()      const { return x; }
    double getY()      const { return y; }
    double getMasa()   const { return masa; }
};

#endif
