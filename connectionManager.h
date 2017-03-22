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

	QString getGamepadIp() const;
	void setGamepadIp(const QString &value);

	quint16 getGamepadPort() const;
	void setGamepadPort(const quint16 &value);

signals:
	void onConnectButtonClicked();
	void stateChanged(QAbstractSocket::SocketState socketState);
	void dataReceived();

private slots:
	void checkConnection();

private:
	QTimer *timer;

	QTcpSocket socket;
	QString cameraIp;
	QString cameraPort;

	QString gamepadIp;
	quint16 gamepadPort;
};

