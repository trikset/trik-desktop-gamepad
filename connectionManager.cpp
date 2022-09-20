/* Copyright 2017 Konstantin Batoev.
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
 * This file was modified by Konstantin Batoev to make it comply with the requirements of trikRuntime
 * project. See git revision history for detailed changes. */

#include <QEventLoop>
#include <QNetworkProxy>
#include "connectionManager.h"

ConnectionManager::ConnectionManager(QSettings *settings, QObject *parent)
	: QObject(parent)
	, mSettings(settings)
{
}

ConnectionManager::~ConnectionManager()
{
	reset();
	mKeepaliveTimer->deleteLater();
	mSocket->deleteLater();
}

void ConnectionManager::init()
{
	mKeepaliveTimer = new QTimer();
	mSocket = new QTcpSocket();
	/// passing this to QTcpSocket allows `socket` to be moved
	/// to another thread with the parent
	/// when connectionManager.moveToThread() is called
	qRegisterMetaType<QAbstractSocket::SocketState>();
	connect(mSocket, &QTcpSocket::stateChanged, this, &ConnectionManager::stateChanged);
	connect(mKeepaliveTimer, &QTimer::timeout, this, [this]() { write("keepalive 4000\n"); } );
}

bool ConnectionManager::isConnected() const
{
	return mSocket->state() == QTcpSocket::ConnectedState;
}

void ConnectionManager::write(const QString &data)
{
	qint64 result = mSocket->write(data.toLatin1().data());
	Q_EMIT dataWasWritten(static_cast<int>(result));
}

void ConnectionManager::reset()
{
	if (!mKeepaliveTimer->isActive())
		return;
	mKeepaliveTimer->stop();
	mSocket->disconnectFromHost();
	Q_EMIT dataWasWritten(-1); // simulate disconnect
}

void ConnectionManager::reconnectToHost()
{
	reset();
	constexpr auto timeout = 3 * 1000;
	QEventLoop loop;
	QTimer::singleShot(timeout, &loop, &QEventLoop::quit);
	connect(mSocket, &QTcpSocket::connected, &loop, &QEventLoop::quit);
	connect(mSocket
			, static_cast<void(QTcpSocket::*)(QAbstractSocket::SocketError)>(&QTcpSocket::error)
			, &loop
			, [&loop](QAbstractSocket::SocketError) { loop.quit(); });
	mSocket->setProxy(QNetworkProxy::NoProxy);
	const auto &gamepadIp = mSettings->value("gamepadIp").toString();
	auto gamepadPort = static_cast<quint16>(mSettings->value("gamepadPort").toUInt());
	mSocket->connectToHost(gamepadIp, gamepadPort);
	loop.exec();

	if (mSocket->state() == QTcpSocket::ConnectedState) {
		mKeepaliveTimer->start(3000);
	} else {
		mSocket->abort();
		Q_EMIT connectionFailed();
	}
}
