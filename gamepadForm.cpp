/* Copyright 2015-2016 CyberTech Labs Ltd.
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
 * This file was modified by Yurii Litvinov to make it comply with the requirements of trikRuntime
 * project. See git revision history for detailed changes. */

#include "gamepadForm.h"

#include "ui_gamepadForm.h"

#include <QtWidgets/QMessageBox>

GamepadForm::GamepadForm()
	: QWidget()
	, mUi(new Ui::GamepadForm())
{
	// Here all GUI widgets are created and initialized.
	mUi->setupUi(this);

	// Connecting "Connect" button to handler.
	connect(mUi->connectButton, &QPushButton::clicked, this, &GamepadForm::onConnectButtonClicked);

	// Disabling buttons since we are not connected to robot yet and can not send any commands.
	setButtonsEnabled(false);

	// Declaring lambda-function that calls "button pressed" handler with correct button id.
	const auto buttonClickedMapper = [this](int buttonId) {
		return [this, buttonId]() { onButtonPressed(buttonId); };
	};

	// Connecting buttons to handler using this function.
	connect(mUi->button1, &QPushButton::clicked, buttonClickedMapper(1));
	connect(mUi->button2, &QPushButton::clicked, buttonClickedMapper(2));
	connect(mUi->button3, &QPushButton::clicked, buttonClickedMapper(3));
	connect(mUi->button4, &QPushButton::clicked, buttonClickedMapper(4));
	connect(mUi->button5, &QPushButton::clicked, buttonClickedMapper(5));

	// Declaring lambda-function that calls "pad pressed" handler with correct command.
	const auto padsPressedMapper = [this](const QString &command) {
		return [this, command]() { onPadPressed(command); };
	};

	// Connecting pads to "pad pressed" handler.
	connect(mUi->buttonPad1Up, &QPushButton::pressed, padsPressedMapper("pad 1 0 100"));
	connect(mUi->buttonPad1Down, &QPushButton::pressed, padsPressedMapper("pad 1 0 -100"));
	connect(mUi->buttonPad1Left, &QPushButton::pressed, padsPressedMapper("pad 1 -100 0"));
	connect(mUi->ButtonPad1Right, &QPushButton::pressed, padsPressedMapper("pad 1 100 0"));

	connect(mUi->buttonPad2Up, &QPushButton::pressed, padsPressedMapper("pad 2 0 100"));
	connect(mUi->buttonPad2Down, &QPushButton::pressed, padsPressedMapper("pad 2 0 -100"));
	connect(mUi->buttonPad2Left, &QPushButton::pressed, padsPressedMapper("pad 2 -100 0"));
	connect(mUi->ButtonPad2Right, &QPushButton::pressed, padsPressedMapper("pad 2 100 0"));


	// Declaring lambda-function that calls "pad released" handler with correct pad id.
	const auto padsReleasedMapper = [this](int padId) {
		return [this, padId]() { onPadReleased(padId); };
	};

	// Connecting pads to "pad released" handler.
	connect(mUi->buttonPad1Up, &QPushButton::released, padsReleasedMapper(1));
	connect(mUi->buttonPad1Down, &QPushButton::released, padsReleasedMapper(1));
	connect(mUi->buttonPad1Left, &QPushButton::released, padsReleasedMapper(1));
	connect(mUi->ButtonPad1Right, &QPushButton::released, padsReleasedMapper(1));

	connect(mUi->buttonPad2Up, &QPushButton::released, padsReleasedMapper(2));
	connect(mUi->buttonPad2Down, &QPushButton::released, padsReleasedMapper(2));
	connect(mUi->buttonPad2Left, &QPushButton::released, padsReleasedMapper(2));
	connect(mUi->ButtonPad2Right, &QPushButton::released, padsReleasedMapper(2));
}

GamepadForm::~GamepadForm()
{
	// Gracefully disconnecting from host.
	mSocket.disconnectFromHost();
	// Here we do not care for success or failure of operation since we will close anyway.
	mSocket.waitForDisconnected(3000);
}

void GamepadForm::onConnectButtonClicked()
{
	// Getting IP address of a robot.
	const auto ip = mUi->robotIpLineEdit->text();

	// Disable "connect" button and address input field to indicate that we are trying to connect.
	mUi->connectButton->setEnabled(false);
	mUi->robotIpLineEdit->setEnabled(false);

	// Allow form to redraw disabled buttons.
	QApplication::processEvents();

	// Connecting. 4444 is hardcoded here since it is default gamepad port on TRIK.
	mSocket.connectToHost(ip, 4444);

	// Waiting for opened connection and checking that connection is actually established.
	if (!mSocket.waitForConnected(3000)) {
		// If not, warn user.
		QMessageBox::warning(this, "Connection failed", "Failed to connect to robot");
	} else {
		// Ok, connection is established, now we can enable all buttons.
		setButtonsEnabled(true);
	}

	// In any case, reenable address input field and "connect" button.
	mUi->connectButton->setEnabled(true);
	mUi->robotIpLineEdit->setEnabled(true);
}

void GamepadForm::setButtonsEnabled(bool enabled)
{
	// Here we enable or disable pads and "magic buttons" depending on given parameter.
	mUi->button1->setEnabled(enabled);
	mUi->button2->setEnabled(enabled);
	mUi->button3->setEnabled(enabled);
	mUi->button4->setEnabled(enabled);
	mUi->button5->setEnabled(enabled);

	mUi->buttonPad1Left->setEnabled(enabled);
	mUi->ButtonPad1Right->setEnabled(enabled);
	mUi->buttonPad1Up->setEnabled(enabled);
	mUi->buttonPad1Down->setEnabled(enabled);

	mUi->buttonPad2Left->setEnabled(enabled);
	mUi->ButtonPad2Right->setEnabled(enabled);
	mUi->buttonPad2Up->setEnabled(enabled);
	mUi->buttonPad2Down->setEnabled(enabled);
}

void GamepadForm::onButtonPressed(int buttonId)
{
	// Checking that we are still connected, just in case.
	if (mSocket.state() != QTcpSocket::ConnectedState) {
		return;
	}

	// Sending "btn <buttonId>" command to robot.
	if (mSocket.write(QString("btn %1\n").arg(buttonId).toLatin1()) == -1) {
		// If sending failed for some reason, we think that we lost connection and disable buttons.
		setButtonsEnabled(false);
	}
}

void GamepadForm::onPadPressed(const QString &action)
{
	// Here we send "pad <padId> <x> <y>" command.
	if (mSocket.state() != QTcpSocket::ConnectedState) {
		return;
	}

	if (mSocket.write((action + "\n").toLatin1()) == -1) {
		setButtonsEnabled(false);
	}
}

void GamepadForm::onPadReleased(int padId)
{
	// Here we send "pad <padId> up" command.
	if (mSocket.state() != QTcpSocket::ConnectedState) {
		return;
	}

	if (mSocket.write(QString("pad %1 up\n").arg(padId).toLatin1()) == -1) {
		setButtonsEnabled(false);
	}
}
