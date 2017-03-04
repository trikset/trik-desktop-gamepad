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

QString ConnectionManager::getIp() const
{
    return ip;
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

void ConnectionManager::setIp(const QString &value)
{
    ip = value;
}

