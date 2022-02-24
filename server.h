#ifndef SERVER_H
#define SERVER_H

#include <QObject>

QT_BEGIN_NAMESPACE
class QTcpServer;
QT_END_NAMESPACE

//! [0]
class Server : public QObject
{
    Q_OBJECT

public:
    explicit Server(QObject *parent = nullptr);

private slots:
    void sendFortune();

private:
    void initServer();

    QTcpServer *tcpServer = nullptr;
};
//! [0]

#endif
