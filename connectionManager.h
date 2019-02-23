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


#pragma once

#include <QtNetwork/QTcpSocket>
#include <QtCore/QIODevice>


/// TODO description
class ConnectionManager : public QObject
{
	Q_OBJECT

private:
	ConnectionManager(const ConnectionManager &other);
	ConnectionManager & operator=(const ConnectionManager &other);

public:
	ConnectionManager();
	~ConnectionManager();

	/// TODO description
	bool isConnected() const;

	/// TODO description
	void setCameraIp(const QString &value);
	/// TODO description
	QString getCameraIp() const;

	/// TODO description
	QString getCameraPort() const;
	/// TODO description
	void setCameraPort(const QString &value);

	/// TODO description
	void setGamepadIp(const QString &value);

	/// TODO description
	void setGamepadPort(const quint16 &value);

	/// TODO description
	QString getGamepadIp() const;

	/// TODO description
	quint16 getGamepadPort() const;

public slots:
	/// TODO description
	void connectToHost();
	/// TODO description
	void disconnectFromHost();
	/// TODO description
	void write(const QString &);

signals:
	/// TODO description
	void stateChanged(QAbstractSocket::SocketState socketState);
	/// TODO description
	void dataWasWritten(int);
	/// TODO description
	void connectionFailed();

private:
	QTcpSocket *socket; // TODO [Doesn't have | Has] ownership
	QString cameraIp;
	QString cameraPort;

	QString gamepadIp;
	quint16 gamepadPort;
};
