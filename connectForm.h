/* Copyright 2016 Mikhail Wall.
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
 * This file was modified by Mikhail Wall and Konstantin Batoev to make it comply with the requirements of trikRuntime
 * project. See git revision history for detailed changes. */

#pragma once

#include <QtWidgets/QDialog>
#include <QtNetwork/QTcpSocket>
#include <QtCore/QSettings>


#include "connectionManager.h"
#include "ui_connectForm.h"

/// Dialog for creating connection between robot and application
class ConnectForm : public QDialog
{
	Q_OBJECT
	Q_DISABLE_COPY(ConnectForm)

public:
	/// Constructor.
	ConnectForm(ConnectionManager *connectionManager
				, QSettings *settings, QWidget *parent = nullptr);

	/// Constructor that gets the previous entered values or default values
	ConnectForm(ConnectionManager *manager, QWidget *parent);
	~ConnectForm() override = default;

private slots:

	/// Slot for button for connecting to robot
	void onConnectButtonClicked();

	/// Slot for letting user to input ports and ips by himself
	void onAdvancedButtonClicked();

	/// Slot for copying GamepadIp to CameraIp when Advanced button wasn't pressed
	void copyGamepadIpToCameraIp(const QString &text);

signals:
	/// Signal is emitted when user presses ConnectButton
	void newConnectionParameters();

private:
	void setVisibilityToAdditionalButtons(bool mode);

	/// Field with GUI automatically generated by connectForm.ui.
	QScopedPointer<Ui::ConnectForm> mUi;

	QSettings *mSettings; /// Does not have ownership

	/// ConnectionManager for saving state of internet connection
	ConnectionManager *connectionManager; /// Does not have ownership
};

