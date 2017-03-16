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

ConnectForm::ConnectForm(ConnectionManager *manager
						 , QWidget *parent)
	: QDialog(parent)
	, mUi(new Ui::ConnectForm)
	, connectionManager(manager)
{
	mUi->setupUi(this);

	setVisibilityToAdditionalButtons(false);

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
	connect(mUi->advancedButton, &QPushButton::pressed, this, &ConnectForm::onAdvancedButtonClicked);
	connect(connectionManager, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
			this, SLOT(dealWithState(QAbstractSocket::SocketState)), Qt::QueuedConnection);
}

ConnectForm::~ConnectForm()
{

}

void ConnectForm::onConnectButtonClicked()
{
	const auto ip = mUi->robotIpLineEdit->text();
	const auto port = mUi->robotPortLineEdit->text().toInt();

	if (mUi->cameraIPLineEdit->text().isEmpty())
		connectionManager->setCameraIp(ip);
	else
		connectionManager->setCameraIp(mUi->cameraIPLineEdit->text());
	connectionManager->setCameraPort(mUi->cameraPortLineEdit->text());
	emit connectionManager->onConnectButtonClicked();

	// Connecting. 4444 is hardcoded here since it is default gamepad port on TRIK.

	/*
	QFuture<void> connectFunction = QtConcurrent::run(
		this->connectionManager, &ConnectionManager::connectToHost, ip, port, QIODevice::ReadWrite);
	connectFunction.waitForFinished();
	*/
	//connectionManager->connectToHost(ip, port);

	// Waiting for opened connection and checking that connection is actually established.

	/*
	if (!connectionManager->waitForConnected(3000)) {
		// If not, warn user.
		QMessageBox::warning(this, tr("Connection failed"), tr("Failed to connect to robot"));
	} else {
		mUi->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
		QMessageBox::information(this, tr("Connection succeeded"), tr("Connected to robot"));
	}
	*/
}

void ConnectForm::onAdvancedButtonClicked()
{
	mUi->advancedButton->setVisible(false);
	setVisibilityToAdditionalButtons(true);
}

void ConnectForm::dealWithState(QAbstractSocket::SocketState socketState)
{
	qDebug() << socketState;
	qDebug() << "ConnectForm thread id is " << QThread::currentThreadId();
	switch (socketState) {
	case QAbstractSocket::ConnectedState:
		this->accept();
		break;
	case QAbstractSocket::UnconnectedState:
		QMessageBox::warning(this, tr("Connection failed"), tr("Failed to connect to robot"));
		break;
	default:
		break;
	}
}

void ConnectForm::setVisibilityToAdditionalButtons(bool mode)
{
	mUi->cameraIPLabel->setVisible(mode);
	mUi->cameraIPLineEdit->setVisible(mode);
	mUi->cameraPortLabel->setVisible(mode);
	mUi->cameraPortLineEdit->setVisible(mode);
	mUi->robotPortLabel->setVisible(mode);
	mUi->robotPortLineEdit->setVisible(mode);
}
