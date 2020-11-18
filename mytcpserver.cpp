#include "mytcpserver.h"
#include <QDebug>
#include <QCoreApplication>

#define intervalTimer 80


MyTcpServer::MyTcpServer()
{
    mTcpSocket = NULL;
    //Задание таймера
    time = new QTime();//Объект времени
    tmr = new QTimer();//Объект таймера
    tmr->setInterval(intervalTimer);//интервал таймера
    connect(this, SIGNAL(newConnection()), this, SLOT(slotNewConnection()));
    connect(tmr,SIGNAL(timeout()), this, SLOT(slotServerWrite()));//Соединение сигнала timeout таймера tmr со слотом

    if(!this->listen(QHostAddress::Any, 23))
    {
        qDebug() << "server is not started";
    }
    else
    {
        qDebug() << "server is started";
    }
}
MyTcpServer::~MyTcpServer(){}



void MyTcpServer::slotNewConnection()
{
    qDebug() << "slotNewConnection()";
    if (mTcpSocket != NULL)
    {
      slotClientDisconnected();
    }
    mTcpSocket = this->nextPendingConnection();
    mTcpSocket->write("Connect to server successfully!!\r\n");

    connect(mTcpSocket, SIGNAL(readyRead()), this, SLOT(slotServerRead()));
    connect(mTcpSocket, SIGNAL(disconnected()), this, SLOT(slotClientDisconnected()));

}

//---------------------------------------------
//***-------Слот выдачи команд клиенту------***
//---------------------------------------------
void MyTcpServer::slotServerWrite()
{
    mTcpSocket->write("01");
}


//---------------------------------------------
//***-------Слот чтения команд от клиента-----***
//---------------------------------------------
void MyTcpServer::slotServerRead()
{
  qDebug() << "slotServerRead()";

    while(mTcpSocket->bytesAvailable()>0)
    {
        QByteArray array = mTcpSocket->readAll();
        //mTcpSocket->write(array);
        //qDebug() << "array.size()" << array.size();
        for (int i = 0; i < array.size(); i++)
        qDebug() << "array[" << i << "]= " << (quint8)(array[i]);
    }
}

//---------------------------------------------
//***--------Слот разъединения сессии--------***
//---------------------------------------------
void MyTcpServer::slotClientDisconnected()
{
  qDebug() << "slotClientDisconnected()";

  if (mTcpSocket != NULL)
  {
    qDebug() << "slotClientDisconnected() mTcpSocket != NULL";

    disconnect(mTcpSocket, &QTcpSocket::readyRead, this, &MyTcpServer::slotServerRead);
    disconnect(mTcpSocket, &QTcpSocket::disconnected, this, &MyTcpServer::slotClientDisconnected);

    mTcpSocket->close();
    mTcpSocket->deleteLater();
    mTcpSocket = NULL;
  }
}

//---------------------------------------------
//***--------Слот запуска таймера--------***
//---------------------------------------------

void MyTcpServer::start_stop_Time(bool condition)
{

    if(condition == 1)
    {
        tmr->start();//запускаем таймер
        time->start();//Запускаем время
    }else
    {
        tmr->stop();
    }
}



