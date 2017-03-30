#include "connectionManager.h"

ConnectionManager::ConnectionManager()
{
	/// (this) lets socket->moveToThread()
	socket = new QTcpSocket(this);
	qRegisterMetaType<QAbstractSocket::SocketState>();
	connect(socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
			this, SIGNAL(stateChanged(QAbstractSocket::SocketState)));
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
	int result = socket->write(data.toLatin1().data());
	emit dataWasWritten(result);
}

void ConnectionManager::connectToHost()
{
	const int timeout = 3 * 1000;
	socket->connectToHost(gamepadIp, gamepadPort);
	if (!socket->waitForConnected(timeout))
		emit connectionFailed();
}

void ConnectionManager::disconnectFromHost()
{
	socket->disconnectFromHost();
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
