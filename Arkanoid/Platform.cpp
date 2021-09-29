#include "Platform.h"
#include "Ball.h"


Platform::Platform(int startHeight, int leftBorder, int rightBorder, bool isStuck)
	:pixmap(width, height)
{
	this->leftBorder = leftBorder;
	this->rightBorder = rightBorder;
	this->isStuck = isStuck;
	dx = 0;
	keyToggleLeft = false;
	keyToggleRight = false;

	int x = leftBorder + (rightBorder - leftBorder - width) / 2;
	rect.setRect(x, startHeight, width, height);

	createPixmap();
}

void Platform::paint(QPainter& painter)
{
	painter.drawPixmap(rect, pixmap);
}

void Platform::setDx(Platform::statusKey stat)
{
	switch (stat)
	{
	case statusKey::UNTOGGLE_LEFT:
		keyToggleLeft = false;
		this->dx = keyToggleRight ? 1 : 0;
		break;
	case statusKey::TOGGLE_LEFT:
		this->dx = -1;
		keyToggleLeft = true;
		break;
	case statusKey::UNTOGGLE_ALL:
		this->dx = 0;
		keyToggleLeft = false;
		keyToggleRight = false;
		break;
	case statusKey::TOGGLE_RIGHT:
		this->dx = 1;
		keyToggleRight = true;
		break;
	case statusKey::UNTOGGLE_RIGHT:
		keyToggleRight = false;
		this->dx = keyToggleLeft ? -1 : 0;
		break;
	default:
		break;
	}

}

void Platform::move()
{
	int x = rect.x() + dx*speed;
	int y = rect.top();

	if (dx == -1) {
		if (x >= leftBorder) {
			rect.moveTo(x, y);
			if (ball != nullptr) {
				ball->setPoint(QPointF(ball->getRect().x() + (float)(dx * speed), ball->getRect().y()));
			}
		}
		else {
			qDebug() << "leftBorder";
		}
	}
	else if (dx == 1) {
		if (x + rect.width() <= rightBorder) {
			rect.moveTo(x, y);

			if (ball != nullptr) {
				ball->setPoint(QPointF(ball->getRect().x() + (float)(dx * speed), ball->getRect().y()));
			}
		}
		else {
			qDebug() << "rightBorder";
		}
	}
}

QRect& Platform::getRect()
{
	return rect;
}

void Platform::stuckBall(Ball* ball)
{
	if (this->ball == nullptr) {
		this->ball = ball;
		ball->setStuck(true);
		ball->setPoint(QPointF(ball->getRect().x(), rect.top() - Ball::getRadius() * 2.0 - 1));
		isStuck = false;
	}
}

void Platform::unstuckBall()
{
	if (this->ball != nullptr) {
		ball->setStuck(false);
		ball = nullptr;
	}
}

void Platform::setStuck(bool is)
{
	isStuck = is;
}

bool Platform::getStuck()
{
	return isStuck;
}

int Platform::getWidth()
{
	return rect.width();
}

void Platform::setWidth(int w)
{
	if (10 > w) return; // minimum
	if ((rightBorder - leftBorder) < w) return; // maximum
	rect.setWidth(w);
	
	if (rect.left() < leftBorder) {
		rect.moveTo(leftBorder, rect.y());
	}
	else if (rect.right() > rightBorder) {
		rect.moveTo(rightBorder - rect.width(), rect.y());
	}

	createPixmap();
}

void Platform::createPixmap()
{
	QRect rectPixmap(0, 0, rect.width(), rect.height());

	QLinearGradient grad(rectPixmap.topLeft(), rectPixmap.bottomLeft());
	grad.setColorAt(0.0, Qt::black);
	grad.setColorAt(0.1, Qt::blue);
	grad.setColorAt(0.3, Qt::white);
	grad.setColorAt(0.9, Qt::blue);
	grad.setColorAt(1, Qt::black);

	QPainter painter(&pixmap);
	painter.fillRect(rectPixmap, grad);
}
