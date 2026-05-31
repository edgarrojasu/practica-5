#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include "particula.h"
#include "obstaculo.h"

using namespace std;


int main()
{
    std::ofstream archivo("trayectoria_particulas.txt");
    if (!archivo.is_open())
    {
        std::cerr << "Error al abrir archivo\n";
        return 1;
    }

    const double DT    = 0.1;
    const double ANCHO = 950;
    const double ALTO  = 700;
    const double G     = 9.81;
    const int    PASOS = 3000;

    archivo << "# Simulacion de particulas\n";
    archivo << "# Posiciones: P<id> x y velX velY masa\n";
    archivo << "# Eventos: COLISION_OBSTACULO, COLISION_PARTICULAS\n";
    archivo << "# ANCHO=" << ANCHO << " ALTO=" << ALTO << "\n\n";

    std::vector<Particula*> particulas;
    particulas.push_back(new Particula(1,  50, 300,  100,  65, 1.0, G));
    particulas.push_back(new Particula(2, 900, 200,  100, 110, 1.5, G));
    particulas.push_back(new Particula(3, 200, 500,  100,  40, 2.0, G));
    particulas.push_back(new Particula(4, 700, 400,  100,  80, 1.0, G));

    std::vector<Obstaculo*> obstaculos;
    obstaculos.push_back(new Obstaculo(1, 300, 400, 80, 80, 1.0));
    obstaculos.push_back(new Obstaculo(2, 600, 250, 80, 80, 1.0));
    obstaculos.push_back(new Obstaculo(3, 150, 550, 80, 80, 0.8));
    obstaculos.push_back(new Obstaculo(4, 750, 500, 80, 80, 0.8));

    archivo << "# OBSTACULOS\n";
    for (auto* obs : obstaculos)
        archivo << "OBS" << obs->getId() << " "
                << obs->getX()     << " " << obs->getY() << " "
                << obs->getAncho() << " " << obs->getAlto() << "\n";
    archivo << "\n";

    for (int paso = 0; paso < PASOS; paso++)
    {
        double t = paso * DT;
        archivo << std::fixed << std::setprecision(3);

        archivo << "t=" << t << "\n";
        for (auto* p : particulas)
        {
            if (!p->estaActiva()) continue;
            archivo << "P" << p->getId() << " "
                    << p->getX()    << " " << p->getY()    << " "
                    << p->getVelX() << " " << p->getVelY() << " "
                    << p->getMasa() << "\n";
        }

        for (auto* p : particulas)
            p->actualizarPosicion(DT, ANCHO, ALTO);

        for (auto* p : particulas)
        {
            if (!p->estaActiva()) continue;
            double vx0 = p->getVelX(), vy0 = p->getVelY();
            if (p->verificarColisionObstaculos(obstaculos))
            {
                archivo << "COLISION_OBSTACULO t=" << t
                        << " P" << p->getId()
                        << " x=" << p->getX() << " y=" << p->getY()
                        << " vel_antes=(" << vx0 << "," << vy0 << ")"
                        << " vel_despues=(" << p->getVelX() << "," << p->getVelY() << ")\n";
            }
        }

        for (int i = 0; i < (int)particulas.size(); i++)
        {
            for (int j = i+1; j < (int)particulas.size(); j++)
            {
                if (!particulas[i]->estaActiva() || !particulas[j]->estaActiva()) continue;
                double m1 = particulas[i]->getMasa();
                double m2 = particulas[j]->getMasa();
                if (particulas[i]->verificarColisionParticula(particulas[j]))
                {
                    archivo << "COLISION_PARTICULAS t=" << t
                            << " P" << particulas[i]->getId()
                            << "+P" << particulas[j]->getId()
                            << " masas=(" << m1 << "+" << m2 << ")"
                            << " masaNueva=" << particulas[i]->getMasa()
                            << " pos=(" << particulas[i]->getX() << "," << particulas[i]->getY() << ")"
                            << " vel=(" << particulas[i]->getVelX() << "," << particulas[i]->getVelY() << ")\n";
                }
            }
        }

        archivo << "\n";
    }

    archivo.close();
    cout << "Simulacion completada. Datos en trayectoria_particulas.txt\n";

    for (auto* p : particulas) delete p;
    for (auto* o : obstaculos) delete o;
    return 0;
}
