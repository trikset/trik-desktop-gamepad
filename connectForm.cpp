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

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"

#include <QtWidgets/QMessageBox>

#pragma GCC diagnostic pop

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
	const QString connectButton = tr("Connect");
	const QString advancedButton = tr("Advanced...");

	mUi->cancelButton->setText(buttonCancel);
	mUi->connectButton->setText(connectButton);
	mUi->advancedButton->setText(advancedButton);

	// Connecting buttons with methods
	connect(mUi->cancelButton, &QPushButton::pressed, this, &QDialog::reject);
	connect(mUi->connectButton, &QPushButton::pressed, this, &ConnectForm::onConnectButtonClicked);
	connect(mUi->advancedButton, &QPushButton::pressed, this, &ConnectForm::onAdvancedButtonClicked);
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

	connectionManager->setGamepadIp(ip);
	connectionManager->setGamepadPort(port);
	this->reject();

	emit dataReceived();

}

void ConnectForm::onAdvancedButtonClicked()
{
	mUi->advancedButton->setVisible(false);
	setVisibilityToAdditionalButtons(true);
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
