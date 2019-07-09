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
 * This file was modified by Yurii Litvinov, Aleksei Alekseev, Mikhail Wall and Konstantin Batoev
 * to make it comply with the requirements of trikRuntime project.
 * See git revision history for detailed changes. */

#include "gamepadForm.h"
#include "ui_gamepadForm.h"

#include <QtWidgets/QMessageBox>
#include <QtGui/QKeyEvent>

#include <QtNetwork/QNetworkRequest>
#include <QtMultimedia/QMediaContent>
#include <QtGui/QFontDatabase>

GamepadForm::GamepadForm()
	: QWidget()
	, mUi(new Ui::GamepadForm())
	, strategy(Strategy::getStrategy(Strategies::standartStrategy))
{
	mUi->setupUi(this);
	this->installEventFilter(this);
	setUpGamepadForm();
	connectionManager.moveToThread(&thread);
	connect(&thread, &QThread::finished, &connectionManager, &ConnectionManager::reset);
	thread.start();
}

GamepadForm::~GamepadForm()
{
	thread.quit();
	thread.wait(1000);
}

void GamepadForm::startController(QStringList args)
{
	connectionManager.setGamepadIp(args.at(1));
	QString portStr = args.size() < 3 ? "4444" : args.at(2);
	quint16 gamepadPort = static_cast<quint16>(portStr.toInt());
	QString cameraPort = args.size() < 4 ? "8080" : args.at(3);
	QString cameraIp = args.size() < 5 ? connectionManager.getGamepadIp() : args.at(4);
	connectionManager.setCameraPort(cameraPort);
	connectionManager.setGamepadPort(gamepadPort);
	connectionManager.setCameraIp(cameraIp);
	startVideoStream();
	QMetaObject::invokeMethod(&connectionManager, &ConnectionManager::connectToHost, Qt::QueuedConnection);
}

void GamepadForm::setUpGamepadForm()
{
	createMenu();
	setFontToPadButtons();
	setUpControlButtonsHash();
	createConnection();
	setVideoController();
	setLabels();
	setImageControl();
	retranslate();
}

void GamepadForm::setVideoController()
{
	videoWidget = new QVideoWidget(this);
	videoWidget->setMinimumSize(320, 240);
	videoWidget->setVisible(false);
	mUi->verticalLayout->addWidget(videoWidget);
	mUi->verticalLayout->setAlignment(videoWidget, Qt::AlignCenter);

	player = new QMediaPlayer(videoWidget, QMediaPlayer::StreamPlayback);

	connect(player, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)),
		this, SLOT(handleMediaStatusChanged(QMediaPlayer::MediaStatus)));
	player->setVideoOutput(videoWidget);

	movie.setFileName(":/images/loading.gif");
	mUi->loadingMediaLabel->setVisible(false);
	mUi->loadingMediaLabel->setMovie(&movie);

	QPixmap pixmap(":/images/noVideoSign.png");
	mUi->invalidMediaLabel->setPixmap(pixmap);

	mUi->loadingMediaLabel->setVisible(false);
	mUi->invalidMediaLabel->setVisible(false);

}

void GamepadForm::handleMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
	mTakeImageAction->setEnabled(false);
	movie.setPaused(true);
	switch (status) {
	case QMediaPlayer::StalledMedia:
	case QMediaPlayer::LoadedMedia:
	case QMediaPlayer::BufferingMedia:
		player->play();
		mTakeImageAction->setEnabled(true);
		mUi->loadingMediaLabel->setVisible(false);
		mUi->invalidMediaLabel->setVisible(false);
		mUi->label->setVisible(false);
		videoWidget->setVisible(true);
		break;

	case QMediaPlayer::LoadingMedia:
		mUi->invalidMediaLabel->setVisible(false);
		mUi->label->setVisible(false);
		videoWidget->setVisible(false);
		mUi->loadingMediaLabel->setVisible(true);
		movie.setPaused(false);
		break;

	case QMediaPlayer::InvalidMedia:
		mUi->loadingMediaLabel->setVisible(false);
		mUi->label->setVisible(false);
		videoWidget->setVisible(false);
		mUi->invalidMediaLabel->setVisible(true);
		break;

	case QMediaPlayer::NoMedia:
	case QMediaPlayer::EndOfMedia:
		mUi->loadingMediaLabel->setVisible(false);
		mUi->invalidMediaLabel->setVisible(false);
		videoWidget->setVisible(false);
		mUi->label->setVisible(true);
		break;

	default:
		break;
	}
}

void GamepadForm::startVideoStream()
{
	const QString ip = connectionManager.getCameraIp();
	const QString port = connectionManager.getCameraPort();
	const auto status = player->mediaStatus();

	if (status == QMediaPlayer::NoMedia || status == QMediaPlayer::EndOfMedia || status == QMediaPlayer::InvalidMedia) {
		const QString url = "http://" + ip + ":" + port + "/?action=stream";
		// QNetworkRequest nr = QNetworkRequest(url);
		// nr.setPriority(QNetworkRequest::LowPriority);
		// nr.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::AlwaysCache);
		player->setMedia(QUrl(url));
	}
}

void GamepadForm::checkSocket(QAbstractSocket::SocketState state)
{
	switch (state) {
	case QAbstractSocket::ConnectedState:
		mUi->disconnectedLabel->setVisible(false);
		mUi->connectedLabel->setVisible(true);
		mUi->connectingLabel->setVisible(false);
		setButtonsCheckable(true);
		setButtonsEnabled(true);
		break;

	case QAbstractSocket::ConnectingState:
		mUi->disconnectedLabel->setVisible(false);
		mUi->connectedLabel->setVisible(false);
		mUi->connectingLabel->setVisible(true);
		setButtonsCheckable(false);
		setButtonsEnabled(false);
		break;

	case QAbstractSocket::UnconnectedState:
	default:
		mUi->disconnectedLabel->setVisible(true);
		mUi->connectedLabel->setVisible(false);
		mUi->connectingLabel->setVisible(false);
		setButtonsCheckable(false);
		setButtonsEnabled(false);
		break;
	}
}

void GamepadForm::checkBytesWritten(int result)
{
	if (result == -1) {
		setButtonsEnabled(false);
		setButtonsCheckable(false);
	}
}

void GamepadForm::showConnectionFailedMessage()
{
	QMessageBox failedConnectionMessage(this);
	failedConnectionMessage.setText(tr("Couldn't connect to robot"));
	failedConnectionMessage.exec();
}

void GamepadForm::setFontToPadButtons()
{
	const int id = QFontDatabase::addApplicationFont(":/fonts/freemono.ttf");
	const QString family = QFontDatabase::applicationFontFamilies(id).at(0);
	const int pointSize = 50;
	QFont font(family, pointSize);

	mUi->buttonPad1Down->setFont(font);
	mUi->buttonPad1Up->setFont(font);
	mUi->buttonPad1Left->setFont(font);
	mUi->buttonPad1Right->setFont(font);

	mUi->buttonPad2Down->setFont(font);
	mUi->buttonPad2Up->setFont(font);
	mUi->buttonPad2Left->setFont(font);
	mUi->buttonPad2Right->setFont(font);
}

void GamepadForm::setButtonChecked(const int &key, bool checkStatus)
{
	controlButtonsHash[key]->setChecked(checkStatus);
}

void GamepadForm::createConnection()
{
	mMapperButtonPressed = new QSignalMapper(this);
	mMapperButtonReleased = new QSignalMapper(this);

	for (auto button : controlButtonsHash.values()) {
		connect(button, SIGNAL(pressed()), mMapperButtonPressed, SLOT(map()));
		mMapperButtonPressed->setMapping(button, button);
		connect(button, SIGNAL(released()), mMapperButtonReleased, SLOT(map()));
		mMapperButtonReleased->setMapping(button, button);
	}

	connect(mMapperButtonPressed, SIGNAL(mapped(QWidget *)), this, SLOT(handleButtonPress(QWidget*)));
	connect(mMapperButtonReleased, SIGNAL(mapped(QWidget *)), this, SLOT(handleButtonRelease(QWidget*)));

	connect(&connectionManager, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
		this, SLOT(checkSocket(QAbstractSocket::SocketState)));
	connect(&connectionManager, SIGNAL(dataWasWritten(int)), this, SLOT(checkBytesWritten(int)));
	connect(&connectionManager, SIGNAL(connectionFailed()), this, SLOT(showConnectionFailedMessage()));
	connect(this, SIGNAL(commandReceived(QString)), &connectionManager, SLOT(write(QString)));

	connect(strategy, SIGNAL(commandPrepared(QString)), this, SLOT(sendCommand(QString)));
	connect(qApp, SIGNAL(applicationStateChanged(Qt::ApplicationState)),
		this, SLOT(dealWithApplicationState(Qt::ApplicationState)));
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

	mModeMenu = new QMenu(this);
	mMenuBar->addMenu(mModeMenu);

	mImageMenu = new QMenu(this);
	mMenuBar->addMenu(mImageMenu);
	mTakeImageAction = new QAction(this);
	mImageMenu->addAction(mTakeImageAction);
	mTakeImageAction->setEnabled(false);
	mTakeImageAction->setShortcut(QKeySequence("Ctrl+I"));
	connect(mTakeImageAction, SIGNAL(triggered(bool)), this, SLOT(requestImage()));

	mLanguageMenu = new QMenu(this);
	mMenuBar->addMenu(mLanguageMenu);

	mConnectAction = new QAction(this);
	// Set to QKeySequence for Ctrl+N shortcut
	mConnectAction->setShortcuts(QKeySequence::New);
	connect(mConnectAction, &QAction::triggered, this, &GamepadForm::openConnectDialog);

	mExitAction = new QAction(this);
	mExitAction->setShortcuts(QKeySequence::Quit);
	connect(mExitAction, &QAction::triggered, this, &GamepadForm::exit);

	mModesActions = new QActionGroup(this);
	mStandartStrategyAction = new QAction(this);
	mAccelerateStrategyAction = new QAction(this);
	mStandartStrategyAction->setCheckable(true);
	mAccelerateStrategyAction->setCheckable(true);
	mStandartStrategyAction->setChecked(true);
	connect(mStandartStrategyAction, &QAction::triggered, this, [this](){changeMode(Strategies::standartStrategy);});
	connect(mAccelerateStrategyAction, &QAction::triggered,
		this, [this](){changeMode(Strategies::accelerateStrategy);});
	mModesActions->addAction(mStandartStrategyAction);
	mModesActions->addAction(mAccelerateStrategyAction);
	mModesActions->setExclusive(true);

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

	mModeMenu->addAction(mStandartStrategyAction);
	mModeMenu->addAction(mAccelerateStrategyAction);

	mLanguageMenu->addAction(mRussianLanguageAction);
	mLanguageMenu->addAction(mEnglishLanguageAction);
	mLanguageMenu->addAction(mFrenchLanguageAction);
	mLanguageMenu->addAction(mGermanLanguageAction);

	mMenuBar->addAction(mAboutAction);

	this->layout()->setMenuBar(mMenuBar);
}

void GamepadForm::setButtonsEnabled(bool enabled)
{
	// Here we enable or disable pads and "magic buttons" depending on given parameter.
	for (auto button : controlButtonsHash.values())
		button->setEnabled(enabled);
}

void GamepadForm::setButtonsCheckable(bool checkableStatus)
{
	for (auto button : controlButtonsHash.values())
		button->setCheckable(checkableStatus);
}

void GamepadForm::setUpControlButtonsHash()
{
	controlButtonsHash.insert(Qt::Key_1, mUi->button1);
	controlButtonsHash.insert(Qt::Key_2, mUi->button2);
	controlButtonsHash.insert(Qt::Key_3, mUi->button3);
	controlButtonsHash.insert(Qt::Key_4, mUi->button4);
	controlButtonsHash.insert(Qt::Key_5, mUi->button5);

	controlButtonsHash.insert(Qt::Key_A, mUi->buttonPad1Left);
	controlButtonsHash.insert(Qt::Key_D, mUi->buttonPad1Right);
	controlButtonsHash.insert(Qt::Key_W, mUi->buttonPad1Up);
	controlButtonsHash.insert(Qt::Key_S, mUi->buttonPad1Down);

	controlButtonsHash.insert(Qt::Key_Left, mUi->buttonPad2Left);
	controlButtonsHash.insert(Qt::Key_Right, mUi->buttonPad2Right);
	controlButtonsHash.insert(Qt::Key_Up, mUi->buttonPad2Up);
	controlButtonsHash.insert(Qt::Key_Down, mUi->buttonPad2Down);
}

void GamepadForm::setLabels()
{
	QPixmap redBall(":/images/redBall.png");
	mUi->disconnectedLabel->setPixmap(redBall);
	mUi->disconnectedLabel->setVisible(true);

	QPixmap greenBall(":/images/greenBall.png");
	mUi->connectedLabel->setPixmap(greenBall);
	mUi->connectedLabel->setVisible(false);

	QPixmap blueBall(":/images/blueBall.png");
	mUi->connectingLabel->setPixmap(blueBall);
	mUi->connectingLabel->setVisible(false);
}

void GamepadForm::setImageControl()
{
	probe = new QVideoProbe(this);
	connect(probe, SIGNAL(videoFrameProbed(QVideoFrame)), this,
		SLOT(saveImageToClipboard(QVideoFrame)), Qt::QueuedConnection);
	isFrameNecessary = false;
	probe->setSource(player);
	clipboard = QApplication::clipboard();
}

bool GamepadForm::eventFilter(QObject *obj, QEvent *event)
{
	Q_UNUSED(obj)

	// Handle key press event for View
	if(event->type() == QEvent::KeyPress) {
		int pressedKey = (dynamic_cast<QKeyEvent *> (event))->key();
		if (controlButtonsHash.keys().contains(pressedKey))
			setButtonChecked(pressedKey, true);

	} else if(event->type() == QEvent::KeyRelease) {

		QKeyEvent *keyEvent = dynamic_cast<QKeyEvent*>(event);

		auto releasedKey = keyEvent->key();
		if (controlButtonsHash.keys().contains(releasedKey))
			setButtonChecked(releasedKey, false);
	}

	// delegating events to Command-generating-strategy
	strategy->processEvent(event);

	return false;
}

void GamepadForm::sendCommand(const QString &command)
{
	if (!connectionManager.isConnected()) {
		return;
	}

	emit commandReceived(command);
}

void GamepadForm::changeMode(Strategies type)
{
	disconnect(strategy, SIGNAL(commandPrepared(QString)), this, SLOT(sendCommand(QString)));
	strategy = Strategy::getStrategy(type);
	connect(strategy, SIGNAL(commandPrepared(QString)), this, SLOT(sendCommand(QString)));
}

void GamepadForm::dealWithApplicationState(Qt::ApplicationState state)
{
	if (state != Qt::ApplicationActive) {
		strategy->reset();
		for (auto button : controlButtonsHash.values())
			button->setChecked(false);
	}
}

void GamepadForm::saveImageToClipboard(QVideoFrame buffer)
{
	if (isFrameNecessary) {
		isFrameNecessary = false;
		QVideoFrame frame(buffer);
		frame.map(QAbstractVideoBuffer::ReadOnly);

		QImage::Format imageFormat = QVideoFrame::imageFormatFromPixelFormat(frame.pixelFormat());
		QImage img;
		// check whether videoframe can be transformed to qimage by qt
		if (imageFormat != QImage::Format_Invalid) {
			img = QImage(frame.bits(),
						 frame.width(),
						 frame.height(),
						 // frame.bytesPerLine(),
						 imageFormat);
		} else {
			int width = frame.width();
			int height = frame.height();
			int size = height * width;
			const uchar *data = frame.bits();

			img = QImage(width, height, QImage::Format_RGB32);
			/// converting from yuv420 to rgb32
			for (int i = 0; i < height; i++)
				for (int j = 0; j < width; j++) {
					int y = static_cast<int> (data[i * width + j]);
					int u = static_cast<int> (data[(i / 2) * (width / 2) + (j / 2) + size]);
					int v = static_cast<int> (data[(i / 2) * (width / 2) + (j / 2) + size + (size / 4)]);

					int r = y + int(1.13983 * (v - 128));
					int g = y - int(0.39465 * (u - 128)) - int(0.58060 * (v - 128));
					int b = y + int(2.03211 * (u - 128));

					r = r < 0 ? 0 : r > 255 ? 255 : r;
					g = g < 0 ? 0 : g > 255 ? 255 : g;
					b = b < 0 ? 0 : b > 255 ? 255 : b;

					img.setPixel(j, i, qRgb(r, g, b));
				}
		}

		clipboard->setImage(img);
		frame.unmap();
	}
}

void GamepadForm::requestImage()
{
	isFrameNecessary = true;
}

void GamepadForm::openConnectDialog()
{
	QMap<QString, QString> args;
	args.insert("gamepadIp", connectionManager.getGamepadIp());
	args.insert("gamepadPort", QString::number(connectionManager.getGamepadPort()));
	args.insert("cameraIp", connectionManager.getCameraIp());
	args.insert("cameraPort", connectionManager.getCameraPort());

	mMyNewConnectForm = new ConnectForm(&connectionManager, args, this);
	mMyNewConnectForm->show();

	connect(mMyNewConnectForm, &ConnectForm::dataReceived, &connectionManager, &ConnectionManager::connectToHost);
	connect(mMyNewConnectForm, &ConnectForm::dataReceived, this, &GamepadForm::startVideoStream);
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

void GamepadForm::handleButtonPress(QWidget *widget)
{
	QPushButton *padButton = dynamic_cast<QPushButton *> (widget);
	padButton->setChecked(true);
	auto key = controlButtonsHash.key(padButton);
	QKeyEvent keyEvent(QEvent::KeyPress, key, Qt::NoModifier);
	strategy->processEvent(&keyEvent);
}

void GamepadForm::handleButtonRelease(QWidget *widget)
{
	QPushButton *padButton = dynamic_cast<QPushButton *> (widget);
	padButton->setChecked(false);
	auto key = controlButtonsHash.key(padButton);
	QKeyEvent keyEvent(QEvent::KeyRelease, key, Qt::NoModifier);
	strategy->processEvent(&keyEvent);
}

void GamepadForm::retranslate()
{
	mConnectionMenu->setTitle(tr("&Connection"));
	mModeMenu->setTitle(tr("&Mode"));
	mLanguageMenu->setTitle(tr("&Language"));

	mConnectAction->setText(tr("&Connect"));
	mExitAction->setText(tr("&Exit"));

	mStandartStrategyAction->setText(tr("&Simple"));
	mAccelerateStrategyAction->setText(tr("&Accelerate"));

	mRussianLanguageAction->setText(tr("&Russian"));
	mEnglishLanguageAction->setText(tr("&English"));
	mFrenchLanguageAction->setText(tr("&French"));
	mGermanLanguageAction->setText(tr("&German"));

	mImageMenu->setTitle(tr("&Image"));
	mTakeImageAction->setText(tr("&Screenshot to clipboard"));

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
	const QString about =  "TRIK Gamepad 2.1.0\n\n"+tr("This is desktop gamepad for TRIK robots.");
	QMessageBox::about(this, title, about);
}
