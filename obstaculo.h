#ifndef OBSTACULO_H
#define OBSTACULO_H

class Obstaculo {
private:
    double x, y;
    double ancho, alto;
    double e;
    int id;

public:
    Obstaculo(int id, double x, double y, double ancho, double alto, double e);

    bool verificarColision(double px, double py, double radio,
                           double &velX, double &velY);

    double getX()     const { return x; }
    double getY()     const { return y; }
    double getAncho() const { return ancho; }
    double getAlto()  const { return alto; }
    int    getId()    const { return id; }
};

#endif
