#include "mainwindow.h"
#include <QDebug>
#include <QMessageBox>
#include <time.h>
#include <stdlib.h>

MainWindow::MainWindow(QWidget *parent):  QWidget(parent)
{
    //allgemeine Werte
    srand(time(NULL));
    sizeX = 500;
    sizeY = 500;
    sizeBlock = 20;
    d = Direction::Start;

    setFixedSize(sizeX, sizeY);
    setStyleSheet("background-color: black");

    //initialisere Timer
    timer = new QTimer();
    //timer->start(300);
    connect(timer, &QTimer::timeout, this, &MainWindow::play);

    //initialisere pen
    QPen snakePen(Qt::blue, sizeBlock);
    QPen applePen(Qt::red, sizeBlock);

    //Erzeuge Schlange, die nach rechts läuft
    //int xHead = 240;//sizeX / 2;
    //int yHead = 240;//sizeY / 2;
    //qDebug() << QString::number(xHead) << ", " << QString::number(yHead);
    //snake.push_back(QPoint(xHead, yHead));
    //snake.push_back(QPoint(xHead-sizeBlock, yHead));

    start();
    //Erzeuge Apfel
    spawnApple();
}

MainWindow::~MainWindow()
{

}

void MainWindow::start() {
    d = Direction::Start;
    timer->start(200);
    //connect(timer, &QTimer::timeout, this, &MainWindow::play);
    int xHead = 240;//sizeX / 2;
    int yHead = 240;//sizeY / 2;
    qDebug() << QString::number(xHead) << ", " << QString::number(yHead);
    snake.clear();
    snake.push_back(QPoint(xHead, yHead));
    snake.push_back(QPoint(xHead-sizeBlock, yHead));

}

void MainWindow::play() {
    qDebug() << "play()";
    /*
     * Ablauf: Checke ob Apfel existiert
     *          nein:
     *              erzeuge Apfel, dann machen weiter
     *          ja: bewege Schlange
     *              checkHitWall()
     *              checkHitSnake()
     *          wenn nicht Wand/Snake getroffen: Checke, ob Apfel gegessen wurde
     *              nein: weitermachen
     *              ja: Setze Head auf Apfel Position, verlängere Schlange um 1
     */
    if(!(apple.x() >= 0 && apple.y() >= 0)) {
        spawnApple();
    }
    //bewege Schlange
    QPoint head = snake[0];
    switch(d) {
        case Direction::Down:
            snake.insert(snake.begin(), QPoint(head.x(), head.y()+sizeBlock));
            snake.pop_back();
            break;
        case Direction::Up:
            snake.insert(snake.begin(), QPoint(head.x(), head.y()-sizeBlock));
            snake.pop_back();
            break;
        case Direction::Left:
            snake.insert(snake.begin(), QPoint(head.x()-sizeBlock, head.y()));
            snake.pop_back();
            break;
        case Direction::Right:
            snake.insert(snake.begin(), QPoint(head.x()+sizeBlock, head.y()));
            snake.pop_back();
            break;
        default:
            break;
    }
    update();

    //Checke ob Wand oder Schlange getroffen
    if(checkHitSnake(snake[0]) || checkHitWall(snake[0])) {
        timer->stop();
        QMessageBox msgBox;
        msgBox.setText("Game Over!");
        msgBox.setInformativeText("Nochmal spielen?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);
        int ret = msgBox.exec();
        switch(ret) {
            case QMessageBox::Yes:
                start();
                break;
            case QMessageBox::No:
                msgBox.close();
        }
    }
    if(snake[0].x() == apple.x() && snake[0].y() == apple.y()) {
        snake.insert(snake.begin(), QPoint(apple.x(), apple.y()));
        apple.setX(-1);
        apple.setY(-1);
    }
}

void MainWindow::spawnApple() {
    int nRows = sizeY / sizeBlock;
    int nColumns = sizeX / sizeBlock;
    QPoint p;
    do {
        int x = rand() % nColumns * sizeBlock;
        p.setX(x);
        int y = rand() % nRows * sizeBlock;
        p.setY(y);
    } while(checkHitSnake(p));
    apple = p;
    qDebug() << "x-Position of Apple: " << QString::number(p.x()) << ", y-Position of Apple: " << QString::number(p.y());
}

void MainWindow::keyPressEvent(QKeyEvent *k) {
    if(!((k->key() == Qt::Key_Left && (d == Direction::Start)) ||
         (k->key() == Qt::Key_Left && (d == Direction::Right)) ||
         (k->key() == Qt::Key_Right && (d == Direction::Left)) ||
         (k->key() == Qt::Key_Down && (d == Direction::Up)) ||
         (k->key() == Qt::Key_Up && (d == Direction::Down)))) {
        switch(k->key()){
            case Qt::Key_Up:
                d = Direction::Up;
                //qDebug() << "Up";
                break;
            case Qt::Key_Down:
                d = Direction::Down;
                //qDebug() << "Down";
                break;
            case Qt::Key_Left:
                d = Direction::Left;
                //qDebug() << "Left";
                break;
            case Qt::Key_Right:
                d = Direction::Right;
                //qDebug() << "Right";
                break;
            default:
                break;
        }
    }
}

void MainWindow::paintEvent(QPaintEvent *p) {
    Q_UNUSED(p);
    QPainter painter(this);
    if(apple.x() > 0) {
        painter.setPen(applePen);
        painter.setBrush(QBrush(Qt::red, Qt::SolidPattern));
        painter.drawEllipse(apple.x(), apple.y(), sizeBlock, sizeBlock);
    }
    painter.setPen(snakePen);
    painter.setBrush(QBrush(Qt::blue, Qt::SolidPattern));
    for(size_t i = 0; i < snake.size(); i++) {
        if (i == 0) {
            //qDebug() << "First case, x is " << QString::number(snake[i].x()) << ", y is " << QString::number(snake[i].y());
            painter.drawEllipse(snake[i].x(), snake[i].y(), sizeBlock, sizeBlock);
        } else {
            //qDebug() << "Second case, x is " << QString::number(snake[i].x()) << ", y is " << QString::number(snake[i].y());
            painter.fillRect(snake[i].x(), snake[i].y(), sizeBlock, sizeBlock, QBrush(Qt::blue, Qt::SolidPattern));
        }
    }
}

bool MainWindow::checkHitSnake(QPoint head) {
    bool hit = false;
    for(size_t i = 1; i < snake.size(); i++) {
        if(head.x() == snake[i].x() && head.y() == snake[i].y()) hit = true;
    }
    return hit;
}

bool MainWindow::checkHitWall(QPoint head) {
    bool hit = false;
    if(head.x() < 0 || head.x() > sizeX) hit = true;
    if(head.y() < 0 || head.y() > sizeY) hit = true;
    return hit;
}
