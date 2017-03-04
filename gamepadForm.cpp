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
 * This file was modified by Yurii Litvinov, Aleksei Alekseev and Mikhail Wall to make it comply with the requirements of trikRuntime
 * project. See git revision history for detailed changes. */

#include "gamepadForm.h"

#include "ui_gamepadForm.h"

#include <QtWidgets/QMessageBox>
#include <QtGui/QKeyEvent>

#include <QDebug>
#include <QNetworkRequest>
#include <QMediaContent>

GamepadForm::GamepadForm()
		: QWidget()
		, mUi(new Ui::GamepadForm())
{
	// Here all GUI widgets are created and initialized.
	mUi->setupUi(this);
	this->installEventFilter(this);
	setUpGamepadForm();
}

GamepadForm::~GamepadForm()
{
	// Gracefully disconnecting from host.
    connectionManager.disconnectFromHost();
}

void GamepadForm::setUpGamepadForm()
{
	createMenu();
	createTimer();
	createConnection();
    setVideoController();
    retranslate();
}

void GamepadForm::setVideoController()
{
    player = new QMediaPlayer(this, QMediaPlayer::StreamPlayback);
    videoWidget = new QVideoWidget(this);
    videoWidget->setMinimumSize(320, 240);
    videoWidget->setVisible(false);
    mUi->verticalLayout->addWidget(videoWidget);
    mUi->verticalLayout->setAlignment(videoWidget, Qt::AlignCenter);
}

void GamepadForm::checkCamera()
{
    const QString colorRed = "QLabel {color : red; }";
    const QString colorGreen = "QLabel {color : green; }";
    const QString colorBlue = "QLabel {color : blue; }";

    const QString ip = connectionManager.getIp();

    const QMediaPlayer::MediaStatus status = player->mediaStatus();
    if ((status == QMediaPlayer::NoMedia || status == QMediaPlayer::InvalidMedia) && !ip.isEmpty() ) {
        const QString url = "http://" + ip + ":8080/?action=stream";
        QNetworkRequest nr = QNetworkRequest(url);
        nr.setPriority(QNetworkRequest::LowPriority);
        nr.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::AlwaysCache);
        player->setMedia(QMediaContent(nr));
        player->setVideoOutput(videoWidget);
        player->play();
    } else if (status == QMediaPlayer::StalledMedia) {
        mUi->cameraStatus->setText(tr("Camera status: Streaming"));
        mUi->cameraStatus->setStyleSheet(colorGreen);
        mUi->label->setVisible(false);
        videoWidget->setVisible(true);
    } else if (status == QMediaPlayer::LoadingMedia) {
        mUi->cameraStatus->setText(tr("Camera status: Loading Media..."));
        mUi->cameraStatus->setStyleSheet(colorBlue);
    } else {
        mUi->cameraStatus->setText(tr("Camera status: No Media"));
        mUi->cameraStatus->setStyleSheet(colorRed);
        mUi->label->setVisible(true);
        videoWidget->setVisible(false);
    }
}

void GamepadForm::createConnection()
{
	mMapperPadPressed = new QSignalMapper(this);
	mMapperPadReleased = new QSignalMapper(this);
	mMapperDigitPressed = new QSignalMapper(this);

	mPadHashtable = {
			{mUi->buttonPad1Up, {1, 0, 100}}
			, {mUi->buttonPad1Down, {1, 0, -100}}
			, {mUi->buttonPad1Right, {1, 100, 0}}
			, {mUi->buttonPad1Left, {1, -100, 0}}
			, {mUi->buttonPad2Up, {2, 0, 100}}
			, {mUi->buttonPad2Down, {2, 0, -100}}
			, {mUi->buttonPad2Right, {2, 100 ,0}}
			, {mUi->buttonPad2Left, {2, -100, 0}}
	};

	for (auto button : mPadHashtable.keys()) {
		connect(button, SIGNAL(pressed()), mMapperPadPressed, SLOT(map()));
		mMapperPadPressed->setMapping(button, button);
		connect(button, SIGNAL(released()), mMapperPadReleased, SLOT(map()));
		mMapperPadReleased->setMapping(button, button);
	}

	connect(mMapperPadPressed, SIGNAL(mapped(QWidget *)), this, SLOT(handlePadPress(QWidget*)));
	connect(mMapperPadReleased, SIGNAL(mapped(QWidget *)), this, SLOT(handlePadRelease(QWidget*)));

	mDigitHashtable = {
			{mUi->button1, 1}
			, {mUi->button2, 2}
			, {mUi->button3, 3}
			, {mUi->button4, 4}
			, {mUi->button5, 5}
	};

	for (auto button : mDigitHashtable.keys()) {
		connect(button, SIGNAL(pressed()), mMapperDigitPressed, SLOT(map()));
		mMapperDigitPressed->setMapping(button, button);
	}

	connect(mMapperDigitPressed, SIGNAL(mapped(QWidget *)), this, SLOT(handleDigitPress(QWidget*)));

}

void GamepadForm::createMenu()
{
	// Path to your local directory. Set up if you don't put .qm files into your debug folder.
	const QString pathToDir = "languages/trikDesktopGamepad";

	const QString russian = pathToDir + "_ru";
	const QString english = pathToDir + "_en";
	const QString french = pathToDir + "_fr";
	const QString german = pathToDir + "_de";

	mMenuBar = new QMenuBar(this);

	mConnectionMenu = new QMenu(this);
	mMenuBar->addMenu(mConnectionMenu);

	mLanguageMenu=  new QMenu(this);
	mMenuBar->addMenu(mLanguageMenu);

	mConnectAction = new QAction(this);
	// Set to QKeySequence for Ctrl+N shortcut
	mConnectAction->setShortcuts(QKeySequence::New);
	connect(mConnectAction, &QAction::triggered, this, &GamepadForm::openConnectDialog);

	mExitAction = new QAction(this);
	mExitAction->setShortcuts(QKeySequence::Quit);
	connect(mExitAction, &QAction::triggered, this, &GamepadForm::exit);

	mRussianLanguageAction = new QAction(this);
	mEnglishLanguageAction = new QAction(this);
	mFrenchLanguageAction = new QAction(this);
	mGermanLanguageAction = new QAction(this);

	mLanguages = new QActionGroup(this);
	mLanguages->addAction(mRussianLanguageAction);
	mLanguages->addAction(mEnglishLanguageAction);
	mLanguages->addAction(mFrenchLanguageAction);
	mLanguages->addAction(mGermanLanguageAction);
	mLanguages->setExclusive(true);

	// Set up language actions checkable
	mEnglishLanguageAction->setCheckable(true);
	mRussianLanguageAction->setCheckable(true);
	mFrenchLanguageAction->setCheckable(true);
	mGermanLanguageAction->setCheckable(true);
	mEnglishLanguageAction->setChecked(true);

	// Connecting languages to menu items
	connect(mRussianLanguageAction, &QAction::triggered, this, [this, russian](){changeLanguage(russian);});
	connect(mEnglishLanguageAction, &QAction::triggered, this, [this, english](){changeLanguage(english);});
	connect(mFrenchLanguageAction, &QAction::triggered, this, [this, french](){changeLanguage(french);});
	connect(mGermanLanguageAction, &QAction::triggered, this, [this, german](){changeLanguage(german);});

	mAboutAction = new QAction(this);
	connect(mAboutAction, &QAction::triggered, this, &GamepadForm::about);

	mConnectionMenu->addAction(mConnectAction);
	mConnectionMenu->addAction(mExitAction);

	mLanguageMenu->addAction(mRussianLanguageAction);
	mLanguageMenu->addAction(mEnglishLanguageAction);
	mLanguageMenu->addAction(mFrenchLanguageAction);
	mLanguageMenu->addAction(mGermanLanguageAction);

	mMenuBar->addAction(mAboutAction);

	this->layout()->setMenuBar(mMenuBar);
}

void GamepadForm::createTimer()
{
	connect(&mTimer, &QTimer::timeout, this, &GamepadForm::checkConnection);
    connect(&mTimer, &QTimer::timeout, this, &GamepadForm::checkCamera);
	mTimer.start(1000);
}

void GamepadForm::checkConnection()
{
	const QString successfulConnection = tr("Connected.\nButtons enabled.");
	const QString unsuccessfulConnection = tr("Disconnected.\nButtons disabled.");
	const QString colorRed = "QLabel {color : red; }";
	const QString colorGreen = "QLabel {color : green; }";

    if (!connectionManager.isConnected()) {
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
	mUi->buttonPad1Right->setEnabled(enabled);
	mUi->buttonPad1Up->setEnabled(enabled);
	mUi->buttonPad1Down->setEnabled(enabled);

	mUi->buttonPad2Left->setEnabled(enabled);
	mUi->buttonPad2Right->setEnabled(enabled);
	mUi->buttonPad2Up->setEnabled(enabled);
	mUi->buttonPad2Down->setEnabled(enabled);
}

bool GamepadForm::eventFilter(QObject *obj, QEvent *event)
{
	Q_UNUSED(obj)

	int resultingPowerX1 = 0;
	int resultingPowerY1 = 0;
	int resultingPowerX2 = 0;
	int resultingPowerY2 = 0;

	// Handle key press event
	if(event->type() == QEvent::KeyPress) {

		mPressedKeys += ((QKeyEvent*) event)->key();

		// Handle W A S D buttons
		resultingPowerX1 = (mPressedKeys.contains(Qt::Key_D) ? 100 : 0) + (mPressedKeys.contains(Qt::Key_A) ? -100 : 0);
		resultingPowerY1 = (mPressedKeys.contains(Qt::Key_S) ? -100 : 0) + (mPressedKeys.contains(Qt::Key_W) ? 100 : 0);
		// Handle arrow buttons
		resultingPowerX2 = (mPressedKeys.contains(Qt::Key_Right) ? 100 : 0) + (mPressedKeys.contains(Qt::Key_Left) ? -100 : 0);
		resultingPowerY2 = (mPressedKeys.contains(Qt::Key_Down) ? -100 : 0) + (mPressedKeys.contains(Qt::Key_Up) ? 100 : 0);

		if (resultingPowerX1 != 0 || resultingPowerY1 != 0) {
			onPadPressed(QString("pad 1 %1 %2").arg(resultingPowerX1).arg(resultingPowerY1));
		} else if (resultingPowerX2 != 0 || resultingPowerY2 != 0) {
			onPadPressed(QString("pad 2 %1 %2").arg(resultingPowerX2).arg(resultingPowerY2));
		}

		// Handle 1 2 3 4 5 buttons
		QMap<int, QPushButton *> digits = {
				{Qt::Key_1, mUi->button1}
				, {Qt::Key_2, mUi->button2}
				, {Qt::Key_3, mUi->button3}
				, {Qt::Key_4, mUi->button4}
				, {Qt::Key_5, mUi->button5}
		};

		for (auto key : digits.keys()) {
			if (mPressedKeys.contains(key)) {
				digits[key]->pressed();
			}
		}
	} else if(event->type() == QEvent::KeyRelease) {

		// Handle key release event
		QSet<int> pad1 = {Qt::Key_W, Qt::Key_A, Qt::Key_S, Qt::Key_D};
		QSet<int> pad2 = {Qt::Key_Left, Qt::Key_Right, Qt::Key_Up, Qt::Key_Down};

		auto releasedKey = ((QKeyEvent*) event)->key();
		mPressedKeys -= releasedKey;

		if (pad1.contains(releasedKey)) {
			onPadReleased(1);
		} else if (pad2.contains(releasedKey)){
			onPadReleased(2);
		}
	}

	return false;
}

void GamepadForm::onButtonPressed(int buttonId)
{
	// Checking that we are still connected, just in case.
    if (!connectionManager.isConnected()) {
		return;
	}

	// Sending "btn <buttonId>" command to robot.
    if (connectionManager.write(QString("btn %1\n").arg(buttonId).toLatin1()) == -1) {
		// If sending failed for some reason, we think that we lost connection and disable buttons.
		setButtonsEnabled(false);
	}
}

void GamepadForm::onPadPressed(const QString &action)
{
	// Here we send "pad <padId> <x> <y>" command.
    if (!connectionManager.isConnected()) {
		return;
	}

    if (connectionManager.write((action + "\n").toLatin1()) == -1) {
		setButtonsEnabled(false);
	}
}

void GamepadForm::onPadReleased(int padId)
{
	// Here we send "pad <padId> up" command.
    if (!connectionManager.isConnected()) {
		return;
	}

    if (connectionManager.write(QString("pad %1 up\n").arg(padId).toLatin1()) == -1) {
		setButtonsEnabled(false);
	}
}

void GamepadForm::openConnectDialog()
{
    mMyNewConnectForm = new ConnectForm(&connectionManager);
	mMyNewConnectForm->show();
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

void GamepadForm::handlePadRelease(QWidget *w)
{
	onPadReleased(mPadHashtable[w][0]);
}

void GamepadForm::handlePadPress(QWidget *w)
{
	auto vec = mPadHashtable[w];
	onPadPressed(QString("pad %1 %2 %3").arg(vec[0]).arg(vec[1]).arg(vec[2]));
}

void GamepadForm::handleDigitPress(QWidget *w)
{
	onButtonPressed(mDigitHashtable[w]);
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
	mTranslator = new QTranslator(this);
	mTranslator->load(language);
	qApp->installTranslator(mTranslator);

	mUi->retranslateUi(this);
}

void GamepadForm::about()
{
	const QString title = tr("About TRIK Gamepad");
	const QString about =  tr("TRIK Gamepad 1.1.2\n\nThis is desktop gamepad for TRIK robots.");
	QMessageBox::about(this, title, about);
}
