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
#include <QScopedPointer>
#include <QTimer>
#include <QSettings>


/// TODO description
class ConnectionManager : public QObject
{
	Q_OBJECT
	Q_DISABLE_COPY(ConnectionManager)

public:
	explicit ConnectionManager(QSettings *settings, QObject *parent = nullptr);
	~ConnectionManager();

	/// inits manager after moved to correct thread
	void init();

	/// set camera and gamepad connection parameters
	void setCameraAndGamepad(const QString &cameraIp, const QString &cameraPort, const QString &gamepadIp, quint16 gamepadPort);

	/// checks connection
	bool isConnected() const;

public slots:
	/// Reinstantiate the connection to the desired host
	void reconnectToHost();
	/// TODO description
	void write(const QString &);

	/// Disconnect
	void reset();

signals:
	/// TODO description
	void stateChanged(QAbstractSocket::SocketState socketState);
	/// TODO description
	void dataWasWritten(int);
	/// TODO description
	void connectionFailed();

private:
	QTcpSocket *mSocket {};
	QTimer *mKeepaliveTimer {};
	QSettings *mSettings; // No ownership
};
