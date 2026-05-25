#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsEllipseItem>
#include <QTimer>
#include <QPushButton>
#include <QVBoxLayout>
#include "particula.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void onTick();
    void onChangeGravity();

private:
    QGraphicsScene*       scene;
    QGraphicsView*        view;
    QGraphicsEllipseItem* item;
    QTimer*               timer;
    QPushButton*          btnGravedad;

    Particula* particula;
    double g;

    static constexpr double DT     = 0.1;
    static constexpr double ANCHO  = 950;
    static constexpr double ALTO   = 600;
};

#endif // MAINWINDOW_H
