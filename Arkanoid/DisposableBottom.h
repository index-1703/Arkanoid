#pragma once
#include <QRect>
#include <QPainter>

class DisposableBottom
{
	bool isActive;
	QRect rect;
	int height;

public:
	DisposableBottom(int height, int leftBorder, int rightBorder, bool isActive = false);
	bool getActive();
	void setActive(bool is);
	void paint(QPainter& painter);
	int getHeight();
};

