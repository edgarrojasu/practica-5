#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QTimer>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <vector>
#include "jugador.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void onTick();
    void onDisparar();

private:
    // Escena
    QGraphicsScene* scene;
    QGraphicsView*  view;
    QTimer*         timer;

    // UI controles
    QSlider*   sliderAngulo;
    QSlider*   sliderVelocidad;
    QLabel*    lblAngulo;
    QLabel*    lblVelocidad;
    QLabel*    lblTurno;
    QLabel*    lblEstado;
    QPushButton* btnDisparar;

    // Juego
    Jugador* jugador1;
    Jugador* jugador2;
    int turnoActual;   // 1 o 2
    bool simulando;

    // Constantes
    static constexpr double DT       = 0.05;
    static constexpr double ANCHO    = 900;
    static constexpr double ALTO     = 500;
    static constexpr double G        = 9.81;
    static constexpr int    MAX_REB  = 5;
    static constexpr double MASA_P   = 2.0;
    static constexpr double RADIO_P  = 10.0;

    void setupUI();
    void setupEscena();
    void verificarVictoria();
    void cambiarTurno();
    QGraphicsTextItem* crearLabel(const QString& txt, double x, double y,
                                  int size = 12, QColor color = Qt::black);
};

#endif
