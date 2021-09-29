#include "DisposableBottom.h"

DisposableBottom::DisposableBottom(int height, int leftBorder, int rightBorder, bool isActive)
{
	this->isActive = isActive;
	rect.setRect(leftBorder, height, rightBorder - leftBorder, 10);
	this->height = height;
}

bool DisposableBottom::getActive()
{
	return isActive;
}

void DisposableBottom::setActive(bool is)
{
	isActive = is;
}

void DisposableBottom::paint(QPainter& painter)
{
	painter.fillRect(rect, Qt::red);
}

int DisposableBottom::getHeight()
{
	return height;
}
