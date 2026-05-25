#include "mainwindow.h"

// Helper para crear una partícula con su ítem visual
static Particula* crearParticula(QGraphicsScene* scene,
                                  double x, double y,
                                  double vel, double ang,
                                  double masa, double g,
                                  QColor color)
{
    auto* item = new QGraphicsEllipseItem(-10, -10, 20, 20);
    item->setBrush(color);
    scene->addItem(item);
    return new Particula(x, y, vel, ang, masa, g, item);
}

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), g(9.81)
{
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, ANCHO, ALTO);
    view = new QGraphicsView(scene);
    view->setFixedSize(1000, 650);

    // --- Obstáculo ---
    auto* rect1 = new QGraphicsRectItem();
    rect1->setBrush(Qt::blue);
    scene->addItem(rect1);
    obstaculos.push_back(new Obstaculo(400, 350, 80, 80, 0.6, rect1));

    // --- 4 Partículas con posiciones, velocidades y masas distintas ---
    particulas.push_back(crearParticula(scene,  50, 300,  80,  65, 1.0, g, Qt::red));
    particulas.push_back(crearParticula(scene, 900, 200,  70, 110, 1.5, g, Qt::green));
    particulas.push_back(crearParticula(scene, 200, 500,  60,  40, 2.0, g, Qt::yellow));
    particulas.push_back(crearParticula(scene, 700, 400,  90,  80, 1.0, g, Qt::cyan));

    // --- Botón ---
    btnGravedad = new QPushButton("Change Gravity");
    connect(btnGravedad, &QPushButton::clicked, this, &MainWindow::onChangeGravity);

    QWidget*     central = new QWidget(this);
    QVBoxLayout* layout  = new QVBoxLayout(central);
    layout->addWidget(view);
    layout->addWidget(btnGravedad);
    setCentralWidget(central);

    // --- Timer ---
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::onTick);
    timer->start(50);
}

MainWindow::~MainWindow() {
    for (auto* p : particulas) delete p;
    for (auto* o : obstaculos) delete o;
}

void MainWindow::onTick() {
    // 1. Mover cada partícula y verificar obstáculos
    for (auto* p : particulas) {
        p->actualizarPosicion(DT, ANCHO, ALTO);
        p->verificarColisionObstaculos(obstaculos);
    }

    // 2. Verificar colisiones entre partículas (completamente inelásticas)
    for (int i = 0; i < (int)particulas.size(); i++) {
        for (int j = i + 1; j < (int)particulas.size(); j++) {
            if (particulas[i]->estaActiva() && particulas[j]->estaActiva()) {
                if (particulas[i]->verificarColisionParticula(particulas[j])) {
                    qDebug() << "Colision inelastica: particula" << i
                             << "absorbe particula" << j
                             << "| nueva masa:" << particulas[i]->getMasa()
                             << "| vel:(" << particulas[i]->getVelX()
                             << "," << particulas[i]->getVelY() << ")";
                }
            }
        }
    }
}

void MainWindow::onChangeGravity() {
    g *= 2;
    for (auto* p : particulas)
        p->setGravedad(g);
}
