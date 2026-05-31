#ifndef JUGADOR_H
#define JUGADOR_H

#include <vector>
#include <QString>
#include "obstaculo_juego.h"
#include "proyectil.h"

class Jugador {
private:
    int    numero;       // 1 o 2
    double xBase;        // posición X de lanzamiento
    double yBase;        // posición Y de lanzamiento
    bool   vivo;

public:
    std::vector<ObstaculoJuego*> obstaculos;
    Proyectil* proyectil;

    Jugador(int numero, double xBase, double yBase, Proyectil* proy)
        : numero(numero), xBase(xBase), yBase(yBase), vivo(true), proyectil(proy) {}

    void lanzar(double vel, double angGrados) {
        proyectil->lanzar(xBase, yBase, vel, angGrados);
    }

    // Verifica si todos los obstáculos propios están destruidos
    bool perdio() const {
        for (auto* obs : obstaculos)
            if (!obs->estaDestruido()) return false;
        return true;
    }

    int    getNumero() const { return numero; }
    double getXBase()  const { return xBase; }
    double getYBase()  const { return yBase; }
    QString getNombre() const { return "Jugador " + QString::number(numero); }
};

#endif
