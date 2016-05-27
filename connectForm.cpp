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
 * This file was modified by Mikhail Wall to make it comply with the requirements of trikRuntime
 * project. See git revision history for detailed changes. */

#include "connectForm.h"
#include "ui_connectForm.h"

#include <QtWidgets/QMessageBox>

/// Form for connect dialog
ConnectForm::ConnectForm(QTcpSocket *mSocket, QWidget *parent) :
	QDialog(parent),
	mSocket_(mSocket),
	mUi(new Ui::ConnectForm)
{
	mUi->setupUi(this);
	mUi->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
}

ConnectForm::~ConnectForm()
{
	delete mUi;
}

///  Connect button click event
void ConnectForm::on_connectButton_clicked()
{
	const auto ip = mUi->robotIpLineEdit->text();

	// Disable "connect" button and address input field to indicate that we are trying to connect.
	// Allow form to redraw disabled buttons.
	QApplication::processEvents();

	// Connecting. 4444 is hardcoded here since it is default gamepad port on TRIK.
	mSocket_->connectToHost(ip, 4444);
	// Waiting for opened connection and checking that connection is actually established.
	if (!mSocket_->waitForConnected(3000)) {
		// If not, warn user.
		QMessageBox::warning(this, "Connection failed", "Failed to connect to robot");
	} else {
		mUi->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
	}
}

/// Ok button click event
void ConnectForm::on_buttonBox_accepted()
{
	this->accept();
}

/// Cancel button click event
void ConnectForm::on_buttonBox_rejected()
{
	this->reject();
}
