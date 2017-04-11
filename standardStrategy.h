#pragma once

#include "istrategy.h"

#include <QKeyEvent>

class StandardStrategy : public Strategy
{
public:
	StandardStrategy();
	void processEvent(QEvent *event);

private:
	/// Set for saving pressed keys
	QSet<int> mPressedKeys;
};

