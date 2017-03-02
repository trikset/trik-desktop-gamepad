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
 * This file was modified by Mikhail Wall to make it comply with the requirements of trikRuntime
 * project. See git revision history for detailed changes. */

#include "connectForm.h"
#include "ui_connectForm.h"

#include <QtWidgets/QMessageBox>

ConnectForm::ConnectForm(QTcpSocket *socket
        , QString *ip
        , QWidget *parent)
		: QDialog(parent)
		, mUi(new Ui::ConnectForm)
		, mSocket(socket)
        , ip(ip)

{
	mUi->setupUi(this);

	// These constants was added for translations purposes
	const QString buttonCancel = tr("Cancel");
	const QString buttonOK = tr("Ok");
	const QString connectButton = tr("Connect");

	mUi->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

	mUi->buttonBox->button(QDialogButtonBox::Ok)->setText(buttonOK);
	mUi->buttonBox->button(QDialogButtonBox::Cancel)->setText(buttonCancel);
	mUi->connectButton->setText(connectButton);

	// Connecting buttons with methods
	connect(mUi->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
	connect(mUi->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
	connect(mUi->connectButton, &QPushButton::pressed, this, &ConnectForm::onConnectButtonClicked);
}

ConnectForm::~ConnectForm()
{

}

void ConnectForm::onConnectButtonClicked()
{
    *ip = mUi->robotIpLineEdit->text();

	// Connecting. 4444 is hardcoded here since it is default gamepad port on TRIK.
    mSocket->connectToHost(*ip, 4444);
	// Waiting for opened connection and checking that connection is actually established.
	if (!mSocket->waitForConnected(3000)) {
		// If not, warn user.
		QMessageBox::warning(this, tr("Connection failed"), tr("Failed to connect to robot"));
	} else {
		mUi->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
		QMessageBox::information(this, tr("Connection succeeded"), tr("Connected to robot"));
	}
}
