#pragma once

#include <QtNetwork>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QTcpSocket>
#include <QIODevice>

#include <QQueue>

class ConnectionThread : public QThread
{
	Q_OBJECT

public:
	ConnectionThread(QObject *parent = 0);
	~ConnectionThread();


	void run() Q_DECL_OVERRIDE;


	void setHostName(const QString &value);
	void setPort(const quint16 &value);

public slots:
	void sendCommand(const QString &commandMessage);

signals:
	void socketStateChanged(QAbstractSocket::SocketState state);


private:
	QQueue<QString> commandsQueue;

	QString hostName;
	quint16 port;
	QMutex mutex;
	QWaitCondition cond;
	bool quit;
};

