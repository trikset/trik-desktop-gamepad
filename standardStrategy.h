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
#include <QtGui/QKeyEvent>

/// class that generates commands from pad only with max/min values
class StandardStrategy : public Strategy
{
public:
	/// Create new object for this strategy
	explicit StandardStrategy(QObject *parent = nullptr);
	/// method that generates commands
	void processEvent(QEvent *event) final;
};

