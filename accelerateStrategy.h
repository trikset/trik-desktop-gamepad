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

#include "strategy.h"

#include <QtCore/QTimer>
#include <QtCore/QSignalMapper>

/// class that generates commands from pad with various values depending on time a key is pressed
class AccelerateStrategy : public Strategy
{
	Q_OBJECT
	Q_DISABLE_COPY(AccelerateStrategy)

public:
	/// set initial period of time to check dealWithPads
	explicit AccelerateStrategy(int speed = 300);

	/// slot for getting events from UI
	void processEvent(QEvent *event);
	/// set period of time to check dealWithPads
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
	QMap<int, Power> indices;
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
};

