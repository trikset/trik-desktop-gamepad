#pragma once

#include <QTcpSocket>
#include <QIODevice>

class ConnectionManager
{

public:
    ConnectionManager();

    bool isConnected() const;
    bool waitForConnected(int msecs = 30000);

    QString getIp() const;

    qint64 write(const char *data);

    void connectToHost(const QString & hostName, quint16 port, QIODevice::OpenMode openMode = QIODevice::ReadWrite);
    void disconnectFromHost();
    void setIp(const QString &value);

private:
    QTcpSocket socket;
    QString ip;
};

