#include "connectionManager.h"

ConnectionManager::ConnectionManager()
	: socket(new QTcpSocket(this))
{
	/// passing this to QTcpSocket forces automatically socket->moveToThread()
	/// when calling connectionManaget.moveToThread()
	qRegisterMetaType<QAbstractSocket::SocketState>();
	connect(socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
			this, SIGNAL(stateChanged(QAbstractSocket::SocketState)));
}

ConnectionManager::~ConnectionManager()
{
	disconnect(socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
			   this, SIGNAL(stateChanged(QAbstractSocket::SocketState)));
	delete socket;
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
	emit dataWasWritten(static_cast<int>(result));
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
