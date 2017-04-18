#pragma once

#include "istrategy.h"
#include <QTimer>

class AccelerateStrategy : public Strategy
{
	Q_OBJECT

public:
	AccelerateStrategy();

	void processEvent(QEvent *event);

private slots:
	void stopPads();

private:
	int powerX1;
	int powerY1;
	int powerX2;
	int powerY2;

	QTimer *timer;
	bool pad1WasActive;
	bool pad2WasActive;
	QSet<int> mPressedKeys;
	QMap<int, int> additions;
};

