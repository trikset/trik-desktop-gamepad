#pragma once

#include <QTcpSocket>
#include <QIODevice>

class ConnectionManager
{

public:
    ConnectionManager();

    bool isConnected() const;
    bool waitForConnected(int msecs = 30000);

    QString getCameraIp() const;

    qint64 write(const char *data);

    void connectToHost(const QString & hostName, quint16 port, QIODevice::OpenMode openMode = QIODevice::ReadWrite);
    void disconnectFromHost();
    void setCameraIp(const QString &value);

    QString getCameraPort() const;
    void setCameraPort(const QString &value);

private:
    QTcpSocket socket;
    QString cameraIp;
    QString cameraPort;
};

