#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include <QtGui>
#include <QtCore>

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void start();
    void play();
    void spawnApple();
    void keyPressEvent(QKeyEvent *k);
    void paintEvent(QPaintEvent *p);
    bool checkHitSnake(QPoint pt);
    bool checkHitWall(QPoint pt);

    QTimer *timer;
    std::vector<QPoint> snake;
    QPoint apple;
    QPen snakePen;
    QPen applePen;
    enum class Direction {Up, Down, Left, Right, Start};
    Direction d;

    int sizeX;
    int sizeY;
    int sizeBlock;
};
#endif // MAINWINDOW_H
