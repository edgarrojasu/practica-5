#ifndef JUGADOR_H
#define JUGADOR_H

#include <vector>
#include <QString>
#include "obstaculo_juego.h"
#include "proyectil.h"

class Jugador {
private:
    int    numero;
    double xBase;
    double yBase;

public:
    std::vector<ObstaculoJuego*> obstaculos;
    Proyectil* proyectil;

    Jugador(int numero, double xBase, double yBase, Proyectil* proy);

    void    lanzar(double vel, double angGrados);
    bool    perdio() const;
    int     getNumero() const;
    double  getXBase()  const;
    double  getYBase()  const;
    QString getNombre() const;
};

#endif
