#include "mainwindow.h"
#include <QFont>
#include <QMessageBox>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), jugador1(nullptr), jugador2(nullptr),
      turnoActual(1), simulando(false)
{
    setupUI();
    setupEscena();

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::onTick);
    timer->start(16); // ~60 FPS
}

MainWindow::~MainWindow() {
    delete jugador1;
    delete jugador2;
}

// ─── UI ──────────────────────────────────────────────────────────────────────
void MainWindow::setupUI() {
    setWindowTitle("Juego de colisiones — Práctica 5");

    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, ANCHO, ALTO);

    view = new QGraphicsView(scene);
    view->setFixedSize(static_cast<int>(ANCHO) + 4,
                       static_cast<int>(ALTO)  + 4);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setRenderHint(QPainter::Antialiasing);

    lblTurno = new QLabel("Turno: Jugador 1");
    lblTurno->setStyleSheet("font-weight:bold; font-size:14px; color:#1a5276;");

    lblEstado = new QLabel("Configura y dispara.");

    sliderAngulo    = new QSlider(Qt::Horizontal);
    sliderVelocidad = new QSlider(Qt::Horizontal);
    sliderAngulo->setRange(1, 89);
    sliderAngulo->setValue(45);
    sliderVelocidad->setRange(10, 400);
    sliderVelocidad->setValue(180);

    lblAngulo    = new QLabel("Ángulo: 45°");
    lblVelocidad = new QLabel("Velocidad: 180");

    connect(sliderAngulo, &QSlider::valueChanged, [&](int v) {
        lblAngulo->setText("Ángulo: " + QString::number(v) + "°");
    });
    connect(sliderVelocidad, &QSlider::valueChanged, [&](int v) {
        lblVelocidad->setText("Velocidad: " + QString::number(v));
    });

    btnDisparar = new QPushButton("¡Disparar!");
    btnDisparar->setFixedHeight(40);
    btnDisparar->setStyleSheet(
        "QPushButton { background:#2e86c1; color:white; font-weight:bold; "
        "border-radius:6px; font-size:13px; }"
        "QPushButton:hover { background:#1a5276; }"
        "QPushButton:disabled { background:#aab; }");
    connect(btnDisparar, &QPushButton::clicked, this, &MainWindow::onDisparar);

    auto* ctrlBox    = new QGroupBox("Controles");
    auto* ctrlLayout = new QVBoxLayout(ctrlBox);
    ctrlLayout->addWidget(lblTurno);
    ctrlLayout->addWidget(lblEstado);
    ctrlLayout->addSpacing(8);
    ctrlLayout->addWidget(lblAngulo);
    ctrlLayout->addWidget(sliderAngulo);
    ctrlLayout->addWidget(lblVelocidad);
    ctrlLayout->addWidget(sliderVelocidad);
    ctrlLayout->addSpacing(8);
    ctrlLayout->addWidget(btnDisparar);
    ctrlLayout->addStretch();

    auto* central    = new QWidget(this);
    auto* mainLayout = new QHBoxLayout(central);
    mainLayout->addWidget(view);
    mainLayout->addWidget(ctrlBox);
    setCentralWidget(central);
}

// ─── HELPERS ─────────────────────────────────────────────────────────────────
QGraphicsTextItem* MainWindow::crearLabel(const QString& txt, double x, double y,
                                           int size, QColor color)
{
    auto* lbl = new QGraphicsTextItem(txt);
    QFont f;
    f.setPointSize(size);
    f.setBold(true);
    lbl->setFont(f);
    lbl->setDefaultTextColor(color);
    lbl->setPos(x, y);
    scene->addItem(lbl);
    return lbl;
}

ObstaculoJuego* MainWindow::crearObstaculo(double x, double y, double w, double h,
                                            double e, double hp, QColor color)
{
    auto* rectItem = new QGraphicsRectItem();
    rectItem->setBrush(color);
    rectItem->setPen(QPen(color.darker(140), 1.5));
    scene->addItem(rectItem);

    auto* lblItem = new QGraphicsTextItem();
    QFont f;
    f.setBold(true);
    f.setPointSize(10);
    lblItem->setFont(f);
    lblItem->setDefaultTextColor(Qt::white);
    scene->addItem(lblItem);

    static int nextId = 1;
    return new ObstaculoJuego(nextId++, x, y, w, h, e, hp, rectItem, lblItem);
}

// ─── ESCENA ──────────────────────────────────────────────────────────────────
void MainWindow::setupEscena() {
    scene->setBackgroundBrush(QColor(245, 248, 255));
    scene->addRect(0, 0, ANCHO, ALTO, QPen(Qt::black, 2));

    // Divisor central
    scene->addLine(ANCHO / 2, 0, ANCHO / 2, ALTO,
                   QPen(QColor(150,150,150), 1, Qt::DashLine));

    // Colores
    QColor azul(70, 130, 180);
    QColor rojo(200, 80, 80);
    double e  = 0.7;
    double hL = 200.0; // hp laterales
    double hC = 100.0; // hp central

    // Obstáculos J1 (izquierda) — coordenadas del mainwindow.cpp del usuario
    auto* o1a = crearObstaculo(  0, 370,  90, 130, e, hL, azul);
    auto* o1b = crearObstaculo( 90, 270, 120, 100, e, hC, azul);
    auto* o1c = crearObstaculo(210, 370,  90, 160, e, hL, azul);

    // Obstáculos J2 (derecha)
    auto* o2a = crearObstaculo(600, 370, 90, 160, e, hL, rojo);
    auto* o2b = crearObstaculo(690, 270,120, 100, e, hC, rojo);
    auto* o2c = crearObstaculo(810, 370, 90, 160, e, hL, rojo);

    // Proyectiles
    auto makeProy = [&](QColor color) -> Proyectil* {
        auto* ellItem = new QGraphicsEllipseItem(0, 0, RADIO_P * 2, RADIO_P * 2);
        ellItem->setBrush(color);
        ellItem->setPen(QPen(color.darker(160), 1));
        scene->addItem(ellItem);
        return new Proyectil(MASA_P, RADIO_P, G, MAX_REB, ellItem);
    };

    Proyectil* proy1 = makeProy(QColor(30, 100, 200));
    Proyectil* proy2 = makeProy(QColor(200,  50,  50));

    // Jugadores — J1 lanza desde izq, J2 desde der
    jugador1 = new Jugador(1, 10.0,         ALTO - 20.0, proy1);
    jugador2 = new Jugador(2, ANCHO - 10.0, ALTO - 20.0, proy2);
    jugador1->obstaculos = {o1a, o1b, o1c};
    jugador2->obstaculos = {o2a, o2b, o2c};

    // Etiquetas
    crearLabel("Jugador 1", 10,           10, 11, QColor(30, 100, 200));
    crearLabel("Jugador 2", ANCHO - 80.0, 10, 11, QColor(180, 30,  30));

    // Marcadores de posición de lanzamiento
    const double r = 7;
    scene->addEllipse(jugador1->getXBase() - r, jugador1->getYBase() - r,
                      r*2, r*2, QPen(Qt::blue, 2), QBrush(QColor(30,100,200)));
    scene->addEllipse(jugador2->getXBase() - r, jugador2->getYBase() - r,
                      r*2, r*2, QPen(Qt::red,  2), QBrush(QColor(200,50,50)));
}

// ─── SLOTS ───────────────────────────────────────────────────────────────────
void MainWindow::onDisparar() {
    if (simulando) return;

    Jugador* actual = (turnoActual == 1) ? jugador1 : jugador2;
    double ang = sliderAngulo->value();
    double vel = sliderVelocidad->value();

    // J2 dispara hacia la izquierda
    if (turnoActual == 2) ang = 180.0 - ang;

    actual->lanzar(vel, ang);
    simulando = true;
    btnDisparar->setEnabled(false);
    lblEstado->setText("Simulando...");
}

void MainWindow::onTick() {
    if (!simulando) return;

    Jugador* actual = (turnoActual == 1) ? jugador1 : jugador2;
    Jugador* rival  = (turnoActual == 1) ? jugador2 : jugador1;

    if (!actual->proyectil->estaActivo()) {
        simulando = false;
        verificarVictoria();
        if (!simulando) {
            cambiarTurno();
            btnDisparar->setEnabled(true);
        }
        return;
    }

    actual->proyectil->actualizar(DT, ANCHO, ALTO, rival->obstaculos);
}

void MainWindow::cambiarTurno() {
    turnoActual = (turnoActual == 1) ? 2 : 1;
    QString nombre = ((turnoActual == 1) ? jugador1 : jugador2)->getNombre();
    lblTurno->setText("Turno: " + nombre);
    lblEstado->setText("Configura y dispara.");
    sliderAngulo->setValue(45);
}

void MainWindow::verificarVictoria() {
    if (jugador2->perdio()) {
        QMessageBox::information(this, "¡Fin del juego!",
            "¡Jugador 1 ganó! Destruyó todos los obstáculos del rival.");
        btnDisparar->setEnabled(false);
        simulando = true;
    } else if (jugador1->perdio()) {
        QMessageBox::information(this, "¡Fin del juego!",
            "¡Jugador 2 ganó! Destruyó todos los obstáculos del rival.");
        btnDisparar->setEnabled(false);
        simulando = true;
    }
}
