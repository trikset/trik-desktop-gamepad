#pragma once

#include "strategy.h"
#include <QTimer>
#include <QSignalMapper>

class AccelerateStrategy : public Strategy
{
	Q_OBJECT

public:
	AccelerateStrategy();
	AccelerateStrategy(int speed);

	/// slot for getting events from UI
	void processEvent(QEvent *event);

	void setSpeed(int newSpeed);

private slots:
	/// slot for stopping pads if they were active
	void stopPads(int padNumber);

	/// slot that analyzes mPressedKeys and generates pad-commands
	/// pads values are changed iteratively every period of time
	void dealWithPads();

	/// slot for Magic Buttons
	void dealWithButtons(QKeyEvent *keyEvent);

private:

	enum Power {
		X1 = 0
		, Y1
		, X2
		, Y2
	};

	/// checks if some key from set was pressed no longer than 1 tact
	void checkPower(int &power, int &cnt, QSet<Qt::Key> set);
	QMap<Power, int> powers;
	QMap<int, Power> indeces;
	QMap<int, int> additions;

	/// variables for setting 0 to PowerVariables if they were not pressed more than 1 tact
	QMap<Power, int> cntPowers;

	QTimer stopTimerForPad1;
	QTimer stopTimerForPad2;
	bool pad1WasActive;
	bool pad2WasActive;

	QSet<int> pad1;
	QSet<int> pad2;
	QSet<int> magicButtons;
	QTimer workTimer;

	/// defines period of time to check dealWithPads
	int speed;
	QSignalMapper *padsMapper;
};

