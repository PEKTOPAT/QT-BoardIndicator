#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>
#include <QTime>

class MyTcpServer:public QTcpServer
{
    Q_OBJECT
public:
    MyTcpServer();
    ~MyTcpServer();



public slots:
    void slotNewConnection();
    void slotServerRead();
    void slotClientDisconnected();
    void slotServerWrite();
    void start_stop_Time(bool condition);

private:
    QTcpSocket * mTcpSocket;
    QTimer *tmr;
    QTime *time;
    QByteArray Data;
};

#endif // MYTCPSERVER_H
