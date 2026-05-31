#include "jugador.h"

Jugador::Jugador(int numeroIn, double xBaseIn, double yBaseIn, Proyectil* proy)
    : numero(numeroIn), xBase(xBaseIn), yBase(yBaseIn), proyectil(proy) {}

void Jugador::lanzar(double vel, double angGrados) {
    proyectil->lanzar(xBase, yBase, vel, angGrados);
}

bool Jugador::perdio() const {
    for (auto* obs : obstaculos)
        if (!obs->estaDestruido()) return false;
    return true;
}

int     Jugador::getNumero() const { return numero; }
double  Jugador::getXBase()  const { return xBase; }
double  Jugador::getYBase()  const { return yBase; }
QString Jugador::getNombre() const { return "Jugador " + QString::number(numero); }
