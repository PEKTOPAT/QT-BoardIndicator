#include "mytcpclient.h"

MyTcpClient::MyTcpClient()
{
    mTcpSocket = NULL;//Объект сокета
}

MyTcpClient::~MyTcpClient(){}

//---------------------------------------------
//***--Слот установки соединения клиент-сервер--***
//---------------------------------------------
void MyTcpClient::slotConnectToServ(const QString &host, quint16 port)
{
    if (mTcpSocket == NULL)
    {
      mTcpSocket = new QTcpSocket(this);
      mTcpSocket->connectToHost(host, port);
      connect(mTcpSocket, SIGNAL(readyRead()), this, SLOT(slotSockReady()));
      connect(mTcpSocket, SIGNAL(connected()), this, SLOT(slotSockConn()));
      connect(mTcpSocket, SIGNAL(disconnected()), this, SLOT(slotSockDisc()));
    }
    else
    {
      delete mTcpSocket;
      mTcpSocket = NULL;
    }
}

void MyTcpClient::slotSockReady()
{
    emit sendMessage(mTcpSocket->readAll());
}

void MyTcpClient::slotSockConn()
{
  qDebug() << "Server connected";
}

void MyTcpClient::slotSockDisc()
{
    qDebug() << "Server disconnected";
    mTcpSocket->deleteLater();
    mTcpSocket = NULL;
}

