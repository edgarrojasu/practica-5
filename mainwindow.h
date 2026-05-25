#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsEllipseItem>
#include <QTimer>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDebug>
#include "particula.h"
#include "obstaculo.h"
#include <vector>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void onTick();
    void onChangeGravity();

private:
    QGraphicsScene* scene;
    QGraphicsView*  view;
    QTimer*         timer;
    QPushButton*    btnGravedad;

    std::vector<Particula*> particulas;
    std::vector<Obstaculo*> obstaculos;
    double g;

    static constexpr double DT    = 0.1;
    static constexpr double ANCHO = 950;
    static constexpr double ALTO  = 600;
};

#endif
