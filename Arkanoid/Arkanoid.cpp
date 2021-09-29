#include "Arkanoid.h"
#include <iostream>
#include <ctime>
#define LVL_MAP LvlMap

static type_brick LvlMap[5][7] = {
	{ H1, H1, H2, H3, H2, H1, H1},
	{ H1, H2, H3, H4, H3, H2, H1},
	{ H2, H3, H4, H5, H4, H3, H2},
	{ H1, H2, H3, H4, H3, H2, H1},
	{ H1, H1, H2, H3, H2, H1, H1}
};

static type_brick LvlEasy[1][1] = {
	{ H1}
};

static type_brick lvl1[7][7] = {
	{ NO, H1, H2, H3, H4, H5, NB},
	{ NO, H1, H2, H3, H4, H5, NB},
	{ NO, H1, H2, H3, H4, H5, NB},
	{ NO, H1, H2, H3, H4, H5, NB},
	{ NO, H1, H2, H3, H4, H5, NB},
	{ NO, H1, H2, H3, H4, H5, NB},
	{ NO, H1, H2, H3, H4, H5, NB}
};

static type_brick lvlGreen[9][8] = {
	{ H1, H1, H1, H1, H1, H1, H1, H1},
	{ H1, H1, H1, H1, H1, H1, H1, H1},
	{ H1, H1, H1, H1, H1, H1, H1, H1},
	{ H1, H1, H1, H1, H1, H1, H1, H1},
	{ H1, H1, H1, H1, H1, H1, H1, H1},
	{ H1, H1, H1, H1, H1, H1, H1, H1},
	{ H1, H1, H1, H1, H1, H1, H1, H1},
	{ H1, H1, H1, H1, H1, H1, H1, H1},
	{ H1, H1, H1, H1, H1, H1, H1, H1}
};

Arkanoid::Arkanoid(QWidget* parent)
	: QWidget(parent)
{
	setSizeIncrement(SIZE_WIDTH_SCREEN, SIZE_HEIGHT_SCREEN);
	setFixedSize(SIZE_WIDTH_SCREEN, SIZE_HEIGHT_SCREEN);
	setWindowTitle("Arkanoid");

	gameStatus = GAME;

	platform = new Platform(height() - 25, 0, width());
	disposableBottom = new DisposableBottom(height() - 10, 0, width());
	score = 0;

	int angle = rand() % 180;
	Ball* ball = new Ball(QPointF(float(platform->getRect().center().x()) - Ball::getRadius(), platform->getRect().top() - Ball::getRadius()*2.0),
		QVector2D(cos(angle * PI / 180.0), -sin(angle * PI / 180.0)));
	listBalls.append(ball);

	platform->stuckBall(ball);

	int maxCountH = sizeof(LVL_MAP)/sizeof(LVL_MAP[0]);
	int maxCountW = sizeof(LVL_MAP[0]) / sizeof(LVL_MAP[0][0]);
	int brickH = 30;
	int brickW = width() / maxCountW;
	int topMargin = 50;

	DropBonus* bonus;
	countBreakingBrick = 0;
	for (int i = 0; i < maxCountH; i++)
		for (int j = 0; j < maxCountW; j++) {
			if (LVL_MAP[i][j] == NO)
				continue;
			if (LVL_MAP[i][j] != NB) {
				countBreakingBrick++;
				bonus = getRandBonus(QPoint(j * brickW + brickW / 2, i * brickH + topMargin + brickH / 2));
			}
			else {
				bonus = NULL;
			}
			bricks.append(new Brick(j * brickW, i * brickH + topMargin, brickW, brickH, LVL_MAP[i][j], bonus));
		}

	timerID = startTimer(DELAY);
}

Arkanoid::~Arkanoid()
{
	delete platform;
	delete disposableBottom;

	for (Ball* ball : listBalls) {
		delete ball;
	}

	for (Brick* brick : bricks) {
		delete brick;
	}

	for (DropBonus* bonus : listBonus) {
		delete bonus;
	}
}

void Arkanoid::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	if (gameStatus == GAME) {

		painter.fillRect(rect(), Qt::black);

		if (disposableBottom->getActive()) {
			disposableBottom->paint(painter);
		}

		platform->paint(painter);

		for (Ball* ball : listBalls) {
			ball->paint(painter);
		}

		for (Brick* brick : bricks) {
			brick->paint(painter);
		}

		for (DropBonus* bonus : listBonus) {
			bonus->paint(painter);
		}

		QString str;
		str.setNum(score);
		QPainterPath path;
		path.addText(QPoint(0, 35), QFont("Consolas", 35, 0), str);
		painter.setBrush(Qt::white);
		painter.drawPath(path);
	}
	else if (gameStatus == WIN) {
		printFinishLogo(painter, "WIN");
	}
	else if (gameStatus == LOSS) {
		printFinishLogo(painter, "LOSS");
	}
}

void Arkanoid::keyPressEvent(QKeyEvent* event)
{
	switch (event->key())
	{
	case Qt::Key_Left:
		platform->setDx(Platform::statusKey::TOGGLE_LEFT);
		break;
	case Qt::Key_Right:
		platform->setDx(Platform::statusKey::TOGGLE_RIGHT);
		break;
	case Qt::Key_Escape:
		close();
		break;
	default:
		QWidget::keyPressEvent(event);
		break;
	}
}

void Arkanoid::keyReleaseEvent(QKeyEvent* event)
{
	switch (event->key())
	{
	case Qt::Key_Left:
		platform->setDx(Platform::statusKey::UNTOGGLE_LEFT);
		break;
	case Qt::Key_Right:
		platform->setDx(Platform::statusKey::UNTOGGLE_RIGHT);
		break;
	case Qt::Key_Space:
		platform->unstuckBall();
		break;
	default:
		QWidget::keyPressEvent(event);
		break;
	}
}

void Arkanoid::moveObject() {
	platform->move();

	for (Ball* ball : listBalls) {
		ball->move();
	}

	for (DropBonus* bonus : listBonus) {
		bonus->move();
	}
}

QPoint clamp(QPoint value, QPoint min, QPoint max) {
	return QPoint(std::max(min.x(), std::min(max.x(), value.x())), std::max(min.y(), std::min(max.y(), value.y())));
}

void Arkanoid::check—ollision()
{
	QMutableListIterator<Ball*> itrBall(listBalls);
	
	while (itrBall.hasNext())
	{
		Ball* ball = itrBall.next();

		if (ball->getStuck()) 
			continue;

		if (ball->getRect().top() > height()) {
			itrBall.remove();
			//qDebug() << "remove Ball";

			continue;
		}

		//Ball and Borders	
		ball->check—ollision(rect());

		//Ball and Disposable Bottom	
		ball->check—ollision(disposableBottom);

		// Ball and Platform
		if (ball->check—ollision(platform) && platform->getStuck()) {
			platform->stuckBall(ball);
		}

		// Ball and Bricks
		QMutableListIterator<Brick*> brick(bricks);
		while (brick.hasNext()) {
			if (ball->check—ollision(brick.next()) && brick.value()->isBreakingBrick()) {
				brick.value()->hit();
				score++;
				if (brick.value()->getHealth() <= 0) {
					DropBonus* bonus = brick.value()->getBonus();
					if (bonus != NULL) {
						listBonus.append(bonus);
					}
					brick.remove();
				}
			}
		}

		for (Ball* ball2 : listBalls) {
			ball->check—ollision(ball2);
		}
	}

	QMutableListIterator<DropBonus*> itrBonus(listBonus);
	while (itrBonus.hasNext())
	{
		if (itrBonus.next()->getRect().top() > height()) {
			itrBonus.remove();
		}
		else if (itrBonus.value()->check—ollision(platform)) {
			itrBonus.value()->activate();
			itrBonus.remove();
		}
	}

	if (bricks.size() == 0) {
		gameStatus = WIN;
	}
	else if (listBalls.size() == 0) {
		gameStatus = LOSS;
	}
}

void Arkanoid::timerEvent(QTimerEvent* e)
{
	moveObject();
	check—ollision();
	repaint();
	if (gameStatus != GAME) {
		killTimer(timerID);
	}
}

DropBonus* Arkanoid::getRandBonus(QPoint pos)
{
	int randBonus = rand() % 8;
	//randBonus = 7;
	DropBonus* bonus;
	switch (randBonus)
	{
	case 0:
		bonus = NULL;
		break;
	case 1:
		bonus = new ReducingPlatfomBonus(platform, pos);
		break;
	case 2:
		bonus = new IncreasingPlatfomBonus(platform, pos);
		break;
	case 3:
		bonus = new DecreaseSpeedBallsBonus(&listBalls, pos);
		break;
	case 4:
		bonus = new IncreaseSpeedBallsBonus(&listBalls, pos);
		break;
	case 5:
		bonus = new StuckBallBonus(platform, pos);
		break;
	case 6:
		bonus = new DisposableBottomBallBonus(disposableBottom, pos);
		break;
	case 7:
		bonus = new DoubleBallsBonus(&listBalls, pos);
		break;
	default:
		bonus = (DropBonus*)NULL;
		break;
	}

	return bonus;
}

void Arkanoid::printFinishLogo(QPainter& painter, QString message) {
	QFont font("Consolas", 35, 0);
	QFontMetrics fm(font);
	QPainterPath path;

	int textWidthMessage = fm.horizontalAdvance(message);
	path.addText(QPoint(-textWidthMessage / 2, 0), font, message);

	QString strScore;
	strScore.setNum(score);
	strScore = QString("Score: ") + strScore;
	int textWidthScore = fm.horizontalAdvance(strScore);
	path.addText(QPoint(-textWidthScore / 2, fm.height()), font, strScore);

	painter.fillRect(rect(), Qt::black);
	painter.translate(rect().center());
	painter.setBrush(Qt::white);
	painter.drawPath(path);
}