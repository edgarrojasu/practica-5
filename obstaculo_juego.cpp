#include "obstaculo_juego.h"
#include <algorithm>
#include <cmath>
#include <QString>

ObstaculoJuego::ObstaculoJuego(int idIn, double xIn, double yIn,
                                double anchoIn, double altoIn,
                                double eIn, double hpIn,
                                QGraphicsRectItem* itemIn,
                                QGraphicsTextItem* hpLabelIn)
    : id(idIn), x(xIn), y(yIn), ancho(anchoIn), alto(altoIn),
      e(eIn), hp(hpIn), hpMax(hpIn), item(itemIn), hpLabel(hpLabelIn)
{
    item->setRect(x, y, ancho, alto);
    actualizarLabel();
}

bool ObstaculoJuego::verificarColision(double px, double py, double radio,
                                        double &velX, double &velY, double masa)
{
    if (hp <= 0) return false;

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

    double minPenet = std::min({pIzq, pDer, pArriba, pAbajo});

    bool   golpe   = false;
    double vImpacto = 0.0;

    if      (minPenet == pArriba && velY > 0) { vImpacto = std::abs(velY); velY *= -e; golpe = true; }
    else if (minPenet == pAbajo  && velY < 0) { vImpacto = std::abs(velY); velY *= -e; golpe = true; }
    else if (minPenet == pIzq    && velX > 0) { vImpacto = std::abs(velX); velX *= -e; golpe = true; }
    else if (minPenet == pDer    && velX < 0) { vImpacto = std::abs(velX); velX *= -e; golpe = true; }

    if (golpe) {
        double danio = FACTOR_DANIO * masa * vImpacto;
        hp = std::max(0.0, hp - danio);
        actualizarLabel();
        if (hp <= 0) item->setOpacity(0.25);
    }
    return golpe;
}

void ObstaculoJuego::actualizarLabel() {
    hpLabel->setPlainText(QString::number(static_cast<int>(hp)));
    hpLabel->setPos(x + ancho / 2.0 - hpLabel->boundingRect().width()  / 2.0,
                    y + alto  / 2.0 - hpLabel->boundingRect().height() / 2.0);
}

bool   ObstaculoJuego::estaDestruido() const { return hp <= 0; }
double ObstaculoJuego::getHP()    const { return hp; }
double ObstaculoJuego::getX()     const { return x; }
double ObstaculoJuego::getY()     const { return y; }
double ObstaculoJuego::getAncho() const { return ancho; }
double ObstaculoJuego::getAlto()  const { return alto; }
int    ObstaculoJuego::getId()    const { return id; }
