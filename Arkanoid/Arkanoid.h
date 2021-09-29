#pragma once
#ifndef ARKANOID_H_
#define ARKANOID_H_

#include <QWidget>
#include <QPainter>
#include <QKeyEvent>
#include <QTimer>
#include <QList>
#include <QDebug>
#include <QVector2d>
#include <QPainterPath>

#include "Platform.h"
#include "Ball.h"
#include "Brick.h"
#include "DropBonus.h"
#include "DisposableBottom.h"

#define SIZE_WIDTH_SCREEN	500
#define SIZE_HEIGHT_SCREEN	500
#define DELAY 15

class Arkanoid: public QWidget
{
	int timerID;
	int countBreakingBrick;
	int score;

	Platform* platform;
	DisposableBottom* disposableBottom;
	QList<Ball*> listBalls;
	QList<Brick*> bricks;
	QList<DropBonus*> listBonus;

	enum GameStatus {
		GAME,
		LOSS,
		WIN
	}gameStatus;

public:
	Arkanoid(QWidget* parent = 0);
	~Arkanoid();

	void moveObject();
	void check—ollision();

	//Qt Events
	void paintEvent(QPaintEvent* event);
	void keyPressEvent(QKeyEvent* event);
	void keyReleaseEvent(QKeyEvent* event);
	void timerEvent(QTimerEvent* e);
	
	DropBonus* getRandBonus(QPoint pos);
	void printFinishLogo(QPainter& painter, QString message);
};

#endif // !ARKANOID_H_
