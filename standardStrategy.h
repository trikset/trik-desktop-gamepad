#pragma once

#include "strategy.h"
#include <QKeyEvent>

class StandardStrategy : public Strategy
{
public:
	StandardStrategy();
	void processEvent(QEvent *event);
};

