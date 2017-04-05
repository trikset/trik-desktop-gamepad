#pragma once

#include <QTcpSocket>
#include <QIODevice>


class ConnectionManager : public QObject
{
	Q_OBJECT

private:
	ConnectionManager(const ConnectionManager &other);
	ConnectionManager & operator=(const ConnectionManager &other);

public:
	ConnectionManager();
	~ConnectionManager();

	bool isConnected() const;

	void setCameraIp(const QString &value);
	QString getCameraIp() const;

	QString getCameraPort() const;
	void setCameraPort(const QString &value);

	QString getGamepadIp() const;
	void setGamepadIp(const QString &value);

	quint16 getGamepadPort() const;
	void setGamepadPort(const quint16 &value);

public slots:
	void connectToHost();
	void disconnectFromHost();
	void write(const QString &);

signals:
	void stateChanged(QAbstractSocket::SocketState socketState);
	void dataWasWritten(int);
	void connectionFailed();

private:
	QTcpSocket *socket;
	QString cameraIp;
	QString cameraPort;

	QString gamepadIp;
	quint16 gamepadPort;
};
