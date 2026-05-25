#ifndef OBSTACULO_H
#define OBSTACULO_H

#include <QGraphicsRectItem>
#include <QRectF>

class Obstaculo {
private:
    double x, y;
    double ancho, alto;
    double e;
    QGraphicsRectItem* item;

public:
    Obstaculo(double x, double y, double ancho, double alto,
              double e, QGraphicsRectItem* item);

    bool verificarColision(double px, double py, double radio,
                           double &velX, double &velY);

    QRectF getRect() const;
};

#endif
