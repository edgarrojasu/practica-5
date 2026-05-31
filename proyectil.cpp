#include "proyectil.h"
#include <cmath>

Proyectil::Proyectil(double masaIn, double radioIn, double gIn, int maxRebIn,
                     QGraphicsEllipseItem* itemIn)
    : x(0), y(0), velX(0), velY(0),
      masa(masaIn), radio(radioIn), g(gIn),
      rebotes(0), maxRebotes(maxRebIn),
      activo(false), item(itemIn)
{
    item->setVisible(false);
}

void Proyectil::lanzar(double xIn, double yIn, double vel, double angGrados) {
    x       = xIn;
    y       = yIn;
    velX    =  vel * std::cos(angGrados * M_PI / 180.0);
    velY    = -vel * std::sin(angGrados * M_PI / 180.0);
    rebotes = 0;
    activo  = true;
    item->setVisible(true);
    item->setPos(x - radio, y - radio);
}

void Proyectil::actualizar(double dt, double ancho, double alto,
                            std::vector<ObstaculoJuego*>& obstaculos)
{
    if (!activo) return;

    // Varios sub-pasos por tick para mayor precisión y velocidad visual
    const int SUBPASOS = 3;
    double dts = dt / SUBPASOS;

    for (int s = 0; s < SUBPASOS; s++) {
        velY += g * dts;
        x    += velX * dts;
        y    += velY * dts;

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

        if (rebotes >= maxRebotes) { detener(); return; }
    }

    item->setPos(x - radio, y - radio);
}

void Proyectil::detener() {
    activo = false;
    item->setVisible(false);
}

bool   Proyectil::estaActivo() const { return activo; }
double Proyectil::getX()       const { return x; }
double Proyectil::getY()       const { return y; }
double Proyectil::getMasa()    const { return masa; }
