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
    QGraphicsScene* scene;
    QGraphicsView*  view;
    QTimer*         timer;

    QSlider*     sliderAngulo;
    QSlider*     sliderVelocidad;
    QLabel*      lblAngulo;
    QLabel*      lblVelocidad;
    QLabel*      lblTurno;
    QLabel*      lblEstado;
    QPushButton* btnDisparar;

    Jugador* jugador1;
    Jugador* jugador2;
    int  turnoActual;
    bool simulando;

    static constexpr double DT      = 0.1;
    static constexpr double ANCHO   = 900.0;
    static constexpr double ALTO    = 500.0;
    static constexpr double G       = 9.81;
    static constexpr int    MAX_REB = 5;
    static constexpr double MASA_P  = 2.0;
    static constexpr double RADIO_P = 10.0;

    void setupUI();
    void setupEscena();
    void cambiarTurno();
    void verificarVictoria();

    QGraphicsTextItem* crearLabel(const QString& txt, double x, double y,
                                   int size = 12, QColor color = Qt::black);
    ObstaculoJuego* crearObstaculo(double x, double y, double w, double h,
                                    double e, double hp, QColor color);
};

#endif
