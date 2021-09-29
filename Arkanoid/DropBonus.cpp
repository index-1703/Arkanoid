#include "DropBonus.h"
#include "Platform.h"
#include "Ball.h"
#include "DisposableBottom.h"
#include <QPainterPath>

void createBackgroundPixmap(QPixmap& pixmap);

DropBonus::DropBonus(QPoint pos)
	:pixmap(width,height)
{
	createBackgroundPixmap(pixmap);
	rect.setRect(pos.x() - width / 2, pos.y() - height / 2, width, height);
}

void DropBonus::move()
{
	rect.moveTo(rect.x(), rect.y() + speed);
}

bool DropBonus::checkÑollision(Platform* platform)
{
	if (rect.intersects(platform->getRect())) 
		return true;
	return false;
}

QRect DropBonus::getRect()
{
	return rect;
}

void DropBonus::paint(QPainter& painter)
{
	painter.drawPixmap(rect.topLeft(), pixmap);
}

void createBackgroundPixmap(QPixmap& pixmap) {

	int indent = 4;

	QRect rectPixmap(QPoint(0,0), pixmap.size());
	QLinearGradient grad;

	grad.setStart(rectPixmap.topLeft());
	grad.setFinalStop(rectPixmap.bottomLeft());
	grad.setColorAt(0.0, Qt::gray);
	grad.setColorAt(1, Qt::white);

	QPolygon polygonLeft({ rectPixmap.topLeft(), rectPixmap.topLeft() + QPoint(indent, indent),
						rectPixmap.bottomLeft() + QPoint(indent, -indent), rectPixmap.bottomLeft() });

	QPolygon polygonTop({ rectPixmap.topLeft(), rectPixmap.topLeft() + QPoint(indent, indent),
						rectPixmap.topRight() + QPoint(-indent, indent), rectPixmap.topRight() });

	QPolygon polygonRight({ rectPixmap.topRight(), rectPixmap.topRight() + QPoint(-indent, indent),
						rectPixmap.bottomRight() + QPoint(-indent, -indent), rectPixmap.bottomRight() });

	QPolygon polygonBottom({ rectPixmap.bottomRight(), rectPixmap.bottomRight() + QPoint(-indent, -indent),
						rectPixmap.bottomLeft() + QPoint(indent, -indent), rectPixmap.bottomLeft() });


	QPainter painter(&pixmap);

	painter.setBrush(QBrush(Qt::white));
	painter.drawPolygon(polygonLeft);
	painter.drawPolygon(polygonTop);

	painter.setBrush(QBrush(Qt::gray));
	painter.drawPolygon(polygonRight);
	painter.drawPolygon(polygonBottom);

	painter.fillRect(rectPixmap.x() + indent, rectPixmap.y() + indent, rectPixmap.width() - indent * 2, rectPixmap.height() - indent * 2, grad);

}

ReducingPlatfomBonus::ReducingPlatfomBonus(Platform* platform, QPoint pos)
	:DropBonus(pos)
{
	this->platform = platform;

	int w = pixmap.size().width() / 4;
	int h = pixmap.size().height() / 4;
	QRect rectPlatformPath(pixmap.size().width() / 2 - w / 2, pixmap.size().height() / 2 - h / 2, w, h);

	QLinearGradient grad(rectPlatformPath.topLeft(), rectPlatformPath.bottomLeft());
	grad.setColorAt(0.0, Qt::black);
	grad.setColorAt(0.1, Qt::blue);
	grad.setColorAt(0.3, Qt::white);
	grad.setColorAt(0.9, Qt::blue);
	grad.setColorAt(1, Qt::black);

	QPainter painter(&pixmap);
	painter.fillRect(rectPlatformPath, grad);
}

void ReducingPlatfomBonus::activate()
{
	platform->setWidth(platform->getWidth()/2);
}

IncreasingPlatfomBonus::IncreasingPlatfomBonus(Platform* platform, QPoint pos)
	:DropBonus(pos)
{
	this->platform = platform;

	int w = pixmap.size().width() / 1.5;
	int h = pixmap.size().height() / 4;
	QRect rectPlatformPath(pixmap.size().width() / 2 - w / 2, pixmap.size().height() / 2 - h / 2, w, h);

	QLinearGradient grad(rectPlatformPath.topLeft(), rectPlatformPath.bottomLeft());
	grad.setColorAt(0.0, Qt::black);
	grad.setColorAt(0.1, Qt::blue);
	grad.setColorAt(0.3, Qt::white);
	grad.setColorAt(0.9, Qt::blue);
	grad.setColorAt(1, Qt::black);

	QPainter painter(&pixmap);
	painter.fillRect(rectPlatformPath, grad);
}

void IncreasingPlatfomBonus::activate()
{
	platform->setWidth(platform->getWidth() * 2);
}

DecreaseSpeedBallsBonus::DecreaseSpeedBallsBonus(QList<Ball*> *listBalls, QPoint pos)
	:DropBonus(pos)
{
	this->listBalls = listBalls;

	int radius = rect.width() / 5;

	QRect rectPixmapBall(0, 0, radius * 2.0, radius * 2.0);

	QRadialGradient grad;
	grad.setCenter(rectPixmapBall.center());
	grad.setFocalPoint(rectPixmapBall.center() - QPointF(radius / 3.0, radius / 3.0));
	grad.setRadius(radius);
	grad.setColorAt(0.0, Qt::white);
	grad.setColorAt(0.7, Qt::blue);
	grad.setColorAt(1.0, Qt::black);

	QImage img(rectPixmapBall.size(), QImage::Format_ARGB32);

	QPainter painter;
	painter.begin(&img);
	painter.setCompositionMode(QPainter::CompositionMode_Source);
	painter.fillRect(rectPixmapBall, Qt::transparent);
	painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
	painter.setBrush(grad);
	painter.drawEllipse(rectPixmapBall);
	painter.end();

	QPoint pointCenter(rect.width() / 2 - radius, rect.height() / 2 - radius);
	painter.begin(&pixmap);
	painter.setOpacity(0.2);
	painter.drawImage(pointCenter - QPoint(2,-1)*3, img);
	painter.setOpacity(0.6);
	painter.drawImage(pointCenter, img);
	painter.setOpacity(1.0);
	painter.drawImage(pointCenter + QPoint(2,-1)*3, img);
	painter.end();
}

void DecreaseSpeedBallsBonus::activate()
{
	for (Ball* ball : *listBalls) {
		ball->setSpeed(ball->getSpeed() * 2);
	}
}

IncreaseSpeedBallsBonus::IncreaseSpeedBallsBonus(QList<Ball*> *listBalls, QPoint pos)
	:DropBonus(pos)
{
	this->listBalls = listBalls;

	int radius = rect.width() / 5;

	QRect rectPixmapBall(0, 0, radius * 2.0, radius * 2.0);

	QRadialGradient grad;
	grad.setCenter(rectPixmapBall.center());
	grad.setFocalPoint(rectPixmapBall.center() - QPointF(radius / 3.0, radius / 3.0));
	grad.setRadius(radius);
	grad.setColorAt(0.0, Qt::white);
	grad.setColorAt(0.7, Qt::blue);
	grad.setColorAt(1.0, Qt::black);

	QImage img(rectPixmapBall.size(), QImage::Format_ARGB32);

	QPainter painter;
	painter.begin(&img);
	painter.setCompositionMode(QPainter::CompositionMode_Source);
	painter.fillRect(rectPixmapBall, Qt::transparent);
	painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
	painter.setBrush(grad);
	painter.drawEllipse(rectPixmapBall);
	painter.end();

	QPoint pointCenter(rect.width() / 2 - radius, rect.height() / 2 - radius);
	painter.begin(&pixmap);
	painter.drawImage(pointCenter, img);
	painter.end();
}

void IncreaseSpeedBallsBonus::activate()
{
	for (Ball* ball : *listBalls) {
		ball->setSpeed(ball->getSpeed() / 2);
	}
}

StuckBallBonus::StuckBallBonus(Platform* platform, QPoint pos)
	:DropBonus(pos)
{
	this->platform = platform;

	int w = pixmap.size().width() / 2;
	int h = pixmap.size().height() / 4;
	QRect rectPlatformPath(pixmap.size().width() / 2 - w / 2, pixmap.size().height() / 2 - h / 2, w, h);

	QLinearGradient grad(rectPlatformPath.topLeft(), rectPlatformPath.bottomLeft());
	grad.setColorAt(0.0, Qt::black);
	grad.setColorAt(0.1, Qt::green);
	grad.setColorAt(0.3, Qt::white);
	grad.setColorAt(0.9, Qt::green);
	grad.setColorAt(1, Qt::black);

	QPainter painter(&pixmap);
	painter.fillRect(rectPlatformPath, grad);
}

void StuckBallBonus::activate()
{
	platform->setStuck(true);
}

DisposableBottomBallBonus::DisposableBottomBallBonus(DisposableBottom* disposableBottom, QPoint pos)
	:DropBonus(pos)
{
	this->disposableBottom = disposableBottom;

	QPainterPath path;
	path.addText(pixmap.rect().center() + QPoint(-13, 5), QFont("Consolas", 10, 0), "SAVE");
	QPainter painter(&pixmap);
	painter.setBrush(Qt::black);
	painter.drawPath(path);
}

void DisposableBottomBallBonus::activate()
{
	disposableBottom->setActive(true);
}

DoubleBallsBonus::DoubleBallsBonus(QList<Ball*> *listBalls, QPoint pos)
	:DropBonus(pos)
{
	this->listBalls = listBalls;

	int radius = rect.width() / 6;

	QRect rectPixmapBall(0, 0, radius * 2.0, radius * 2.0);

	QRadialGradient grad;
	grad.setCenter(rectPixmapBall.center());
	grad.setFocalPoint(rectPixmapBall.center() - QPointF(radius / 3.0, radius / 3.0));
	grad.setRadius(radius);
	grad.setColorAt(0.0, Qt::white);
	grad.setColorAt(0.7, Qt::blue);
	grad.setColorAt(1.0, Qt::black);

	QImage img(rectPixmapBall.size(), QImage::Format_ARGB32);

	QPainter painter;
	painter.begin(&img);
	painter.setCompositionMode(QPainter::CompositionMode_Source);
	painter.fillRect(rectPixmapBall, Qt::transparent);
	painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
	painter.setBrush(grad);
	painter.drawEllipse(rectPixmapBall);
	painter.end();

	QPoint pointCenter(rect.width() / 2 - radius, rect.height() / 2 - radius);
	painter.begin(&pixmap);
	painter.drawImage(pointCenter - QPoint(rect.width() / 5, 0), img);
	painter.drawImage(pointCenter + QPoint(rect.width() / 5, 0), img);
	painter.end();
}

void DoubleBallsBonus::activate()
{
	QList<Ball*> newListBalls;
	for (Ball* ball : *listBalls) {
		Ball* newBall = new Ball(ball->getRect().topLeft(), ball->getVelocity());
		newListBalls.append(newBall);
	}

	listBalls->append(newListBalls);
}