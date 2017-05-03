#include "accelerateStrategy.h"

#include <math.h>

AccelerateStrategy::AccelerateStrategy()
	: pad1WasActive(false)
	, pad2WasActive(false)
	, speed(300)
	, padsMapper(new QSignalMapper(this))
{

	connect(&stopTimerForPad1, SIGNAL(timeout()), padsMapper, SLOT(map()));
	padsMapper->setMapping(&stopTimerForPad1, 1);
	connect(&stopTimerForPad1, SIGNAL(timeout()), &stopTimerForPad1, SLOT(stop()));

	connect(&stopTimerForPad2, SIGNAL(timeout()), padsMapper, SLOT(map()));
	padsMapper->setMapping(&stopTimerForPad2, 2);
	connect(&stopTimerForPad2, SIGNAL(timeout()), &stopTimerForPad2, SLOT(stop()));

	connect(padsMapper, SIGNAL(mapped(int)), this, SLOT(stopPads(int)));
	connect(&workTimer, SIGNAL(timeout()), this, SLOT(dealWithPads()));

	pad1 = {Qt::Key_W, Qt::Key_A, Qt::Key_S, Qt::Key_D};
	pad2 = {Qt::Key_Left, Qt::Key_Right, Qt::Key_Up, Qt::Key_Down};
	magicButtons = {Qt::Key_1, Qt::Key_2, Qt::Key_3, Qt::Key_4, Qt::Key_5};


	powers = {
		{X1, 0}
		, {Y1, 0}
		, {X2, 0}
		, {Y2, 0}
	};

	cntPowers = {
		{X1, 0}
		, {Y1, 0}
		, {X2, 0}
		, {Y2, 0}
	};

	indeces = {
		{Qt::Key_D, X1}
		, {Qt::Key_A, X1}
		, {Qt::Key_W, Y1}
		, {Qt::Key_S, Y1}
		, {Qt::Key_Right, X2}
		, {Qt::Key_Left, X2}
		, {Qt::Key_Up, Y2}
		, {Qt::Key_Down, Y2}
	};
	additions = {
		{Qt::Key_D, 10}
		, {Qt::Key_A, -10}
		, {Qt::Key_W, 10}
		, {Qt::Key_S, -10}
		, {Qt::Key_Right, 10}
		, {Qt::Key_Left, -10}
		, {Qt::Key_Up, 10}
		, {Qt::Key_Down, -10}
	};

}

AccelerateStrategy::AccelerateStrategy(int currentSpeed)
	: AccelerateStrategy()
{
	speed = currentSpeed;
}

void AccelerateStrategy::processEvent(QEvent *event)
{
	int eventType = event->type();
	if (eventType == QEvent::KeyPress || eventType == QEvent::KeyRelease) {
		QKeyEvent *keyEvent = dynamic_cast<QKeyEvent *> (event);
		auto key = static_cast<Qt::Key> (keyEvent->key());
		QSet<int> allKeys = pad1 + pad2 + magicButtons;
		if (allKeys.contains(key)) {
			if (!workTimer.isActive())
				workTimer.start(speed);

			if (magicButtons.contains(key))
				dealWithButtons(keyEvent);
			else {
				if (!keyEvent->isAutoRepeat()) {
					if (eventType == QEvent::KeyPress)
						mPressedKeys += key;
					else
						mPressedKeys -= key;
				}
			}
		}

	}
}

void AccelerateStrategy::setSpeed(int newSpeed)
{
	speed = newSpeed;
}

void AccelerateStrategy::stopPads(int padNumber)
{

	const QString padUp = "pad %1 up\n";
	switch (padNumber) {
	case 1:
		powers[X1] = powers[Y1] = 0;
		cntPowers[X1] = cntPowers[X1] = 0;
		pad1WasActive = false;
		emit commandPrepared(padUp.arg(1));
		break;
	case 2:
		powers[X2] = powers[Y2] = 0;
		cntPowers[X2] = cntPowers[Y2] = 0;
		pad2WasActive = false;
		emit commandPrepared(padUp.arg(2));
		break;
	default:
		break;
	}

	if (!pad1WasActive && !pad2WasActive)
		workTimer.stop();
}

void AccelerateStrategy::dealWithPads()
{

	if (mPressedKeys.size()) {

		// for pad1
		bool isSomeKeyFromPad1 = false;
		for (auto pad1Key : pad1) {
			if (mPressedKeys.contains(pad1Key)) {
				stopTimerForPad1.start(2 * speed + 100);
				isSomeKeyFromPad1 = true;
				pad1WasActive = true;
				Power index = indeces[pad1Key];
				powers[index] = std::max(-100, std::min(100, powers[index] + additions[pad1Key]));
				cntPowers[index] = 0;
			}
		}
		if (pad1WasActive) {
			checkPower(powers[X1], cntPowers[X1], QSet<Qt::Key> {Qt::Key_A, Qt::Key_D});
			checkPower(powers[Y1], cntPowers[Y1], QSet<Qt::Key> {Qt::Key_W, Qt::Key_S});
		}

		if (isSomeKeyFromPad1) {
			QString command = QString("pad 1 %1 %2 \n").arg(powers[X1]).arg(powers[Y1]);
			emit commandPrepared(command);
		}

		// for pad2
		bool isSomeKeyFromPad2 = false;
		for (auto pad2Key : pad2) {
			if (mPressedKeys.contains(pad2Key)) {
				stopTimerForPad2.start(2 * speed + 100);
				isSomeKeyFromPad2 = true;
				pad2WasActive = true;
				Power index = indeces[pad2Key];
				powers[index] = std::max(-100, std::min(100, powers[index] + additions[pad2Key]));
				cntPowers[index] = 0;
			}
		}
		if (pad2WasActive) {
			checkPower(powers[X2], cntPowers[X2], QSet<Qt::Key> {Qt::Key_Left, Qt::Key_Right});
			checkPower(powers[Y2], cntPowers[Y2], QSet<Qt::Key> {Qt::Key_Up, Qt::Key_Down});
		}

		if (isSomeKeyFromPad2) {
			QString command = QString("pad 2 %1 %2 \n").arg(powers[X2]).arg(powers[Y2]);
			emit commandPrepared(command);
		}
	}
}

void AccelerateStrategy::dealWithButtons(QKeyEvent *keyEvent)
{
	QMap<int, int> digits = {
		{Qt::Key_1, 1}
		, {Qt::Key_2, 2}
		, {Qt::Key_3, 3}
		, {Qt::Key_4, 4}
		, {Qt::Key_5, 5}
	};

	auto key = keyEvent->key();
	if (keyEvent->type() == QEvent::KeyPress) {
		QString command = QString("btn " + QString::number(digits[key]) + "\n");
		emit commandPrepared(command);
	}
}

void AccelerateStrategy::checkPower(int &power, int &cnt, QSet<Qt::Key> set)
{
	if (power) {
		bool contains = false;
		for (auto key : set) {
			contains = contains || mPressedKeys.contains(key);
		}
		if (!contains) {
			if (cnt) {
				cnt = 0;
				power = 0;
			} else {
				cnt++;
			}
		}
	}
}

