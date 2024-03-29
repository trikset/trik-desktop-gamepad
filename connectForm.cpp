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

#include "connectForm.h"
#include "ui_connectForm.h"


#include <QtWidgets/QMessageBox>


ConnectForm::ConnectForm(ConnectionManager *manager
						 , QSettings *settings
						 , QWidget *parent)
	: QDialog(parent)
	, mUi(new Ui::ConnectForm)
	, mSettings(settings)
	, connectionManager(manager)
{
	mUi->setupUi(this);

	setVisibilityToAdditionalButtons(false);

	// These constants was added for translations purposes
	const QString buttonCancel = tr("Cancel");
	const QString connectButton = tr("Connect");
	const QString advancedButton = tr("Advanced settings...");

	mUi->cancelButton->setText(buttonCancel);
	mUi->connectButton->setText(connectButton);
	mUi->advancedButton->setText(advancedButton);

	mUi->robotIpLineEdit->setText(settings->value("gamepadIp", "192.168.77.1").toString());
	mUi->robotPortLineEdit->setText(settings->value("gamepadPort", "4444").toString());
	mUi->cameraIPLineEdit->setText(settings->value("cameraIp", "192.168.77.1").toString());
	mUi->cameraPortLineEdit->setText(settings->value("cameraPort", "8080").toString());

	// Connecting buttons with methods
	connect(mUi->cancelButton, &QPushButton::pressed, this, &QDialog::reject);
	connect(mUi->connectButton, &QPushButton::pressed, this, &ConnectForm::onConnectButtonClicked);
	connect(mUi->advancedButton, &QPushButton::pressed, this, &ConnectForm::onAdvancedButtonClicked);
	connect(mUi->robotIpLineEdit, &QLineEdit::textEdited, this, &ConnectForm::copyGamepadIpToCameraIp);
}

void ConnectForm::onConnectButtonClicked()
{
	const auto ip = mUi->robotIpLineEdit->text();
	const quint16 port = static_cast<quint16>(mUi->robotPortLineEdit->text().toInt());

	const auto &cameraIp = mUi->cameraIPLineEdit->text().isEmpty() ? ip : mUi->cameraIPLineEdit->text();
	const auto &cameraPort  = mUi->cameraPortLineEdit->text();

	this->reject(); //??? Why `reject`?


	mSettings->setValue("cameraIp", cameraIp);
	mSettings->setValue("cameraPort", cameraPort);
	mSettings->setValue("gamepadIp", ip);
	mSettings->setValue("gamepadPort", port);

	Q_EMIT newConnectionParameters();
}

void ConnectForm::onAdvancedButtonClicked()
{
	disconnect(mUi->robotIpLineEdit, &QLineEdit::textEdited, this, &ConnectForm::copyGamepadIpToCameraIp);
	mUi->advancedButton->setVisible(false);
	setVisibilityToAdditionalButtons(true);
}

void ConnectForm::copyGamepadIpToCameraIp(const QString& text)
{
	mUi->cameraIPLineEdit->setText(text);
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
