#include "connectionThread.h"

ConnectionThread::ConnectionThread(QObject *parent)
	: QThread(parent), quit(false)
{
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
	QMutexLocker locker(&mutex);
	commandsQueue.enqueue(commandMessage);
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


	while (commandsQueue.isEmpty()) {
		mutex.lock();
		cond.wait(&mutex);
		mutex.unlock();
	}

	while (!quit) {
		mutex.lock();
		const char *data = commandsQueue.dequeue().toLatin1().data();
		int result = socket.write(data);

		/// force socket to write data
		socket.flush();

		if (!result)
			return;
		if (commandsQueue.isEmpty())
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
