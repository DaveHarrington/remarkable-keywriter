#include <QCoreApplication>
#include <QKeyEvent>

#include "mytcpserver.h"

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

    // Force telnet into char mode
    // IAC WONT LINEMODE IAC WILL ECHO
    socket->write("\377\375\042\377\373\001");
    qDebug() << "Connected! Sent WONT LINEMODE";
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
  QByteArray nextByte;
  QString nextString;

  while (socket->bytesAvailable() > 0)
  {
    buffer->append(socket->readAll());
    while (buffer->size() > 0) {
      nextByte = buffer->mid(0, 10);
      buffer->remove(0, 10);
      nextString = QString::fromLatin1(nextByte);
      qDebug() << "to latin1: " << nextString;
      emit dataReceived(nextString);
    }
  }
}
