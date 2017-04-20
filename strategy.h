#pragma once

#include <QObject>
#include <QKeyEvent>
#include <QVector>
#include <QSharedPointer>

/// is used to get needed instance
enum Strategies {
	standartStrategy = 0
	, accelerateStrategy
};


class Strategy : public QObject
{
	Q_OBJECT

public:
	/// method that encapsulates logic for generating commands
	virtual void processEvent(QEvent *event) = 0;

	/// method that is used in GUI to get needed instance in run-time
	static Strategy *getStrategy(Strategies type);

signals:
	void commandPrepared(const QString &command);


private:
	static void createInstances();

	static QMap<Strategies, QSharedPointer<Strategy> > instances;
};


