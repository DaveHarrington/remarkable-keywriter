#include <QCoreApplication>
#include <QKeyEvent>

#include "mytcpserver.h"

static inline qint32 ArrayToInt(QByteArray source);

MyTcpServer::MyTcpServer(QObject *parent) :
    QObject(parent)
{
    server = new QTcpServer(this);
    connect(server, SIGNAL(newConnection()), SLOT(newConnection()));
    qDebug() << "Listening: " << server->listen(QHostAddress::Any, 1024);
}

void MyTcpServer::newConnection()
{
    while (server->hasPendingConnections())
    {
        QTcpSocket *socket = server->nextPendingConnection();
        connect(socket, SIGNAL(readyRead()), SLOT(readyRead()));
        connect(socket, SIGNAL(disconnected()), SLOT(disconnected()));
        QByteArray *buffer = new QByteArray();
        qint32 *s = new qint32(0);
        buffers.insert(socket, buffer);
        sizes.insert(socket, s);
    }
}

void MyTcpServer::disconnected()
{
    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    QByteArray *buffer = buffers.value(socket);
    qint32 *s = sizes.value(socket);
    socket->deleteLater();
    delete buffer;
    delete s;
}

void MyTcpServer::readyRead()
{
    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    QByteArray *buffer = buffers.value(socket);
    char nextByte;
//    qint32 *s = sizes.value(socket);
//    qint32 size = *s;
    while (socket->bytesAvailable() > 0)
    {
        buffer->append(socket->readAll());
	//socket->write(buffer);
	//socket->flush();
	while (buffer->size() > 0) {
	    nextByte = buffer->front();
	    buffer->remove(0, 1);
	    //emit dataReceived(nextByte);
            Qt::Key nextKey = Qt::Key_K;
            emit dataReceived(nextKey);


//            QKeyEvent keyPressEvent = QKeyEvent(QEvent::Type::KeyPress, Qt::Key_X, Qt::NoModifier, "x");
//            QCoreApplication::postEvent (this->topLevel, &keyPressEvent);


//             auto FOCUSOBJ = QGuiApplication::focusObject(); 
// 	    QKeyEvent *pressEvent = new QKeyEvent(QKeyEvent::KeyPress, Qt::Key_X, Qt::NoModifier, "x");
// 	    QKeyEvent *releaseEvent = new QKeyEvent(QEvent::KeyRelease, Qt::Key_X, Qt::NoModifier);
// 	    QCoreApplication::sendEvent(FOCUSOBJ, &pressEvent);
// 	    QCoreApplication::sendEvent(FOCUSOBJ, &releaseEvent);

	    qDebug() << "Server got: " << (int)nextByte << " " << nextByte;
	}
    }
}
//	qDebug() << "Got bytes: " << size << " " << buffer->size();
//        buffer->append(socket->readAll());
//	    while ((size == 0 && buffer->size() >= 4) || (size > 0 && buffer->size() >= size)) //While can process data, process it
//	    {
//            if (size == 0 && buffer->size() >= 4) //if size of data has received completely, then store it on our global variable
//            {
//                size = ArrayToInt(buffer->mid(0, 4));
//                *s = size;
//                buffer->remove(0, 4);
//            }
//            if (size > 0 && buffer->size() >= size) // If data has received completely, then emit our SIGNAL with the data
//            {
//                QByteArray data = buffer->mid(0, size);
//                buffer->remove(0, size);
//                size = 0;
//                *s = size;
//                emit dataReceived(data);
//                qDebug() << "Got: " << data;
//                socket->write(data);
//		socket->flush();
//                
//            }
//        }
//    }
//}

qint32 ArrayToInt(QByteArray source)
{
    qint32 temp;
    QDataStream data(&source, QIODevice::ReadWrite);
    data >> temp;
    return temp;
}

// MyTcpServer::MyTcpServer(QObject *parent) :
//     QObject(parent)
// {
//     server = new QTcpServer(this);
// 
//     // whenever a user connects, it will emit signal
//     connect(server, SIGNAL(newConnection()),
//             this, SLOT(newConnection()));
// 
//     if(!server->listen(QHostAddress::Any, 9999))
//     {
//         qDebug() << "Server could not start";
//     }
//     else
//     {
//         qDebug() << "Server started!";
//     }
// }
// 
// void MyTcpServer::newConnection()
// {
// 
//     while 
//     // need to grab the socket
//     QTcpSocket *socket = server->nextPendingConnection();
// 
//     socket->write("Hello client\r\n");
//     socket->flush();
// 
//     socket->waitForBytesWritten(3000);
// 
//     socket->close();
// }
