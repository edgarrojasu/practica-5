#include "particula.h"
#include <cmath>

Particula::Particula(int idIn, double xIn, double yIn, double velIn, double angIn,
                     double masaIn, double gravedad)
{
    id    = idIn;
    x     = xIn;
    y     = yIn;
    masa  = masaIn;
    radio = 10.0;
    g     = gravedad;
    activa = true;

    velX =  velIn * cos(angIn * (M_PI / 180));
    velY = -velIn * sin(angIn * (M_PI / 180));
}

void Particula::actualizarPosicion(double dt, double ancho, double alto)
{
    if (!activa) return;

    velY += g * dt;
    x += velX * dt;
    y += velY * dt;

    if (x > ancho && velX > 0)
    {
        x = ancho; velX *= -1;
    }
    if (x < 0     && velX < 0)
    {
        x = 0;     velX *= -1;
    }

    if (y > alto && velY > 0)
    {
        double pen = y - alto;
        y = alto - pen;
        velY *= -1;
    }
    if (y < 0 && velY < 0)
    {
        double pen = -y;
        y = pen;
        velY *= -1;
    }
}

bool Particula::verificarColisionObstaculos(const std::vector<Obstaculo*>& obstaculos)
{
    if (!activa) return false;
    for (Obstaculo* obs : obstaculos)
        if (obs->verificarColision(x, y, radio, velX, velY))
            return true;
    return false;
}

bool Particula::verificarColisionParticula(Particula* otra)
{
    if (!activa || !otra->activa) return false;

    double dx   = otra->x - x;
    double dy   = otra->y - y;
    double dist = sqrt(dx*dx + dy*dy);

    if (dist > (radio + otra->radio)) return false;

    double masaTotal = masa + otra->masa;
    double velXNueva = (masa * velX + otra->masa * otra->velX) / masaTotal;
    double velYNueva = (masa * velY + otra->masa * otra->velY) / masaTotal;

    //x = (masa * x + otra->masa * otra->x) / masaTotal;
    //y = (masa * y + otra->masa * otra->y) / masaTotal;

    radio = sqrt(radio*radio + otra->radio*otra->radio);
    masa  = masaTotal;
    velX  = velXNueva;
    velY  = velYNueva;

    otra->activa = false;
    return true;
}

void Particula::setGravedad(double nuevaG)
{
    g = nuevaG;
}
