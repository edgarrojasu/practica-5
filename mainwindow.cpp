#include "mainwindow.h"
#include <QFont>
#include <QMessageBox>
#include <cmath>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), turnoActual(1), simulando(false)
{
    setupUI();
    setupEscena();

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::onTick);
    timer->start(static_cast<int>(DT * 1000));
}

MainWindow::~MainWindow() {
    delete jugador1;
    delete jugador2;
}

// ─── UI ──────────────────────────────────────────────────────────────────────
void MainWindow::setupUI() {
    setWindowTitle("Juego de colisiones — Práctica 5");

    // Escena y vista
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, ANCHO, ALTO);
    view = new QGraphicsView(scene);
    view->setFixedSize(static_cast<int>(ANCHO) + 4,
                       static_cast<int>(ALTO)  + 4);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Controles
    lblTurno = new QLabel("Turno: Jugador 1");
    lblTurno->setStyleSheet("font-weight:bold; font-size:14px;");

    lblEstado = new QLabel("Configura y dispara.");

    sliderAngulo     = new QSlider(Qt::Horizontal);
    sliderVelocidad  = new QSlider(Qt::Horizontal);
    sliderAngulo->setRange(1, 89);
    sliderAngulo->setValue(45);
    sliderVelocidad->setRange(10, 300);
    sliderVelocidad->setValue(120);

    lblAngulo     = new QLabel("Ángulo: 45°");
    lblVelocidad  = new QLabel("Velocidad: 120");

    connect(sliderAngulo,    &QSlider::valueChanged, [&](int v){
        lblAngulo->setText("Ángulo: " + QString::number(v) + "°");
    });
    connect(sliderVelocidad, &QSlider::valueChanged, [&](int v){
        lblVelocidad->setText("Velocidad: " + QString::number(v));
    });

    btnDisparar = new QPushButton("¡Disparar!");
    btnDisparar->setFixedHeight(36);
    connect(btnDisparar, &QPushButton::clicked, this, &MainWindow::onDisparar);

    // Layout controles
    auto* ctrlBox = new QGroupBox("Controles");
    auto* ctrlLayout = new QVBoxLayout(ctrlBox);
    ctrlLayout->addWidget(lblTurno);
    ctrlLayout->addWidget(lblEstado);
    ctrlLayout->addWidget(lblAngulo);
    ctrlLayout->addWidget(sliderAngulo);
    ctrlLayout->addWidget(lblVelocidad);
    ctrlLayout->addWidget(sliderVelocidad);
    ctrlLayout->addWidget(btnDisparar);

    // Layout principal
    auto* central   = new QWidget(this);
    auto* mainLayout = new QHBoxLayout(central);
    mainLayout->addWidget(view);
    mainLayout->addWidget(ctrlBox);
    setCentralWidget(central);
}

// ─── ESCENA ──────────────────────────────────────────────────────────────────
void MainWindow::setupEscena() {
    // Fondo
    scene->setBackgroundBrush(QColor(240, 245, 255));

    // Borde de la caja
    scene->addRect(0, 0, ANCHO, ALTO, QPen(Qt::black, 2));

    // Divisor central (visual)
    auto* div = scene->addLine(ANCHO/2, 0, ANCHO/2, ALTO,
                               QPen(Qt::gray, 1, Qt::DashLine));
    Q_UNUSED(div);

    // ── Obstáculos Jugador 1 (lado izquierdo) ─────────────────────────────
    //   2 laterales + 1 central, disposición como PDF
    auto makeObs = [&](double x, double y, double w, double h,
                       double e, double hp, QColor color) -> ObstaculoJuego* {
        auto* rectItem = new QGraphicsRectItem();
        rectItem->setBrush(color);
        rectItem->setPen(QPen(color.darker(130), 1.5));
        scene->addItem(rectItem);

        auto* lblItem = new QGraphicsTextItem();
        QFont f; f.setBold(true); f.setPointSize(10);
        lblItem->setFont(f);
        lblItem->setDefaultTextColor(Qt::white);
        scene->addItem(lblItem);

        return new ObstaculoJuego(0, x, y, w, h, e, hp, rectItem, lblItem);
    };

    // J1: x en [30..410]
    double hpLat = 200, hpCen = 100, e1 = 0.7;
    auto* o1a = makeObs( 0, 370, 90, 130, e1, hpLat, QColor(70, 130, 180));  // lateral izq
    auto* o1b = makeObs(90, 270,120, 100, e1, hpCen, QColor(70, 130, 180));  // central
    auto* o1c = makeObs(210, 370, 90, 160, e1, hpLat, QColor(70, 130, 180));  // lateral der

    // J2: x en [480..860]
    auto* o2a = makeObs(600, 370, 90, 160, e1, hpLat, QColor(200,  80,  80)); // lateral izq
    auto* o2b = makeObs(690, 270,120, 100, e1, hpCen, QColor(200,  80,  80)); // central
    auto* o2c = makeObs(810, 370, 90, 160, e1, hpLat, QColor(200,  80,  80)); // lateral der

    // ── Proyectiles ───────────────────────────────────────────────────────
    auto makeProy = [&](QColor color) -> Proyectil* {
        auto* ellItem = new QGraphicsEllipseItem(0, 0,
                            RADIO_P*2, RADIO_P*2);
        ellItem->setBrush(color);
        ellItem->setPen(QPen(color.darker(150), 1));
        scene->addItem(ellItem);
        return new Proyectil(MASA_P, RADIO_P, G, MAX_REB, ellItem);
    };

    Proyectil* proy1 = makeProy(QColor(30, 100, 200));
    Proyectil* proy2 = makeProy(QColor(200, 50, 50));

    // ── Jugadores ─────────────────────────────────────────────────────────
    // J1 dispara desde la izquierda hacia la derecha
    jugador1 = new Jugador(1, 10, ALTO - 20, proy1);
    jugador1->obstaculos = {o1a, o1b, o1c};

    // J2 dispara desde la derecha hacia la izquierda (ángulo > 90°)
    jugador2 = new Jugador(2, ANCHO - 10, ALTO - 20, proy2);
    jugador2->obstaculos = {o2a, o2b, o2c};

    // Etiquetas de jugador
    crearLabel("Jugador 1", 10, 10, 11, QColor(30,100,200));
    crearLabel("Jugador 2", ANCHO - 80, 10, 11, QColor(180,30,30));

    // Iconos de posición de lanzamiento
    scene->addEllipse(jugador1->getXBase() - 6, jugador1->getYBase() - 6,
                      12, 12, QPen(Qt::blue,2), QBrush(Qt::blue));
    scene->addEllipse(jugador2->getXBase() - 6, jugador2->getYBase() - 6,
                      12, 12, QPen(Qt::red, 2), QBrush(Qt::red));
}

QGraphicsTextItem* MainWindow::crearLabel(const QString& txt, double x, double y,
                                           int size, QColor color)
{
    auto* lbl = new QGraphicsTextItem(txt);
    QFont f; f.setPointSize(size); f.setBold(true);
    lbl->setFont(f);
    lbl->setDefaultTextColor(color);
    lbl->setPos(x, y);
    scene->addItem(lbl);
    return lbl;
}

// ─── SLOTS ───────────────────────────────────────────────────────────────────
void MainWindow::onDisparar() {
    if (simulando) return;

    Jugador* actual = (turnoActual == 1) ? jugador1 : jugador2;
    double ang = sliderAngulo->value();
    double vel = sliderVelocidad->value();

    // J2 dispara hacia la izquierda: espejear ángulo
    if (turnoActual == 2) ang = 180.0 - ang;

    actual->lanzar(vel, ang);
    simulando = true;
    btnDisparar->setEnabled(false);
    lblEstado->setText("Simulando...");
}

void MainWindow::onTick() {
    Jugador* actual = (turnoActual == 1) ? jugador1 : jugador2;
    Jugador* rival  = (turnoActual == 1) ? jugador2 : jugador1;

    if (!actual->proyectil->estaActivo()) {
        if (simulando) {
            simulando = false;
            verificarVictoria();
            if (!simulando) { // no hubo victoria
                cambiarTurno();
                btnDisparar->setEnabled(true);
            }
        }
        return;
    }

    actual->proyectil->actualizar(DT, ANCHO, ALTO, rival->obstaculos);
}

void MainWindow::cambiarTurno() {
    turnoActual = (turnoActual == 1) ? 2 : 1;
    lblTurno->setText("Turno: " + ((turnoActual==1)?jugador1:jugador2)->getNombre());
    lblEstado->setText("Configura y dispara.");
    // Ajustar slider ángulo para que tenga sentido visual
    sliderAngulo->setValue(45);
}

void MainWindow::verificarVictoria() {
    if (jugador2->perdio()) {
        QMessageBox::information(this, "¡Fin del juego!",
            "🏆 ¡Jugador 1 ganó! Destruyó todos los obstáculos del rival.");
        btnDisparar->setEnabled(false);
        simulando = true; // bloquear más disparos
    } else if (jugador1->perdio()) {
        QMessageBox::information(this, "¡Fin del juego!",
            "🏆 ¡Jugador 2 ganó! Destruyó todos los obstáculos del rival.");
        btnDisparar->setEnabled(false);
        simulando = true;
    }
}
