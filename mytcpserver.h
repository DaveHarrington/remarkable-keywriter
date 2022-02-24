#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H
#include <QtCore>
#include <QtNetwork>

class MyTcpServer : public QObject
{
    Q_OBJECT
public:
    explicit MyTcpServer(QObject *parent = 0);

signals:
    void dataReceived(QVariant text);
    //void dataReceived();

private slots:
    void newConnection();
    void disconnected();
    void readyRead();

private:
    QTcpServer *server;
    // We need a buffer to store data until block has completely received
    QHash<QTcpSocket*, QByteArray*> buffers;
    // We need to store the size to verify if a block has received completely
    QHash<QTcpSocket*, qint32*> sizes;
};
#endif // MYTCPSERVER_H

// #ifndef MYTCPSERVER_H
// #define MYTCPSERVER_H
// 
// #include <QObject>
// #include <QTcpSocket>
// #include <QTcpServer>
// #include <QDebug>
// 
// class MyTcpServer : public QObject
// {
//     Q_OBJECT
// public:
//     explicit MyTcpServer(QObject *parent = 0);
//     
// signals:
//     
// public slots:
//     void newConnection();
// 
// private:
//     QTcpServer *server;
// };
// 
// #endif // MYTCPSERVER_H
