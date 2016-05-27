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
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QtWidgets/QMessageBox>
#include <QKeyEvent>
#include <QMenuBar>
#include <memory>
#include <SDL2/SDL.h>

GamepadForm::GamepadForm()
	: QWidget()
    , mUi(new Ui::GamepadForm())
{
	// Here all GUI widgets are created and initialized.
	mUi->setupUi(this);    
//    Tried to implement camera support. Currently not woking
//    QMediaPlayer *mp = new QMediaPlayer(0, 0);
//    std::shared_ptr<QMediaPlayer> mp(new QMediaPlayer(0, 0));
//    QMediaContent *mc = new QMediaContent(QUrl("http://127.0.0.1:8080/vlc"));
//    mp->setMedia(*mc);
//    QVideoWidget *vw = new QVideoWidget(this);
//    vw->setMaximumSize(704, 576);
//    vw->setMinimumSize(704, 576);

//    mp->setVideoOutput(vw);
//    vw->show();
//    mp->play();

    mSocket = new QTcpSocket;
    createMenu();
    createTimer();
    createConnection();
}

GamepadForm::~GamepadForm()
{
	// Gracefully disconnecting from host.
    mSocket->disconnectFromHost();
	// Here we do not care for success or failure of operation since we will close anyway.
    mSocket->waitForDisconnected(3000);
}

void GamepadForm::createConnection() {

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
    QMenuBar* menuBar = new QMenuBar();
    QMenu *fileMenu = new QMenu("&Connection");
    menuBar->addMenu(fileMenu);
    connectAction = new QAction(tr("&Connect"), this);
    // Set to QKeySequence for Ctrl+N shortcut
    connectAction->setShortcuts(QKeySequence::New);
    connect(connectAction, &QAction::triggered, this, &GamepadForm::openConnectDialog);
    exitAction = new QAction(tr("&Exit"), this);
    exitAction->setShortcuts(QKeySequence::Quit);
    connect(exitAction, &QAction::triggered, this, &GamepadForm::exit);
    aboutAction = new QAction(tr("&About"), this);
    connect(aboutAction, &QAction::triggered, this, &GamepadForm::about);
    fileMenu->addAction(connectAction);
    fileMenu->addAction(exitAction);
    menuBar->addAction(aboutAction);
    this->layout()->setMenuBar(menuBar);
}

void GamepadForm::createTimer()
{
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(checkConnection()));
    timer->start(1000);

}

// Function for checking connection
void GamepadForm::checkConnection()
{
    QString successfulConnection = "Connected.\nButtons enabled.";
    QString unsuccessfulConnection = "Disconnected.\nButtons disabled.";
    QString colorRed = "QLabel {color : red; }";
    QString colorGreen = "QLabel {color : green; }";
    if (mSocket->state() != QTcpSocket::ConnectedState)
    {
        (mUi->connection)->setText(unsuccessfulConnection);
        (mUi->connection)->setStyleSheet(colorRed);
        setButtonsEnabled(false);
    }
    else
    {
        (mUi->connection)->setText(successfulConnection);
        (mUi->connection)->setStyleSheet(colorGreen);
        setButtonsEnabled(true);
    }
}

// Function for buttons enabled
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

// Function for keyPressEvent handling
void GamepadForm::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
        case Qt::Key_A:
            (mUi->buttonPad1Left)->pressed();
            break;
        case Qt::Key_S:
            (mUi->buttonPad1Down)->pressed();
            break;
        case Qt::Key_D:
            (mUi->ButtonPad1Right)->pressed();
            break;
        case Qt::Key_W:
            (mUi->buttonPad1Up)->pressed();
            break;
        case Qt::Key_Left:
            (mUi->buttonPad2Left)->pressed();
            break;
        case Qt::Key_Down:
            (mUi->buttonPad2Down)->pressed();
            break;
        case Qt::Key_Right:
            (mUi->ButtonPad2Right)->pressed();
            break;
        case Qt::Key_Up:
            (mUi->buttonPad2Up)->pressed();
            break;
        case Qt::Key_1:
            (mUi->button1)->pressed();
            break;
        case Qt::Key_2:
            (mUi->button2)->pressed();
            break;
        case Qt::Key_3:
            (mUi->button3)->pressed();
            break;
        case Qt::Key_4:
            (mUi->button4)->pressed();
            break;
        case Qt::Key_5:
            (mUi->button5)->pressed();
            break;
    }
}

// Function for keyRelease event
void GamepadForm::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key())
    {
        case Qt::Key_A:
            (mUi->buttonPad1Left)->released();
            break;
        case Qt::Key_S:
            (mUi->buttonPad1Down)->released();
            break;
        case Qt::Key_D:
            (mUi->ButtonPad1Right)->released();
            break;
        case Qt::Key_W:
            (mUi->buttonPad1Up)->released();
            break;
        case Qt::Key_Left:
            (mUi->buttonPad2Left)->released();
            break;
        case Qt::Key_Down:
            (mUi->buttonPad2Down)->released();
            break;
        case Qt::Key_Right:
            (mUi->ButtonPad2Right)->released();
            break;
        case Qt::Key_Up:
            (mUi->buttonPad2Up)->released();
            break;
    }
}

// Button pressed event
void GamepadForm::onButtonPressed(int buttonId)
{
	// Checking that we are still connected, just in case.
    if (mSocket->state() != QTcpSocket::ConnectedState)
		return;

	// Sending "btn <buttonId>" command to robot.
    if (mSocket->write(QString("btn %1\n").arg(buttonId).toLatin1()) == -1)
        // If sending failed for some reason, we think that we lost connection and disable buttons.
        setButtonsEnabled(false);
}

// Pad pressed event
void GamepadForm::onPadPressed(const QString &action)
{
	// Here we send "pad <padId> <x> <y>" command.
    if (mSocket->state() != QTcpSocket::ConnectedState)
		return;	

    if (mSocket->write((action + "\n").toLatin1()) == -1)
		setButtonsEnabled(false);	
}

// Pad released event
void GamepadForm::onPadReleased(int padId)
{
	// Here we send "pad <padId> up" command.
    if (mSocket->state() != QTcpSocket::ConnectedState)
		return;

    if (mSocket->write(QString("pad %1 up\n").arg(padId).toLatin1()) == -1)
        setButtonsEnabled(false);
}

// Function for opening connection dialog
void GamepadForm::openConnectDialog()
{
    mMyNewConnect = new ConnectForm(mSocket);
    mMyNewConnect->show();
}

void GamepadForm::exit()
{
    qApp->exit();
}

void GamepadForm::about()
{
    QString title = "About TRIK Gamepad";
    QString about =  "TRIK 1.1.0\n\nThis is desktop gamepad for TRIK robots.";
    QMessageBox::about(this, title, about);
}

// Tried to implement joystic support, currently not working
//void GamepadForm::joystickMove()
//{
//    SDL_Joystick *joy;
//    SDL_Event event;

//    SDL_Init(SDL_INIT_JOYSTICK);
//    SDL_JoystickOpen(0);
//    SDL_JoystickEventState(SDL_ENABLE);

//    joy = SDL_JoystickOpen(0);

//    while(1)
//    {
//        unsigned int j = 0;
//        for (j = 0; j < 10000000; j++);
//        if (event.jaxis.axis == 0)
//        {
//            if (event.jaxis.value == -32768)
//                (mUi->buttonPad1Left) -> pressed();
//            else if (event.jaxis.value == 32767)
//                (mUi->ButtonPad1Right) -> pressed();
//        } else if (event.jaxis.axis == 1)
//        {
//            if (event.jaxis.value == -32768)
//                (mUi->buttonPad1Up) -> pressed();
//            else if (event.jaxis.value == 32767)
//                (mUi->buttonPad1Down) -> pressed();
//        }
//    }
//}
