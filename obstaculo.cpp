#include "obstaculo.h"
#include <algorithm>

Obstaculo::Obstaculo(int idIn, double xIn, double yIn, double anchoIn, double altoIn, double eIn)
    : id(idIn), x(xIn), y(yIn), ancho(anchoIn), alto(altoIn), e(eIn) {}

bool Obstaculo::verificarColision(double px, double py, double radio,
                                   double &velX, double &velY)
{
    double izq    = x         - radio;
    double der    = x + ancho + radio;
    double arriba = y         - radio;
    double abajo  = y + alto  + radio;

    if (px < izq || px > der || py < arriba || py > abajo)
        return false;

    double pIzq    = px - izq;
    double pDer    = der - px;
    double pArriba = py - arriba;
    double pAbajo  = abajo - py;

    double minH = std::min(pIzq, pDer);
    double minV = std::min(pArriba, pAbajo);

    if (minH < minV)
    {
        if (pIzq < pDer && velX > 0)
        {
            velX *= -e; return true;
        }
        else if (pDer <= pIzq && velX < 0)
        {
            velX *= -e; return true;
        }
    }
    else
    {
        if (pArriba < pAbajo && velY > 0)
        {
            velY *= -e; return true;
        }
        else if (pAbajo <= pArriba && velY < 0)
        {
            velY *= -e; return true;
        }
    }
    return false;
}
