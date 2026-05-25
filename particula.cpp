#include "particula.h"
#include <cmath>

Particula::Particula(double xIn, double yIn, double velIn, double angIn,
                     double masaIn, double gravedad, QGraphicsEllipseItem* itemIn)
{
    x     = xIn;
    y     = yIn;
    masa  = masaIn;
    radio = 10.0;   // radio base; crece con la masa
    g     = gravedad;
    activa = true;
    item  = itemIn;

    velX =  velIn * cos(angIn * (M_PI / 180));
    velY = -velIn * sin(angIn * (M_PI / 180));

    item->setPos(x, y);
}

void Particula::actualizarPosicion(double dt, double ancho, double alto) {
    if (!activa) return;

    velY += g * dt;

    x += velX * dt;
    y += velY * dt;

    // Paredes — elásticas, con corrección de penetración
    if (x > ancho && velX > 0) { x = ancho; velX *= -1; }
    if (x < 0     && velX < 0) { x = 0;     velX *= -1; }

    // Suelo/techo — elásticas, corregir penetración simétricamente
    if (y > alto && velY > 0) {
        double pen = y - alto;
        y = alto - pen;
        velY *= -1;
    }
    if (y < 0 && velY < 0) {
        double pen = -y;
        y = pen;
        velY *= -1;
    }

    item->setPos(x, y);
}

void Particula::verificarColisionObstaculos(const std::vector<Obstaculo*>& obstaculos) {
    if (!activa) return;
    for (Obstaculo* obs : obstaculos)
        obs->verificarColision(x, y, radio, velX, velY);
}

bool Particula::verificarColisionParticula(Particula* otra) {
    if (!activa || !otra->activa) return false;

    double dx   = otra->x - x;
    double dy   = otra->y - y;
    double dist = sqrt(dx*dx + dy*dy);
    double sumRadios = radio + otra->radio;

    if (dist > sumRadios) return false;  // no se tocan

    // Colisión completamente inelástica: conservación de momento lineal
    // v' = (m1*v1 + m2*v2) / (m1 + m2)
    double masaTotal = masa + otra->masa;
    double velXNueva = (masa * velX + otra->masa * otra->velX) / masaTotal;
    double velYNueva = (masa * velY + otra->masa * otra->velY) / masaTotal;

    // Centro de masa como nueva posición
    double xNuevo = (masa * x + otra->masa * otra->x) / masaTotal;
    double yNuevo = (masa * y + otra->masa * otra->y) / masaTotal;

    // Nueva masa y radio (área proporcional a la masa)
    masa  = masaTotal;
    radio = sqrt(radio*radio + otra->radio*otra->radio); // conservar área

    velX = velXNueva;
    velY = velYNueva;
    x    = xNuevo;
    y    = yNuevo;

    // Actualizar visual: nuevo tamaño centrado en (0,0) del ítem
    item->setRect(-radio, -radio, radio*2, radio*2);
    item->setPos(x, y);

    // Desactivar la otra partícula
    otra->activa = false;
    otra->item->setVisible(false);

    return true;
}

void Particula::setGravedad(double nuevaG) {
    g = nuevaG;
}
