#pragma once
#ifndef BRICK_H_
#define BRICK_H_

#include <QRect>
#include <QColor>
#include <QPoint>
#include <QPainter>
#include <QGradient>

class DropBonus;

enum type_brick {
	NO,		// Emptiness
	H1,		// 1 Health
	H2,		// 2 Health
	H3,		// 3 Health
	H4,		// 4 Health
	H5,		// 5 Health
	NB		// Non-breaking
};

class Brick
{
	QRect rect;
	QColor color;
	int health;
	DropBonus* bonus;
	QPixmap pixmap;

	static constexpr QColor ColorsBrick[]{
		QColor(0, 255, 0),		// Зелённый
		QColor(255, 255, 0),	// Жёлтый
		QColor(255, 0, 0),		// Красный
		QColor(0, 0, 255),		// Синий
		QColor(101, 6, 196),	// Фиолетовый
		QColor(127, 127, 127)	// Серый
	};

	type_brick type;

public:
	Brick(int x, int y, int width, int height, type_brick typeBrick = H1, DropBonus* bonus = nullptr);
	~Brick();
	virtual void hit();
	void paint(QPainter& painter);
	virtual int getHealth();
	QRect& getRect();
	DropBonus* getBonus();

	bool isBreakingBrick();

private:
	void createPixmap();

};

#endif // !BRICK_H_

