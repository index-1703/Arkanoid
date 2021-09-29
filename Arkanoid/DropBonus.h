#pragma once
#include <QRect>
#include <QColor>
#include <QList>
#include <QPainter>

class Platform;
class Ball;
class DisposableBottom;

class DropBonus
{
	static constexpr int width = 40;
	static constexpr int height = 40;
	static constexpr int speed = 1;

protected:
	QRect rect;
	QPixmap pixmap;

public:
	DropBonus(QPoint pos);
	void move();
	bool checkСollision(Platform* platform);
	QRect getRect();
	virtual void activate() = 0;
	void paint(QPainter& painter);
};

// Уменьшение платформы
class ReducingPlatfomBonus 
	:public DropBonus
{
	Platform* platform;
public:
	ReducingPlatfomBonus(Platform* platform, QPoint pos);
	void activate();
};

// Увеличение платформы
class IncreasingPlatfomBonus 
	:public DropBonus
{
	Platform* platform;
public:
	IncreasingPlatfomBonus(Platform* platform, QPoint pos);
	void activate();
};

// Увеличение скорость шаров
class DecreaseSpeedBallsBonus 
	:public DropBonus
{
	QList<Ball*>* listBalls;
public:
	DecreaseSpeedBallsBonus(QList<Ball*> *listBalls, QPoint pos);
	void activate();
};

// Уменьшает скорость шаров
class IncreaseSpeedBallsBonus 
	:public DropBonus
{
	QList<Ball*>* listBalls;
public:
	IncreaseSpeedBallsBonus(QList<Ball*> *listBalls, QPoint pos);
	void activate();
};

// Прилипание шаров к платформе
class StuckBallBonus 
	:public DropBonus
{
	Platform* platform;
public:
	StuckBallBonus(Platform* platform, QPoint pos);
	void activate();
};

// Одноразовое дно для шарика
class DisposableBottomBallBonus 
	:public DropBonus
{
	DisposableBottom* disposableBottom;
public:
	DisposableBottomBallBonus(DisposableBottom* disposableBottom, QPoint pos);
	void activate();
};

// Удваивает количество шаров
class DoubleBallsBonus 
	:public DropBonus
{
	QList<Ball*> *listBalls;
public:
	DoubleBallsBonus(QList<Ball*> *listBalls, QPoint pos);
	void activate();
};