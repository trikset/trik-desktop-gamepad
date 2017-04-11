#include "standardStrategy.h"

StandardStrategy::StandardStrategy()
{

}

void StandardStrategy::processEvent(QEvent *event)
{
	int resultingPowerX1 = 0;
	int resultingPowerY1 = 0;
	int resultingPowerX2 = 0;
	int resultingPowerY2 = 0;

	if (event->type() == QKeyEvent::KeyPress) {
		auto key = (dynamic_cast<QKeyEvent *> (event))->key();
		mPressedKeys += key;
		// Handle W A S D buttons
		resultingPowerX1 = (mPressedKeys.contains(Qt::Key_D) ? 100 : 0) + (mPressedKeys.contains(Qt::Key_A) ? -100 : 0);
		resultingPowerY1 = (mPressedKeys.contains(Qt::Key_S) ? -100 : 0) + (mPressedKeys.contains(Qt::Key_W) ? 100 : 0);
		// Handle arrow buttons
		resultingPowerX2 = (mPressedKeys.contains(Qt::Key_Right) ? 100 : 0) + (mPressedKeys.contains(Qt::Key_Left) ? -100 : 0);
		resultingPowerY2 = (mPressedKeys.contains(Qt::Key_Down) ? -100 : 0) + (mPressedKeys.contains(Qt::Key_Up) ? 100 : 0);

		if (resultingPowerX1 != 0 || resultingPowerY1 != 0) {
			emit commandPrepared(QString("pad 1 %1 %2 \n").arg(resultingPowerX1).arg(resultingPowerY1));
		} else if (resultingPowerX2 != 0 || resultingPowerY2 != 0) {
			emit commandPrepared(QString("pad 2 %1 %2 \n").arg(resultingPowerX2).arg(resultingPowerY2));
		}

		// Handle 1 2 3 4 5 buttons
		QMap<int, int> digits = {
			{Qt::Key_1, 1}
			, {Qt::Key_2, 2}
			, {Qt::Key_3, 3}
			, {Qt::Key_4, 4}
			, {Qt::Key_5, 5}
		};

		for (auto key : digits.keys()) {
			if (mPressedKeys.contains(key)) {
				emit commandPrepared(QString("btn " + QString::number(digits[key]) + "\n"));
			}
		}

	} else if (event->type() == QKeyEvent::KeyRelease) {
		auto key = (dynamic_cast<QKeyEvent *> (event))->key();
		// Handle key release event
		QSet<int> pad1 = {Qt::Key_W, Qt::Key_A, Qt::Key_S, Qt::Key_D};
		QSet<int> pad2 = {Qt::Key_Left, Qt::Key_Right, Qt::Key_Up, Qt::Key_Down};
		mPressedKeys -= key;

		if (pad1.contains(key)) {
			emit commandPrepared(QString("pad 1 up\n"));
		} else if (pad2.contains(key)) {
			emit commandPrepared(QString("pad 2 up\n"));
		}
	}
}
