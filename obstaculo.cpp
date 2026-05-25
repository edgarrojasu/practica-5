#include "obstaculo.h"
#include <algorithm>

Obstaculo::Obstaculo(double xIn, double yIn, double anchoIn, double altoIn,
                     double eIn, QGraphicsRectItem* itemIn)
    : x(xIn), y(yIn), ancho(anchoIn), alto(altoIn), e(eIn), item(itemIn)
{
    item->setRect(x, y, ancho, alto);
}

bool Obstaculo::verificarColision(double px, double py, double radio,
                                  double &velX, double &velY)
{
    // Bordes expandidos por el radio
    double izq    = x         - radio;
    double der    = x + ancho + radio;
    double arriba = y         - radio;
    double abajo  = y + alto  + radio;

    // Fuera del área: no hay colisión
    if (px < izq || px > der || py < arriba || py > abajo)
        return false;

    // Penetración desde cada borde
    double pIzq    = px - izq;
    double pDer    = der - px;
    double pArriba = py - arriba;
    double pAbajo  = abajo - py;

    double minH = std::min(pIzq, pDer);
    double minV = std::min(pArriba, pAbajo);

    if (minH < minV) {
        // Colisión lateral
        if (pIzq < pDer && velX > 0) {       // viene por la izquierda
            velX *= -e;
            return true;
        } else if (pDer <= pIzq && velX < 0) { // viene por la derecha
            velX *= -e;
            return true;
        }
    } else {
        // Colisión vertical
        // Qt: y crece hacia abajo
        // velY > 0 = baja  → choca con la cara SUPERIOR del obstáculo (pArriba mínima)
        // velY < 0 = sube  → choca con la cara INFERIOR del obstáculo (pAbajo mínima)
        if (pArriba < pAbajo && velY > 0) {   // baja y toca la cara de arriba
            velY *= -e;
            return true;
        } else if (pAbajo <= pArriba && velY < 0) { // sube y toca la cara de abajo
            velY *= -e;
            return true;
        }
    }

    return false;
}

QRectF Obstaculo::getRect() const {
    return QRectF(x, y, ancho, alto);
}
