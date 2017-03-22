#include "connectionThread.h"

ConnectionThread::ConnectionThread(QObject *parent)
	: QThread(parent), quit(false)
{
	command = "first";
}

ConnectionThread::~ConnectionThread()
{
	mutex.lock();
	quit = true;
	cond.wakeOne();
	mutex.unlock();
	wait();
}

void ConnectionThread::sendCommand(const QString &commandMessage)
{
	qDebug() << currentThreadId();
	QMutexLocker locker(&mutex);
	command = commandMessage;
	if (!isRunning())
		start();
	else
		cond.wakeOne();
}

void ConnectionThread::run()
{
	const int timeout = 3 * 1000;

	QTcpSocket socket;
	connect(&socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
			this, SIGNAL(socketStateChanged(QAbstractSocket::SocketState)));

	socket.connectToHost(hostName, port, QIODevice::ReadWrite);

	if (!socket.waitForConnected(timeout)) {
		// emit errorSignal
		return;
	}

	while (!quit) {
		qDebug() << currentThreadId();
		qDebug() << command;
		mutex.lock();
		const char *data = command.toLatin1().data();
		int result = socket.write(data);

		/// force socket to write data
		socket.flush();

		if (!result)
			return;
		cond.wait(&mutex);
		mutex.unlock();
	}

}

void ConnectionThread::setHostName(const QString &value)
{
	hostName = value;
}

void ConnectionThread::setPort(const quint16 &value)
{
	port = value;
}
