#pragma once

#include <QObject>
#include <QKeyEvent>

class Strategy : public QObject
{
	Q_OBJECT

public:
	virtual void processEvent(QEvent *event) = 0;

signals:
	void commandPrepared(const QString &command);
};


