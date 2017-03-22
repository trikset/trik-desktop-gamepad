#include "connectionManager.h"

#include <QDebug>
#include <QThread>

ConnectionManager::ConnectionManager()
{
	qDebug() << socket.state();
	timer = new QTimer(this);
	qRegisterMetaType<QAbstractSocket::SocketState>();
	connect(&socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
			this, SIGNAL(stateChanged(QAbstractSocket::SocketState)), Qt::QueuedConnection);
}

bool ConnectionManager::isConnected() const
{
	return socket.state() == QTcpSocket::ConnectedState;
}

void ConnectionManager::waitForConnected(int msecs)
{
	connect(timer, SIGNAL(timeout()), this, SLOT(checkConnection()));
	connect(timer, SIGNAL(timeout()), timer, SLOT(stop()));
	timer->start(msecs);
}

QString ConnectionManager::getCameraIp() const
{
	return cameraIp;
}

qint64 ConnectionManager::write(const char *data)
{
	return socket.write(data);
}

void ConnectionManager::connectToHost(const QString &hostName, quint16 port, QIODevice::OpenMode openMode)
{
	socket.connectToHost(hostName, port, openMode);
}

void ConnectionManager::disconnectFromHost()
{
	socket.disconnectFromHost();
}

void ConnectionManager::setCameraIp(const QString &value)
{
	cameraIp = value;
}

QString ConnectionManager::getCameraPort() const
{
	return cameraPort;
}

void ConnectionManager::setCameraPort(const QString &value)
{
	cameraPort = value;
}

void ConnectionManager::checkConnection()
{
	qDebug() << socket.state();
	if (socket.state() != QTcpSocket::ConnectedState) {
		socket.abort();
	}
	qDebug() << socket.state();
}

quint16 ConnectionManager::getGamepadPort() const
{
	return gamepadPort;
}

void ConnectionManager::setGamepadPort(const quint16 &value)
{
	gamepadPort = value;
}

QString ConnectionManager::getGamepadIp() const
{
	return gamepadIp;
}

void ConnectionManager::setGamepadIp(const QString &value)
{
	gamepadIp = value;
}
