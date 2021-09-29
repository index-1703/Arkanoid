#pragma once
#ifndef BALL_H_
#define BALL_H_

#include <QPoint>
#include <QPainter>
#include <QRect>
#include <cmath>
#include <QDebug>
#include <QVector2D>
#include <QPixmap>

#define PI 3.14159265 

class Platform;
class Brick;
class DisposableBottom;

class Ball
{
	QRectF rect;
	static constexpr float radius = 10.0f;

	QVector2D velocity;
	float speed;
	bool isStuck;

	QPixmap pixmap;

public:
	Ball(QPointF startPoint, QVector2D velocity, bool isStuck = false);

	void move();
	void paint(QPainter& painter);
	static float getRadius();
	double getXdir();
	double getYdir();
	QRectF& getRect();
	void setAngle(double angle);
	void setPoint(QPointF point);
	void setXdir(float x);
	void setYdir(float y);

	bool check—ollision(Ball* ball);
	bool check—ollision(QRect borders);
	bool check—ollision(Platform* platform);
	bool check—ollision(DisposableBottom* disposableBottom);
	bool check—ollision(Brick* brick);

	bool getStuck();
	void setStuck(bool isStuck);

	int getSpeed();
	void setSpeed(int s);

	QVector2D getVelocity();
private:
	void createPixmap();
};
#endif // !BALL_H_