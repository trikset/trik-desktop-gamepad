/* Copyright 2017 Konstantin Batoev.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This file was modified by Konstantin Batoev to make it comply with the requirements of trikRuntime
 * project. See git revision history for detailed changes. */

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
	void reset();

	/// method that is used in GUI to get needed instance in run-time
	static Strategy *getStrategy(Strategies type);

signals:
	void commandPrepared(const QString &command);


private:
	static void createInstances();

	static QMap<Strategies, QSharedPointer<Strategy> > instances;

protected:
	QSet<int> mPressedKeys;
};


