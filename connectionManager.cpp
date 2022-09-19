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

ConnectionManager::ConnectionManager()
	: cameraIp("192.168.77.1")
	, cameraPort("8080")
	, gamepadIp("192.168.77.1")
{
}

ConnectionManager::~ConnectionManager()
{
	reset();
	keepaliveTimer->deleteLater();
	socket->deleteLater();
}

void ConnectionManager::init()
{
	keepaliveTimer = new QTimer();
	socket = new QTcpSocket();
	/// passing this to QTcpSocket allows `socket` to be moved
	/// to another thread with the parent
	/// when connectionManager.moveToThread() is called
	qRegisterMetaType<QAbstractSocket::SocketState>();
	connect(socket, &QTcpSocket::stateChanged, this, &ConnectionManager::stateChanged);
	connect(keepaliveTimer, &QTimer::timeout, this, [this]() { write("keepalive 4000\n"); } );
}

bool ConnectionManager::isConnected() const
{
	return socket->state() == QTcpSocket::ConnectedState;
}

QString ConnectionManager::getCameraIp() const
{
	return cameraIp;
}

void ConnectionManager::write(const QString &data)
{
	qint64 result = socket->write(data.toLatin1().data());
	Q_EMIT dataWasWritten(static_cast<int>(result));
}

void ConnectionManager::reset()
{
	if (!keepaliveTimer->isActive())
		return;
	keepaliveTimer->stop();
	socket->disconnectFromHost();
	Q_EMIT dataWasWritten(-1); // simulate disconnect
}

quint16 ConnectionManager::getGamepadPort() const
{
	return gamepadPort;
}

QString ConnectionManager::getGamepadIp() const
{
	return gamepadIp;
}

void ConnectionManager::connectToHost(const QString &cIp, const QString &cPort, const QString &gIp, quint16 gPort)
{
	reset();
	constexpr auto timeout = 3 * 1000;
	QEventLoop loop;
	QTimer::singleShot(timeout, &loop, &QEventLoop::quit);
	connect(socket, &QTcpSocket::connected, &loop, &QEventLoop::quit);
	connect(socket
			, static_cast<void(QTcpSocket::*)(QAbstractSocket::SocketError)>(&QTcpSocket::error)
			, &loop
			, [&loop](QAbstractSocket::SocketError) { loop.quit(); });
	socket->setProxy(QNetworkProxy::NoProxy);
	cameraIp = cIp;
	cameraPort = cPort;
	socket->connectToHost(gamepadIp = gIp, gamepadPort = gPort);
	loop.exec();

	if (socket->state() == QTcpSocket::ConnectedState) {
		keepaliveTimer->start(3000);
	} else {
		socket->abort();
		Q_EMIT connectionFailed();
	}
}

QString ConnectionManager::getCameraPort() const
{
	return cameraPort;
}
