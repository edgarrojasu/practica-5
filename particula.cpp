#include "particula.h"
#include <cmath>

Particula::Particula(double xIn, double yIn, double velIn, double angIn, double gravedad,
                     QGraphicsEllipseItem* itemIn)
{
    x = xIn;  y = yIn;
    g = gravedad;
    tiempoX = 0;  tiempoY = 0;
    ang = angIn;
    dir = 1;
    colX = true;
    item = itemIn;

    velX  = velIn * cos(ang * (M_PI / 180));
    velY  = velIn * sin(ang * (M_PI / 180));
    velInY = velY;

    item->setPos(x, y);
}

void Particula::actualizarPosicion(double dt, double ancho, double alto) {
    // Cinemática
    double xNuevo = x + velX * tiempoX * dir;
    double yNuevo = y - (velInY * tiempoY - 0.5 * g * tiempoY * tiempoY);
    velY = velInY - g * tiempoY;  // velocidad instantánea

    // Colisión horizontal
    if (xNuevo > ancho || xNuevo < 0) {
        colX = true;
        actualizarValores(xNuevo, yNuevo);
    }

    // Colisión vertical
    if (yNuevo > alto || yNuevo < 0) {
        yNuevo = (yNuevo < 0) ? 0 : alto;
        colX = false;
        actualizarValores(xNuevo, yNuevo);

        velY   *= -1;    // rebote perfectamente elástico
        velInY  = velY;  // sin pérdida de energía
    }

    // Mover el ítem visual
    item->setPos(xNuevo, yNuevo);

    tiempoX += dt;
    tiempoY += dt;
}

void Particula::actualizarValores(double xIn, double yIn) {
    if (colX) {
        dir *= -1;
        x = xIn;
        tiempoX = 0;
    } else {
        x = xIn;
        y = yIn;

        ang    = -atan2(velY, velX) * 180.0 / M_PI;
        tiempoY = 0;
        tiempoX = 0;
    }
}

bool Particula::estaQuieto() const {
    return (velInY == 0);
}

void Particula::setGravedad(double nuevaG) {
    g = nuevaG;
}
