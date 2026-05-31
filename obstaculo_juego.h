#ifndef OBSTACULO_JUEGO_H
#define OBSTACULO_JUEGO_H

#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QString>
#include <algorithm>

class ObstaculoJuego {
private:
    double x, y, ancho, alto;
    double e;
    double hp, hpMax;
    int id;
    QGraphicsRectItem*  item;
    QGraphicsTextItem*  hpLabel;

    static constexpr double FACTOR_DANIO = 0.05;

public:
    ObstaculoJuego(int id, double x, double y, double ancho, double alto,
                   double e, double hp, QGraphicsRectItem* item, QGraphicsTextItem* hpLabel)
        : id(id), x(x), y(y), ancho(ancho), alto(alto), e(e),
          hp(hp), hpMax(hp), item(item), hpLabel(hpLabel)
    {
        item->setRect(x, y, ancho, alto);
        actualizarLabel();
    }

    // Detecta colisión, aplica rebote inelástico y descuenta HP
    // Retorna true si hubo colisión
    bool verificarColision(double px, double py, double radio,
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

        bool golpe = false;
        double vImpacto = 0;

        if (minPenet == pArriba && velY > 0) {
            vImpacto = std::abs(velY); velY *= -e; golpe = true;
        } else if (minPenet == pAbajo && velY < 0) {
            vImpacto = std::abs(velY); velY *= -e; golpe = true;
        } else if (minPenet == pIzq && velX > 0) {
            vImpacto = std::abs(velX); velX *= -e; golpe = true;
        } else if (minPenet == pDer && velX < 0) {
            vImpacto = std::abs(velX); velX *= -e; golpe = true;
        }

        if (golpe) {
            // Daño = factor * masa * |v_impacto|
            double danio = FACTOR_DANIO * masa * vImpacto;
            hp = std::max(0.0, hp - danio);
            actualizarLabel();
            if (hp <= 0) item->setOpacity(0.25); // visualmente destruido
        }
        return golpe;
    }

    bool estaDestruido() const { return hp <= 0; }
    double getHP() const { return hp; }
    double getX() const { return x; }
    double getY() const { return y; }
    double getAncho() const { return ancho; }
    double getAlto() const { return alto; }
    int getId() const { return id; }

    void actualizarLabel() {
        hpLabel->setPlainText(QString::number((int)hp));
        // Centrar label sobre el obstáculo
        hpLabel->setPos(x + ancho/2 - hpLabel->boundingRect().width()/2,
                        y + alto/2  - hpLabel->boundingRect().height()/2);
    }
};

#endif
