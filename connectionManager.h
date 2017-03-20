#pragma once

#include <QTcpSocket>
#include <QIODevice>
#include <QtCore/QTimer>

class ConnectionManager : public QObject
{
	Q_OBJECT

public:
	ConnectionManager();

	bool isConnected() const;
	void waitForConnected(int msecs = 3000);

	QString getCameraIp() const;

	qint64 write(const char *data);

	void connectToHost(const QString & hostName, quint16 port, QIODevice::OpenMode openMode = QIODevice::ReadWrite);
	void disconnectFromHost();
	void setCameraIp(const QString &value);

	QString getCameraPort() const;
	void setCameraPort(const QString &value);

signals:
	void onConnectButtonClicked();
	void stateChanged(QAbstractSocket::SocketState socketState);

private slots:
	void checkConnection();

private:
	QTimer *timer;

	QTcpSocket socket;
	QString cameraIp;
	QString cameraPort;
};

