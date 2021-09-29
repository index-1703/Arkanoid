#include "Brick.h"
#include "DropBonus.h"

Brick::Brick(int x, int y, int width, int height, type_brick typeBrick, DropBonus* bonus)
	:pixmap(width,height)
{
	rect.setRect(x, y, width, height);
	type = typeBrick;
	this->bonus = bonus;

	health = (int)typeBrick;
	color = ColorsBrick[health - 1];

	createPixmap();
}

Brick::~Brick()
{
	delete bonus;
}

void Brick::hit()
{
	if (type == NB) return;

	health--;
	if (health > 0) {
		color = ColorsBrick[health - 1];
		createPixmap();
	}
}

void Brick::paint(QPainter& painter)
{
	painter.drawPixmap(rect, pixmap);
}

int Brick::getHealth()
{
	return health;
}

QRect& Brick::getRect()
{
	return rect;
}

DropBonus* Brick::getBonus()
{
	return bonus;
}

bool Brick::isBreakingBrick() {
	return type != NB;
}

void Brick::createPixmap()
{
	int indent = 5;

	QRect rectPixmap(0, 0, rect.width(), rect.height());
	QLinearGradient grad;

	grad.setStart(rectPixmap.topLeft());
	grad.setFinalStop(rectPixmap.bottomLeft());
	color.setAlpha(100);
	grad.setColorAt(0.0, color);
	color.setAlpha(255);
	grad.setColorAt(1, color);

	QPolygon polygonLeft({ rectPixmap.topLeft(), rectPixmap.topLeft() + QPoint(indent, indent),
						rectPixmap.bottomLeft() + QPoint(indent, -indent), rectPixmap.bottomLeft() });

	QPolygon polygonTop({ rectPixmap.topLeft(), rectPixmap.topLeft() + QPoint(indent, indent),
						rectPixmap.topRight() + QPoint(-indent, indent), rectPixmap.topRight() });

	QPolygon polygonRight({ rectPixmap.topRight(), rectPixmap.topRight() + QPoint(-indent, indent),
						rectPixmap.bottomRight() + QPoint(-indent, -indent), rectPixmap.bottomRight() });

	QPolygon polygonBottom({ rectPixmap.bottomRight(), rectPixmap.bottomRight() + QPoint(-indent, -indent),
						rectPixmap.bottomLeft() + QPoint(indent, -indent), rectPixmap.bottomLeft() });


	QPainter painter(&pixmap);

	painter.setBrush(QBrush(color));
	painter.drawPolygon(polygonLeft);
	painter.drawPolygon(polygonTop);

	color.setAlpha(100);
	painter.setBrush(QBrush(color));
	painter.drawPolygon(polygonRight);
	painter.drawPolygon(polygonBottom);

	painter.fillRect(rectPixmap.x() + indent, rectPixmap.y() + indent, rectPixmap.width() - indent * 2, rectPixmap.height() - indent * 2, grad);

	if (type == NB) {
		painter.setBrush(Qt::Dense6Pattern);
		painter.drawRoundedRect(rectPixmap, 5, 5);
	}

}
