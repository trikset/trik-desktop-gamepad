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
 * This file was modified by Yurii Litvinov and Mikhail Wall to make it comply with the requirements of trikRuntime
 * project. See git revision history for detailed changes. */

#include "gamepadForm.h"

#include "ui_gamepadForm.h"

#include <QtWidgets/QMessageBox>
#include <QtGui/QKeyEvent>

GamepadForm::GamepadForm()
		: QWidget()
		, mUi(new Ui::GamepadForm())
		, mSocket(new QTcpSocket)
{
	// Here all GUI widgets are created and initialized.
	mUi->setupUi(this);
	setUpGamepadForm();
}

GamepadForm::~GamepadForm()
{
	// Gracefully disconnecting from host.
	mSocket.disconnectFromHost();
}

void GamepadForm::setUpGamepadForm()
{
	createMenu();
	createTimer();
	createConnection();
	retranslate();
}

void GamepadForm::createConnection()
{

	const auto buttonClickedMapper = [this](int buttonId) {
		return [this, buttonId]() { onButtonPressed(buttonId); };
	};

	// Connecting buttons to handler using this function.
	connect(mUi->button1, &QPushButton::pressed, buttonClickedMapper(1));
	connect(mUi->button2, &QPushButton::pressed, buttonClickedMapper(2));
	connect(mUi->button3, &QPushButton::pressed, buttonClickedMapper(3));
	connect(mUi->button4, &QPushButton::pressed, buttonClickedMapper(4));
	connect(mUi->button5, &QPushButton::pressed, buttonClickedMapper(5));

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

void GamepadForm::createMenu()
{
	// Path to your local directory. Set up if you don't put .qm files into your debug folder.
	const QString pathToDir = "trikDesktopGamepad";

	const QString russian = pathToDir + "_ru";
	const QString english = pathToDir + "_en";
	const QString french = pathToDir + "_fr";
	const QString german = pathToDir + "_de";

	// Constants for checkmark setting
	const int russianId = 0;
	const int englishId = 1;
	const int frenchId = 2;
	const int germanId = 3;

	mMenuBar = QSharedPointer<QMenuBar>(new QMenuBar);

	mConnectionMenu = QSharedPointer<QMenu>(new QMenu);
	mMenuBar.data()->addMenu(mConnectionMenu.data());

	mLanguageMenu = QSharedPointer<QMenu>(new QMenu);
	mMenuBar.data()->addMenu(mLanguageMenu.data());

	mConnectAction = QSharedPointer<QAction>(new QAction(this));
	// Set to QKeySequence for Ctrl+N shortcut
	mConnectAction->setShortcuts(QKeySequence::New);
	connect(mConnectAction.data(), &QAction::triggered, this, &GamepadForm::openConnectDialog);

	mExitAction = QSharedPointer<QAction>(new QAction(this));
	mExitAction->setShortcuts(QKeySequence::Quit);
	connect(mExitAction.data(), &QAction::triggered, this, &GamepadForm::exit);

	mRussianLanguageAction = QSharedPointer<QAction>(new QAction(this));
	mEnglishLanguageAction = QSharedPointer<QAction>(new QAction(this));
	mFrenchLanguageAction = QSharedPointer<QAction>(new QAction(this));
	mGermanLanguageAction = QSharedPointer<QAction>(new QAction(this));

	// Set up language actions checkable
	mEnglishLanguageAction->setCheckable(true);
	mRussianLanguageAction->setCheckable(true);
	mFrenchLanguageAction->setCheckable(true);
	mGermanLanguageAction->setCheckable(true);
	mEnglishLanguageAction->setChecked(true);


	// Declaring lambda-function that calls language handler with correct language.
	const auto languageHandler = [this](const QString &language) {
		return [this, language] { changeLanguage(language); };
	};

	// Declaring lambda-functon that setting up chosen language
	const auto checkmarkSetter = [this](const int &id) {
		return [this, id] { changeCheckmark(id); };
	};

	// Connecting languages to checkmark
	connect(mRussianLanguageAction.data(), &QAction::triggered, this, checkmarkSetter(russianId));
	connect(mEnglishLanguageAction.data(), &QAction::triggered, this, checkmarkSetter(englishId));
	connect(mFrenchLanguageAction.data(), &QAction::triggered, this, checkmarkSetter(frenchId));
	connect(mGermanLanguageAction.data(), &QAction::triggered, this, checkmarkSetter(germanId));


	// Connecting languages to menu items
	connect(mRussianLanguageAction.data(), &QAction::triggered, this, languageHandler(russian));
	connect(mEnglishLanguageAction.data(), &QAction::triggered, this, languageHandler(english));
	connect(mFrenchLanguageAction.data(), &QAction::triggered, this, languageHandler(french));
	connect(mGermanLanguageAction.data(), &QAction::triggered, this, languageHandler(german));

	mAboutAction = QSharedPointer<QAction>(new QAction(this));
	connect(mAboutAction.data(), &QAction::triggered, this, &GamepadForm::about);

	mConnectionMenu->addAction(mConnectAction.data());
	mConnectionMenu->addAction(mExitAction.data());

	mLanguageMenu->addAction(mRussianLanguageAction.data());
	mLanguageMenu->addAction(mEnglishLanguageAction.data());
	mLanguageMenu->addAction(mFrenchLanguageAction.data());
	mLanguageMenu->addAction(mGermanLanguageAction.data());

	mMenuBar->addAction(mAboutAction.data());

	this->layout()->setMenuBar(mMenuBar.data());
}

void GamepadForm::createTimer()
{
	mTimer = QSharedPointer<QTimer>(new QTimer(this));
	connect(mTimer.data(), &QTimer::timeout, this, &GamepadForm::checkConnection);
	mTimer.data()->start(1000);

}

void GamepadForm::checkConnection()
{
	const QString successfulConnection = tr("Connected.\nButtons enabled.");
	const QString unsuccessfulConnection = tr("Disconnected.\nButtons disabled.");
	const QString colorRed = "QLabel {color : red; }";
	const QString colorGreen = "QLabel {color : green; }";

	if (mSocket.state() != QTcpSocket::ConnectedState) {
		mUi->connection->setText(unsuccessfulConnection);
		mUi->connection->setStyleSheet(colorRed);
		setButtonsEnabled(false);
	} else {
		mUi->connection->setText(successfulConnection);
		mUi->connection->setStyleSheet(colorGreen);
		setButtonsEnabled(true);
	}
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

void GamepadForm::keyPressEvent(QKeyEvent *event)
{
	switch (event->key())
	{
	case Qt::Key_A:
		mUi->buttonPad1Left->pressed();
		break;
	case Qt::Key_S:
		mUi->buttonPad1Down->pressed();
		break;
	case Qt::Key_D:
		mUi->ButtonPad1Right->pressed();
		break;
	case Qt::Key_W:
		mUi->buttonPad1Up->pressed();
		break;
	case Qt::Key_Left:
		mUi->buttonPad2Left->pressed();
		break;
	case Qt::Key_Down:
		mUi->buttonPad2Down->pressed();
		break;
	case Qt::Key_Right:
		mUi->ButtonPad2Right->pressed();
		break;
	case Qt::Key_Up:
		mUi->buttonPad2Up->pressed();
		break;
	case Qt::Key_1:
		mUi->button1->pressed();
		break;
	case Qt::Key_2:
		mUi->button2->pressed();
		break;
	case Qt::Key_3:
		mUi->button3->pressed();
		break;
	case Qt::Key_4:
		mUi->button4->pressed();
		break;
	case Qt::Key_5:
		mUi->button5->pressed();
		break;
    }
}

void GamepadForm::keyReleaseEvent(QKeyEvent *event)
{
	switch (event->key())
	{
	case Qt::Key_A:
		mUi->buttonPad1Left->released();
		break;
	case Qt::Key_S:
		mUi->buttonPad1Down->released();
		break;
	case Qt::Key_D:
		mUi->ButtonPad1Right->released();
		break;
	case Qt::Key_W:
		mUi->buttonPad1Up->released();
		break;
	case Qt::Key_Left:
		mUi->buttonPad2Left->released();
		break;
	case Qt::Key_Down:
		mUi->buttonPad2Down->released();
		break;
	case Qt::Key_Right:
		mUi->ButtonPad2Right->released();
		break;
	case Qt::Key_Up:
		mUi->buttonPad2Up->released();
		break;
	}
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

void GamepadForm::openConnectDialog()
{
	mMyNewConnectForm = QSharedPointer<ConnectForm>(new ConnectForm(&mSocket));
	mMyNewConnectForm.data()->show();
}

void GamepadForm::exit()
{
	qApp->exit();
}

void GamepadForm::changeEvent(QEvent *event)
{
	if (event->type() == QEvent::LanguageChange)
	{
		mUi->retranslateUi(this);

		retranslate();
	}

	QWidget::changeEvent(event);
}

void GamepadForm::retranslate()
{
	mConnectionMenu->setTitle(tr("&Connection"));
	mLanguageMenu->setTitle(tr("&Language"));
	mConnectAction->setText(tr("&Connect"));
	mExitAction->setText(tr("&Exit"));

	mRussianLanguageAction->setText(tr("&Russian"));
	mEnglishLanguageAction->setText(tr("&English"));
	mFrenchLanguageAction->setText(tr("&French"));
	mGermanLanguageAction->setText(tr("&German"));

	mAboutAction->setText(tr("&About"));

}

void GamepadForm::changeLanguage(const QString &language)
{
	mTranslator = QSharedPointer<QTranslator>(new QTranslator);
	mTranslator.data()->load(language);
	qApp->installTranslator(mTranslator.data());

	mUi->retranslateUi(this);
}

void GamepadForm::changeCheckmark(const int &languageId)
{
	switch (languageId) {

	// Russian language
	case 0:
		mRussianLanguageAction->setChecked(true);
		mEnglishLanguageAction->setChecked(false);
		mFrenchLanguageAction->setChecked(false);
		mGermanLanguageAction->setChecked(false);
		break;

	// English language
	case 1:
		mRussianLanguageAction->setChecked(false);
		mEnglishLanguageAction->setChecked(true);
		mFrenchLanguageAction->setChecked(false);
		mGermanLanguageAction->setChecked(false);
		break;

	// French language
	case 2:
		mRussianLanguageAction->setChecked(false);
		mEnglishLanguageAction->setChecked(false);
		mFrenchLanguageAction->setChecked(true);
		mGermanLanguageAction->setChecked(false);
		break;

	// German language
	case 3:
		mRussianLanguageAction->setChecked(false);
		mEnglishLanguageAction->setChecked(false);
		mFrenchLanguageAction->setChecked(false);
		mGermanLanguageAction->setChecked(true);
		break;
	}
}

void GamepadForm::about()
{
	const QString title = tr("About TRIK Gamepad");
	const QString about =  tr("TRIK Gamepad 1.1.2\n\nThis is desktop gamepad for TRIK robots.");
	QMessageBox::about(this, title, about);
}

