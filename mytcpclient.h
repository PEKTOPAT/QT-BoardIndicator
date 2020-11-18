#ifndef MYTCPCLIENT_H
#define MYTCPCLIENT_H

#include <QTcpSocket>
#include <QString>

class MyTcpClient:public QTcpSocket
{
    Q_OBJECT
public:
    MyTcpClient();
    ~MyTcpClient();
    QTcpSocket *mTcpSocket;

signals:
        void sendMessage(QByteArray Message);

private:


public slots:
    void slotConnectToServ(const QString &host, quint16 port);
    void slotSockReady();
    void slotSockConn();
    void slotSockDisc();

};

#endif // MYTCPCLIENT_H
