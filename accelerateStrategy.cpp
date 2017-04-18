#include "accelerateStrategy.h"

#include <QDebug>

AccelerateStrategy::AccelerateStrategy()
	: powerX1(0)
	, powerY1(0)
	, powerX2(0)
	, powerY2(0)
	, timer(new QTimer(this))
	, pad1WasActive(false)
	, pad2WasActive(false)
{
	connect(timer, SIGNAL(timeout()), this, SLOT(stopPads()));
	additions = {
		{Qt::Key_W, 10}
		, {Qt::Key_S, -10}
		, {Qt::Key_D, 10}
		, {Qt::Key_A, -10}
		, {Qt::Key_Up, 10}
		, {Qt::Key_Down, -10}
		, {Qt::Key_Left, -10}
		, {Qt::Key_Right, 10}
	};
}

void AccelerateStrategy::processEvent(QEvent *event)
{
	int eventType = event->type();
	if (eventType == QEvent::KeyPress || eventType == QEvent::KeyRelease) {
		timer->start(500);
	}
	QSet<int> pad1 = {Qt::Key_W, Qt::Key_A, Qt::Key_S, Qt::Key_D};
	QSet<int> pad2 = {Qt::Key_Left, Qt::Key_Right, Qt::Key_Up, Qt::Key_Down};
	QSet<int> digit = {Qt::Key_1, Qt::Key_2, Qt::Key_3, Qt::Key_4, Qt::Key_5};

	if (eventType == QEvent::KeyPress) {
		auto key = (dynamic_cast<QKeyEvent *> (event))->key();

		if (pad1.contains(key)) {
			mPressedKeys += key;
			pad1WasActive = true;
			if (powerX1 && !mPressedKeys.contains(Qt::Key_D) && !mPressedKeys.contains(Qt::Key_A))
				powerX1 = 0;
			if (powerY1 && !mPressedKeys.contains(Qt::Key_W) && !mPressedKeys.contains(Qt::Key_S))
				powerY1 = 0;
			for (auto pressedKey : mPressedKeys) {
				switch (pressedKey) {
				case Qt::Key_W:
					powerY1 = powerY1 < 100 ? powerY1 + 10 : 100;
					break;
				case Qt::Key_S:
					powerY1 = powerY1 > -100 ? powerY1 - 10 : -100;
					break;
				case Qt::Key_D:
					powerX1 = powerX1 < 100 ? powerX1 + 10 : 100;
					break;
				case Qt::Key_A:
					powerX1 = powerX1 > -100 ? powerX1 - 10 : -100;
					break;
				}
			}
			// maybe timer2 for sending information
			QString command = QString("pad 1 %1 %2 \n").arg(powerX1).arg(powerY1);
			emit commandPrepared(command);
		}

		if (pad2.contains(key)) {
			mPressedKeys += key;
			pad2WasActive = true;
			if (powerX2 && !mPressedKeys.contains(Qt::Key_Right) && !mPressedKeys.contains(Qt::Key_Left))
				powerX2 = 0;
			if (powerY2 && !mPressedKeys.contains(Qt::Key_Up) && !mPressedKeys.contains(Qt::Key_Down))
				powerY2 = 0;
			for (auto pressedKey : mPressedKeys) {
				switch (pressedKey) {
				case Qt::Key_Up:
					powerY2 = powerY2 < 100 ? powerY2 + 10 : 100;
					break;
				case Qt::Key_Down:
					powerY2 = powerY2 > -100 ? powerY2 - 10 : -100;
					break;
				case Qt::Key_Right:
					powerX2 = powerX2 < 100 ? powerX2 + 10 : 100;
					break;
				case Qt::Key_Left:
					powerX2 = powerX2 > -100 ? powerX2 - 10 : -100;
					break;
				}
			}
			// maybe timer2 for sending information
			QString command = QString("pad 2 %1 %2 \n").arg(powerX2).arg(powerY2);
			emit commandPrepared(command);
		}



	} else if (eventType == QEvent::KeyRelease) {
		//int flag = 1;
		auto key = (dynamic_cast<QKeyEvent *> (event))->key();
		mPressedKeys -= key;
	}
}

void AccelerateStrategy::stopPads()
{
	timer->stop();
	const QString padup = "pad %1 up\n";
	if (pad1WasActive) {
		powerX1 = 0;
		powerY1 = 0;
		pad1WasActive = false;
		emit commandPrepared(padup.arg(1));
	}
	if (pad2WasActive) {
		powerX2 = 0;
		powerY2 = 0;
		pad2WasActive = false;
		emit commandPrepared(padup.arg(2));
	}
}

