#pragma once
#ifndef PLATFORM_H_
#define PLATFORM_H_

#include <QRect>
#include <QColor>
#include <QPainter>
#include <QDebug>
#include <QList>

class Ball;

class Platform
{
    int leftBorder, rightBorder;
    int dx;
    int speed = 5;
    int height = 20;
    int width = 100;

    bool keyToggleLeft;
    bool keyToggleRight;
    bool isStuck;
    Ball* ball = nullptr;
    QRect rect;
    QPixmap pixmap;

public:
    enum class statusKey
    {
        UNTOGGLE_LEFT = -2,
        TOGGLE_LEFT,
        UNTOGGLE_ALL,
        TOGGLE_RIGHT,
        UNTOGGLE_RIGHT
    };

    Platform(int startHeight, int leftBorder, int rightBorder, bool isStuck = false);

    void paint(QPainter& painter);
    void setDx(statusKey stat);
    void move();
    QRect& getRect();

    void setStuck(bool is);
    bool getStuck();
    void stuckBall(Ball* ball);
    void unstuckBall();

    int getWidth();
    void setWidth(int w);

private:
    void createPixmap();
};

#endif // !PLATFORM_H_
