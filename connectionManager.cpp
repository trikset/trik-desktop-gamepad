#include "connectionManager.h"

ConnectionManager::ConnectionManager()
{

}

bool ConnectionManager::isConnected() const
{
	return socket.state() == QTcpSocket::ConnectedState;
}

bool ConnectionManager::waitForConnected(int msecs)
{
	return socket.waitForConnected(msecs);
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
