#pragma once

#include <QtNetwork>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QTcpSocket>
#include <QIODevice>

class ConnectionThread : public QThread
{
	Q_OBJECT

public:
	ConnectionThread(QObject *parent = 0);
	~ConnectionThread();

	void sendCommand(const QString &commandMessage);
	void run() Q_DECL_OVERRIDE;


	void setHostName(const QString &value);
	void setPort(const quint16 &value);

signals:
	void socketStateChanged(QAbstractSocket::SocketState state);


private:
	QString command;

	QString hostName;
	quint16 port;
	QMutex mutex;
	QWaitCondition cond;
	bool quit;
};

