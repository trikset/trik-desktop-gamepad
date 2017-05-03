#include "strategy.h"

#include "standardStrategy.h"
#include "accelerateStrategy.h"

// defining static variable
QMap<Strategies, QSharedPointer<Strategy> > Strategy::instances;

void Strategy::reset()
{
	mPressedKeys.clear();
}

Strategy *Strategy::getStrategy(Strategies type)
{
	if (!instances.size()) {
		createInstances();
	}

	if (instances.keys().contains(type))
		return instances.value(type).data();
	else
		return nullptr;
}

void Strategy::createInstances()
{
	instances.insert(standartStrategy, QSharedPointer<Strategy>(new StandardStrategy));
	instances.insert(accelerateStrategy, QSharedPointer<Strategy>(new AccelerateStrategy));
}
