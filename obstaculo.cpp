#include "obstaculo.h"
#include <algorithm>

Obstaculo::Obstaculo(int idIn, double xIn, double yIn, double anchoIn, double altoIn, double eIn)
    : id(idIn), x(xIn), y(yIn), ancho(anchoIn), alto(altoIn), e(eIn) {}

bool Obstaculo::verificarColision(double px, double py, double radio,
                                   double &velX, double &velY)
{
    double izq    = x - (radio/2);
    double der    = x + ancho + (radio/2);
    double arriba = y - (radio/2);
    double abajo  = y + alto  + (radio/2);

    if (px < izq || px > der || py < arriba || py > abajo)
        return false;

    double pIzq    = px - izq;
    double pDer    = der - px;
    double pArriba = py - arriba;
    double pAbajo  = abajo - py;

    double minPenet = std::min({pIzq, pDer, pArriba, pAbajo});

    if (minPenet == pArriba && velY > 0) { velY *= -e; return true; }
    if (minPenet == pAbajo  && velY < 0) { velY *= -e; return true; }
    if (minPenet == pIzq    && velX > 0) { velX *= -e; return true; }
    if (minPenet == pDer    && velX < 0) { velX *= -e; return true; }

    return false;
}
