#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <mytcpserver.h>
#include <mytcpclient.h>
#include <ledindicator.h>
#include <QGroupBox>
#include <boardindiatorobjects.h>
#include <QGridLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QSpinBox>
#include <QLabel>
#include <QStyleFactory>

#include <QFile>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool event(QEvent *ev);

private:
    Ui::MainWindow *ui;
    // =========Панель управления========
    QLabel *Label_Addr;
    QLabel *Label_Port;
    QLineEdit *LineEdit_Addr;
    QLineEdit *LineEdit_Port;
    QPushButton *PushButton_Connect;
    QPushButton *PushButton_On;
    QPushButton *PushButton_Off;
    QGroupBox *GroupBox_Control;
    QGridLayout *GridLayout_Control;
    // =========Панель индикации=========
    QGroupBox *GroupBox_Indication;
    QGridLayout *GridLayout_Indication;
    // ==================================
    QGridLayout *GridLayout_CentralWidget;
    QWidget *Widget_Central;

    MyTcpClient *Client;

    int flagConnected;
    bool fixWindow;
    QStringList outSplitMassiv;
    QStringList inSplitMassiv;
    QString allMassiv;
    QByteArray InData;
    QStringList BIObjectName;
    QList<BoardIndiatorObjects*> BIObjects;

public slots:
    void ReceiveMessage(QString message);
    void ParseMessage(QByteArray Data);
    void ConnectToServ();
    void MSGselector(QStringList message);
    void MSGPlace(QStringList message);
    void MSGSubPlace(QStringList message);
    void MSGColor(QStringList message);
    void MSGText(QStringList message);
    void MSGAfterLight(QStringList message);
    void MSGSize(QStringList message);
    void MSGClear();
    void MSGFixWindow();
    void IndicationOn();
    void IndicationOff();


private slots:


};

#endif // MAINWINDOW_H
