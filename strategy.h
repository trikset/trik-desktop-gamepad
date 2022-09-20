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

#include <QtCore/QObject>
#include <QtGui/QKeyEvent>
#include <QtCore/QVector>

/// is used to get needed instance
enum class Strategies {
	standartStrategy = 0
	, accelerateStrategy
	, TOTAL
};


/// class that generates commands
class Strategy : public QObject
{
	Q_OBJECT

public:
	explicit Strategy(QObject *parent);

	/// method that encapsulates logic for generating commands
	virtual void processEvent(QEvent *event) = 0;
	/// method that do all keys not pressed
	void reset();

	/// method that is used in GUI to get needed instance in run-time
	static Strategy *getStrategy(Strategies type, QObject *parent);

signals:
	/// signal with generated command
	void commandPrepared(const QString &command);


protected:
	QSet<int> mPressedKeys;
};


