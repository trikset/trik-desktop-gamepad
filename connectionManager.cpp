#include "connectionManager.h"

#include <QDebug>
#include <QThread>

ConnectionManager::ConnectionManager()
{
	qDebug() << socket.state();
	qDebug() << "Manager thread id is " << QThread::currentThreadId();
	/// function is called to use enum QAbstractSocket::SocketState in queued
	/// signal and slot connections
	int id = qRegisterMetaType<QAbstractSocket::SocketState>();
	qDebug() << id;
	connect(&socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
			this, SIGNAL(stateChanged(QAbstractSocket::SocketState)), Qt::QueuedConnection);
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
	qDebug() << "12123";
	qDebug() << "ConnectToHost function thread id is " << QThread::currentThreadId();
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
