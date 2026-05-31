#ifndef OBSTACULO_JUEGO_H
#define OBSTACULO_JUEGO_H

#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QColor>

class ObstaculoJuego {
private:
    double x, y, ancho, alto;
    double e;
    double hp, hpMax;
    int id;
    QGraphicsRectItem* item;
    QGraphicsTextItem* hpLabel;
    static constexpr double FACTOR_DANIO = 0.05;

public:
    ObstaculoJuego(int id, double x, double y, double ancho, double alto,
                   double e, double hp,
                   QGraphicsRectItem* item, QGraphicsTextItem* hpLabel);

    bool verificarColision(double px, double py, double radio,
                           double &velX, double &velY, double masa);

    void actualizarLabel();

    bool   estaDestruido() const;
    double getHP()    const;
    double getX()     const;
    double getY()     const;
    double getAncho() const;
    double getAlto()  const;
    int    getId()    const;
};

#endif
