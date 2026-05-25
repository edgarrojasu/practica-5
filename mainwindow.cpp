#include "mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), g(9.81)
{
    // Escena y vista
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, ANCHO, ALTO);

    view = new QGraphicsView(scene);
    view->setFixedSize(1000, 650);

    // Partícula visual
    item = new QGraphicsEllipseItem(-5, -5, 20, 20);
    item->setBrush(Qt::red);
    scene->addItem(item);

    // Lógica de la partícula
    particula = new Particula(50, 250, 80, 65, g, item);

    // Botón
    btnGravedad = new QPushButton("Change Gravity");
    connect(btnGravedad, &QPushButton::clicked, this, &MainWindow::onChangeGravity);

    // Layout central
    QWidget*     central = new QWidget(this);
    QVBoxLayout* layout  = new QVBoxLayout(central);
    layout->addWidget(view);
    layout->addWidget(btnGravedad);
    setCentralWidget(central);

    // Timer
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::onTick);
    timer->start(50);
}

MainWindow::~MainWindow() {
    delete particula;
}

void MainWindow::onTick() {
    // Ya no chequea estaQuieto() porque las colisiones son elásticas
    particula->actualizarPosicion(DT, ANCHO, ALTO);
}

void MainWindow::onChangeGravity() {
    g *= 2;
    particula->setGravedad(g);
}
