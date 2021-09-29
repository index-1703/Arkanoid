#include "Ball.h"
#include "Brick.h"
#include "Platform.h"
#include "DisposableBottom.h"

Ball::Ball(QPointF startPoint, QVector2D velocity, bool isStuck)
	:velocity(velocity), isStuck(isStuck), pixmap(radius * 2.0, radius * 2.0)
{
	speed = 2;
	rect.setRect(startPoint.x(), startPoint.y(), radius * 2.0, radius * 2.0);
	
	createPixmap();
}

void Ball::move()
{
	if (!isStuck) {
		float x = (float)rect.x() + velocity.x() * speed;
		float y = (float)rect.y() + velocity.y() * speed;

		rect.moveTo(x, y);
	}
}

void Ball::paint(QPainter& painter)
{
	//painter.setBackgroundMode(Qt::TransparentMode);
	painter.drawPixmap(rect.topLeft(),pixmap);
}

float Ball::getRadius()
{
	return radius;
}

double Ball::getXdir()
{
	return velocity.x();
}

double Ball::getYdir()
{
	return velocity.y();
}

void Ball::setAngle(double angle)
{
	velocity.setX(cos(angle * PI / 180.0));
	velocity.setY(-sin(angle * PI / 180.0));
}

void Ball::setPoint(QPointF point)
{
	rect.moveTo(point);
}

void Ball::setXdir(float x)
{
	velocity.setX(x);
}

QRectF& Ball::getRect()
{
	return rect;
}

void Ball::setYdir(float y)
{
	velocity.setY(y);
}

bool Ball::checkСollision(Ball* ball) {
	if (this == ball) return false;
	if (isStuck || ball->getStuck()) return false;

	QVector2D diff(rect.center() - ball->getRect().center());
	if (diff.length() < radius * 2.0) {
		velocity += diff.normalized();
		velocity.normalize();
		return true;
	}

	return false;
}

bool Ball::checkСollision(QRect borders)
{
	if (isStuck) return false;

	bool isСollision = false;

	// left border
	if (rect.left() <= 0 && velocity.x() < 0.0f) {
		velocity.setX(-velocity.x());
		setPoint(QPoint(0, rect.y()));
		isСollision = true;
	}

	// right border
	if (rect.right() >= borders.width() && velocity.x() > 0.0f) {
		velocity.setX(-velocity.x());
		setPoint(QPoint(borders.width() - radius * 2.0f, rect.y()));
		isСollision = true;
	}

	// top border
	if (rect.top() <= 0 && velocity.y() < 0.0) {
		velocity.setY(-velocity.y());
		setPoint(QPoint(rect.x(), 0));
		isСollision = true;
	}

	return isСollision;
}

inline float clamp(float value, float min, float max) {
	return std::max(min, std::min(max, value));
}

enum Direction {
	UP,
	RIGHT,
	DOWN,
	LEFT
}; 

Direction VectorDirection(QVector2D target)
{
	QVector2D compass[] = {
		QVector2D(0.0f, 1.0f),	// вверх
		QVector2D(1.0f, 0.0f),	// вправо
		QVector2D(0.0f, -1.0f), // вниз
		QVector2D(-1.0f, 0.0f)	// влево
	};
	float max = 0.0f;
	Direction best_match;
	
	for (unsigned int i = 0; i < 4; i++)
	{
		float dot_product = QVector2D::dotProduct(QVector2D(target).normalized(), compass[i]);
		if (dot_product > max)
		{
			max = dot_product;
			best_match = (Direction)i;
		}
	}
	return best_match;
}

bool Ball::checkСollision(Platform* platform)
{
	if (isStuck) return false;

	QPointF halfBrick(platform->getRect().width() / 2, platform->getRect().height() / 2); // Середина платформы
	QVector2D difference(rect.center() - platform->getRect().center()); // Разность между центром окружности и центром платформыж
	QVector2D clamped(clamp(difference.x(), -halfBrick.x(), halfBrick.x()), clamp(difference.y(), -halfBrick.y(), halfBrick.y()));
	QPointF closest = platform->getRect().center() + clamped.toPointF(); // Близжайшая точка на платформе к окружности

	difference = QVector2D(closest - rect.center()); // Вектор между центром окружности и близжайшей к ней точке на платформе

	if (difference.length() < radius) {
		velocity = QVector2D(rect.center() - platform->getRect().center()).normalized();
		qDebug() << "Ball+Platfotm";
		return true;
	}

	return false;
}

int Ball::getSpeed()
{
	return speed;
}

void Ball::setSpeed(int s)
{
	speed = s;
}

QVector2D Ball::getVelocity()
{
	return velocity;
}

void Ball::createPixmap()
{
	QRect rectPixmap(0, 0, radius * 2.0, radius * 2.0);

	QRadialGradient grad;
	grad.setCenter(rectPixmap.center());
	grad.setFocalPoint(rectPixmap.center() - QPointF(radius / 3.0, radius / 3.0));
	grad.setRadius(radius);
	grad.setColorAt(0.0, Qt::white);
	grad.setColorAt(0.7, Qt::blue);
	grad.setColorAt(1.0, Qt::black);

	QImage img(rectPixmap.size(), QImage::Format_ARGB32);
	
	QPainter painter;
	painter.begin(&img);
	painter.setCompositionMode(QPainter::CompositionMode_Source);
	painter.fillRect(rectPixmap, Qt::transparent);
	painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
	painter.setBrush(grad);
	painter.drawEllipse(rectPixmap);
	painter.end();

	pixmap = QPixmap::fromImage(img);
}

bool Ball::checkСollision(DisposableBottom* disposableBottom) {

	if (disposableBottom->getActive() && rect.bottom() > disposableBottom->getHeight()) {
		disposableBottom->setActive(false);
		velocity.setY(-velocity.y());
		return true;
	}

	return false;
}

bool Ball::checkСollision(Brick* brick)
{
	if (isStuck) return false;

	QPointF halfBrick(brick->getRect().width() / 2, brick->getRect().height() / 2); // Середина блока
	QVector2D difference(rect.center() - brick->getRect().center()); // Разность между центром окружности и центром блока
	QVector2D clamped(clamp(difference.x(), -halfBrick.x(), halfBrick.x()), clamp(difference.y(), -halfBrick.y(), halfBrick.y()));
	QPointF closest = brick->getRect().center() + clamped.toPointF(); // Близжайшая точка на блоке к окружности

	difference = QVector2D(closest - rect.center()); // Вектор между центром окружности и близжайшей к ней точке на блоке

	if (difference.length() < radius) {
		
		speed += 0.05;

		Direction dir = VectorDirection(difference);
		if (dir == LEFT || dir == RIGHT)
		{
			velocity.setX(-velocity.x());

			float penetration = radius - std::abs(difference.x());
			if (dir == LEFT)
				setPoint(QPoint(rect.x() + penetration, rect.y()));
			else
				setPoint(QPoint(rect.x() - penetration, rect.y()));
		}
		else 
		{
			velocity.setY(-velocity.y());
			
			float penetration = radius - std::abs(difference.y());
			if (dir == UP)
				setPoint(QPoint(rect.x(), rect.y() - penetration));
			else
				setPoint(QPoint(rect.x(), rect.y() + penetration));
		}

		return true;
	}

	return false;
}

bool Ball::getStuck()
{
	return isStuck;
}

void Ball::setStuck(bool isStuck)
{
	this->isStuck = isStuck;
}
