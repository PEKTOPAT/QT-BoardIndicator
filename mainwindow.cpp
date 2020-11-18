#include "mainwindow.h"
#include "ui_mainwindow.h"

// ==================================================================================================================
// ============================================ КОНСТРУКТОР =========================================================
// ==================================================================================================================
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Client = new MyTcpClient();
    this->setWindowTitle("Программная ячейка индикации");
    flagConnected = 0;
    fixWindow = false;
    QPalette darkPalette;
    // =======================================Работа со стилем======================================================
    // Настраиваем палитру для цветовых ролей элементов интерфейса
    //   darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::Window, QColor(30, 60, 120));

    darkPalette.setColor(QPalette::WindowText, Qt::white);
    //  darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
    darkPalette.setColor(QPalette::Base, QColor(10, 40, 90));

    //  darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::AlternateBase, QColor(30, 60, 120));

    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    //  darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::Button, QColor(30, 60, 120));

    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));

    //  darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::Highlight, QColor(60, 190, 240, 170));

    darkPalette.setColor(QPalette::HighlightedText, Qt::black);

    //      qApp->setStyle(QStyleFactory::create("Fusion"));
    //      qApp->setPalette(darkPalette);
    // =======================================Панель управления======================================================
    GroupBox_Control = new QGroupBox(this);
    GroupBox_Control->setTitle("Управление");
    GridLayout_Control = new QGridLayout(GroupBox_Control);

    Label_Addr = new QLabel(this);
    Label_Addr->setText("Адрес:");
    Label_Port = new QLabel(this);
    Label_Port->setText("Порт:");
    LineEdit_Addr = new QLineEdit(this);
//    LineEdit_Addr->setText("127.0.0.1");
    LineEdit_Addr->setText("192.168.001.178");
    LineEdit_Addr->setClearButtonEnabled(1);
    LineEdit_Port = new QLineEdit(this);
//    SpinBox_Port->setText("23");
    LineEdit_Port->setText("49300");
    PushButton_Connect = new QPushButton(this);
    PushButton_Connect->setText("Подключиться");
    PushButton_On = new QPushButton(this);
    PushButton_On->setText("On");
    PushButton_Off = new QPushButton(this);
    PushButton_Off->setText("Off");

    GridLayout_Control->addWidget(PushButton_Connect, 3, 0);
    GridLayout_Control->addWidget(Label_Addr, 0, 0);
    GridLayout_Control->addWidget(Label_Port, 1, 0);
    GridLayout_Control->addWidget(LineEdit_Addr, 0, 1);
    GridLayout_Control->addWidget(LineEdit_Port, 1, 1);
    GridLayout_Control->addWidget(PushButton_On, 3, 1);
    GridLayout_Control->addWidget(PushButton_Off, 3, 2);
    // =======================================Панель индикации======================================================
    GroupBox_Indication = new QGroupBox(this);
    GroupBox_Indication->setTitle("Индикация");

    GridLayout_Indication = new QGridLayout(GroupBox_Indication);

    GridLayout_CentralWidget = new QGridLayout();
    GridLayout_CentralWidget->addWidget(GroupBox_Control, 0, 0);
    GridLayout_CentralWidget->addWidget(GroupBox_Indication, 1, 0);

    Widget_Central = new QWidget(this);
    Widget_Central->setLayout(GridLayout_CentralWidget);
    setCentralWidget(Widget_Central);

//    connect(Client, SIGNAL(sendMessage(QString)), this, SLOT(ReceiveMessage(QString)));
    connect(Client, SIGNAL(sendMessage(QByteArray)), this, SLOT(ParseMessage(QByteArray)));
    connect(PushButton_Connect, SIGNAL(clicked()), this, SLOT(ConnectToServ()));
    connect(PushButton_On, SIGNAL(clicked()), this, SLOT(IndicationOn()));
    connect(PushButton_Off, SIGNAL(clicked()), this, SLOT(IndicationOff()));

}
void MainWindow::ConnectToServ()
{
    InData.clear();
    Client->slotConnectToServ(LineEdit_Addr->text(), LineEdit_Port->text().toUInt());
}


// ==================================================================================================================
void MainWindow::ReceiveMessage(QString message)
{
    if(message.size() == 0) return;
    message.replace("\r", "");
    outSplitMassiv = message.split('\n');
    if(outSplitMassiv.size() == 1)
    {
        qDebug() << "Конец строки не найден, ожидание новой информации";
        allMassiv.append(outSplitMassiv[0]);
    }else
    {
        if(allMassiv.size() != 0)
        {
            allMassiv.append(outSplitMassiv[0]);
            outSplitMassiv[0] = allMassiv;
            allMassiv.clear();
        }
        outSplitMassiv.removeOne("");
        for(int i = 0; i < outSplitMassiv.size(); i++)
        {
            int a = outSplitMassiv[i].indexOf("\"");
            if (a < 0)
            {
                inSplitMassiv = outSplitMassiv[i].split(' ');
            }else
            {
                QStringList textSplit = outSplitMassiv[i].split(("\""));
                textSplit.removeLast();
                inSplitMassiv = textSplit[0].split(' ');
                inSplitMassiv.removeLast();
                inSplitMassiv.append(textSplit[1]);
            }

            if(flagConnected == 0)
            {
                if (inSplitMassiv[0] == "INFORMATION_DISPLAY_SERVER")
                {
                    qDebug()<<"INFORMATION_DISPLAY_SERVER";
                    flagConnected = 1;
                }else
                {
                    qDebug() << "Error! Server is not connected!";
                }
            }else
            {
                MSGselector(inSplitMassiv);
            }
        }
        inSplitMassiv.clear();
        outSplitMassiv.clear();
        return;
    }
}
// ==================================================================================================================
void MainWindow::ParseMessage(QByteArray Data)
{
  InData += Data;
  int Index;
  while ((Index = InData.indexOf("\r\n")) >= 0)
  {
    QByteArray Tmp = InData.mid(0, Index);
    InData.remove(0, Index + 2);
    QString Msg = QString::fromUtf8(Tmp).trimmed();
    QString QuoteMsg;
    int QuoteIndex = Msg.indexOf("\"");
    if (QuoteIndex >= 0)
    {
      QuoteMsg = Msg.mid(QuoteIndex);
      Msg.remove(QuoteIndex, Msg.size());
      QuoteMsg.remove(0, 1);
      if (!QuoteMsg.size()) continue;
      if (QuoteMsg.right(1) != "\"") continue;
      QuoteMsg.chop(1);
    }
    QStringList MsgList = Msg.split(" ", QString::SkipEmptyParts);
    for (int i = 0; i < MsgList.size(); i++) MsgList[i] = MsgList[i].trimmed();
    if (QuoteMsg.size()) MsgList.append(QuoteMsg);
    MSGselector(MsgList);
  }
}
// ==================================================================================================================
void MainWindow::MSGPlace(QStringList message)
{
    bool flagPozition;
    int row, column,rowSpan, columnSpan;
    Qt::Alignment alignment = 0;
    bool ok;
    if(message.size() < 4) return;
    row = message[2].toUInt(&ok);
    if(!ok) return;
    column = message[3].toUInt(&ok);
    if(!ok) return;

    if (message.size() > 4 && message[4].size() > 5)
    {
        flagPozition = false;
        for(int i = 4; i < message.size(); i++)
        {
            if(message[i] == "AlignLeft") alignment = alignment | Qt::AlignLeft;
            else if (message[i] == "AlignRight") alignment = alignment | Qt::AlignRight;
            else if (message[i] == "AlignHCenter") alignment = alignment | Qt::AlignHCenter;
            else if (message[i] == "AlignJustify") alignment = alignment | Qt::AlignJustify;
            else if (message[i] == "AlignTop") alignment = alignment | Qt::AlignTop;
            else if (message[i] == "AlignBottom") alignment = alignment | Qt::AlignBottom;
            else if (message[i] == "AlignVCenter") alignment = alignment | Qt::AlignVCenter;
        }
    }else if (message.size() >= 6)
    {
        flagPozition = true;
        rowSpan = message[4].toUInt(&ok);
        if(!ok) return;
        columnSpan = message[5].toUInt(&ok);
        if(!ok) return;
        if(message.size() > 6)
        {
            for(int i = 6; i < message.size(); i++)
            {
                if(message[i] == "AlignLeft") alignment = alignment | Qt::AlignLeft;
                else if (message[i] == "AlignRight") alignment = alignment | Qt::AlignRight;
                else if (message[i] == "AlignHCenter") alignment = alignment | Qt::AlignHCenter;
                else if (message[i] == "AlignJustify") alignment = alignment | Qt::AlignJustify;
                else if (message[i] == "AlignTop") alignment = alignment | Qt::AlignTop;
                else if (message[i] == "AlignBottom") alignment = alignment | Qt::AlignBottom;
                else if (message[i] == "AlignVCenter") alignment = alignment | Qt::AlignVCenter;
            }
        }
    }
    if(message[1].contains("STRING", Qt::CaseInsensitive))
    {
        if (BIObjectName.indexOf(message[0]) != -1) return;
        else BIObjectName.append(message[0]);

        BIString *Obj = new BIString;
        BIObjects.append(Obj);

        if(!flagPozition) GridLayout_Indication->addWidget(Obj, row, column, alignment);
        else GridLayout_Indication->addWidget(Obj, row, column, rowSpan, columnSpan, alignment);
    }else if(message[1].contains("LED", Qt::CaseInsensitive))
    {
        if (BIObjectName.indexOf(message[0]) != -1) return;
        else BIObjectName.append(message[0]);

        BILED *Obj = new BILED;
        BIObjects.append(Obj);

        if(!flagPozition) GridLayout_Indication->addWidget(Obj, row, column, alignment);
        else GridLayout_Indication->addWidget(Obj, row, column, rowSpan, columnSpan, alignment);
    }else if(message[1].contains("PANEL", Qt::CaseInsensitive))
    {
        if (BIObjectName.indexOf(message[0]) != -1) return;
        else BIObjectName.append(message[0]);

        BIPanel *Obj = new BIPanel;
        BIObjects.append(Obj);

        if(!flagPozition) GridLayout_Indication->addWidget(Obj, row, column, alignment);
        else GridLayout_Indication->addWidget(Obj, row, column, rowSpan, columnSpan, alignment);
    }
}
// ==================================================================================================================
void MainWindow::MSGSubPlace(QStringList message)
{
    bool flagPozition;
    int row, column,rowSpan, columnSpan;
    Qt::Alignment alignment = 0;
    bool ok;
    QGridLayout *GridLayout;
    if(message.size() < 5) return;
    row = message[3].toUInt(&ok);
    if(!ok) return;
    column = message[4].toUInt(&ok);
    if(!ok) return;

    if (message.size() > 5 && message[5].size() > 6)
    {
        flagPozition = false;
        for(int i = 5; i < message.size(); i++)
        {
            if(message[i] == "AlignLeft") alignment = alignment | Qt::AlignLeft;
            else if (message[i] == "AlignRight") alignment = alignment | Qt::AlignRight;
            else if (message[i] == "AlignHCenter") alignment = alignment | Qt::AlignHCenter;
            else if (message[i] == "AlignJustify") alignment = alignment | Qt::AlignJustify;
            else if (message[i] == "AlignTop") alignment = alignment | Qt::AlignTop;
            else if (message[i] == "AlignBottom") alignment = alignment | Qt::AlignBottom;
            else if (message[i] == "AlignVCenter") alignment = alignment | Qt::AlignVCenter;
        }
    }else if (message.size() >= 7)
    {
        flagPozition = true;
        rowSpan = message[5].toUInt(&ok);
        if(!ok) return;
        columnSpan = message[6].toUInt(&ok);
        if(!ok) return;
        if(message.size() > 7)
        {
            for(int i = 7; i < message.size(); i++)

                if(message[i] == "AlignLeft") alignment = alignment | Qt::AlignLeft;
                else if (message[i] == "AlignRight") alignment = alignment | Qt::AlignRight;
                else if (message[i] == "AlignHCenter") alignment = alignment | Qt::AlignHCenter;
                else if (message[i] == "AlignJustify") alignment = alignment | Qt::AlignJustify;
                else if (message[i] == "AlignTop") alignment = alignment | Qt::AlignTop;
                else if (message[i] == "AlignBottom") alignment = alignment | Qt::AlignBottom;
                else if (message[i] == "AlignVCenter") alignment = alignment | Qt::AlignVCenter;
        }
    }
    int ind = BIObjectName.indexOf(message[0]);
    if (ind < 0) return;
    GridLayout = BIObjects[ind]->GetGrid();
    if(GridLayout == NULL) return;
    else
    {
        message.removeAt(0);
        if(message[1].contains("STRING", Qt::CaseInsensitive))
        {
            if (BIObjectName.indexOf(message[0]) != -1) return;
            else BIObjectName.append(message[0]);

            BIString *Obj = new BIString;
            BIObjects.append(Obj);
            if(!flagPozition) GridLayout->addWidget(Obj, row, column, alignment);
            else GridLayout->addWidget(Obj, row, column, rowSpan, columnSpan, alignment);
        }else if(message[1].contains("LED", Qt::CaseInsensitive))
        {
            if (BIObjectName.indexOf(message[0]) != -1) return;
            else BIObjectName.append(message[0]);

            BILED *Obj = new BILED;
            BIObjects.append(Obj);

            if(!flagPozition) GridLayout->addWidget(Obj, row, column, alignment);
            else GridLayout->addWidget(Obj, row, column, rowSpan, columnSpan, alignment);
        }else if(message[1].contains("PANEL", Qt::CaseInsensitive))

        {
            if (BIObjectName.indexOf(message[0]) != -1) return;
            else BIObjectName.append(message[0]);

            BIPanel *Obj = new BIPanel;
            BIObjects.append(Obj);

            if(!flagPozition) GridLayout->addWidget(Obj, row, column, alignment);
            else GridLayout->addWidget(Obj, row, column, rowSpan, columnSpan, alignment);
        }
    }



}
// ==================================================================================================================
void MainWindow::MSGColor(QStringList message)
{
    if(message.size() != 4 && message.size() != 5) return;
    QColor variableColor;
    int ind = BIObjectName.indexOf(message[0]);
    if (ind < 0) return;
    int r, g, b, a;
    bool ok;
    r = message[1].toUInt(&ok);
    if(!ok) return;
    g = message[2].toUInt(&ok);
    if(!ok) return;
    b = message[3].toUInt(&ok);
    if(!ok) return;
    if(message.size() == 4)
    {
        variableColor.setRgb(r,g,b);
        BIObjects[ind]->Color(variableColor);
    }else
    {
        a = message[4].toUInt(&ok);
        if(!ok) return;
        variableColor.setRgb(r, g, b, a);
        BIObjects[ind]->Color(variableColor);
    }


}
// ==================================================================================================================
void MainWindow::MSGSize(QStringList message)
{
    int size;
    bool ok;
    if(message.size() != 2) return;
    int ind = BIObjectName.indexOf(message[0]);
    if (ind < 0) return;
    size = message[1].toUInt(&ok);
    if(!ok) return;
    BIObjects[ind]->Size(size);
}
// ==================================================================================================================
void MainWindow::MSGText(QStringList message)
{
    if(message.size() != 2) return;
    int ind = BIObjectName.indexOf(message[0]);
    if (ind < 0) return;
    BIObjects[ind]->Text(message[1]);

}
// ==================================================================================================================
void MainWindow::MSGAfterLight(QStringList message)
{
    int time;
    bool ok;
    if(message.size() != 2) return;
    int ind = BIObjectName.indexOf(message[0]);
    if (ind < 0) return;
    time = message[1].toUInt(&ok);
    if(!ok) return;
    BIObjects[ind]->AfterLight(time);
}
// ==================================================================================================================
void MainWindow::MSGClear()
{
    QGridLayout *GridLayout;
    fixWindow = false;
    BIObjectName.clear();
    for(int i = (BIObjects.size() - 1); i >= 0; i--)
    {
        GridLayout = BIObjects[i]->GetGrid();
        if(GridLayout == NULL) delete BIObjects[i];
        else {
            delete BIObjects[i];
        }
    }
    BIObjects.clear();
}
// ==================================================================================================================
void MainWindow::MSGFixWindow()
{
    fixWindow = true;
}

// ==================================================================================================================
void MainWindow::MSGselector(QStringList message)
{
    if(message.size() == 0) return;
    QString CMD = message[0];
    message.removeAt(0);
    if(CMD.contains("PLACE", Qt::CaseInsensitive))
    {
        MSGPlace(message);
    }
    if(CMD.contains("SUBPLACE", Qt::CaseInsensitive))
    {
        MSGSubPlace(message);
    }
    else if (CMD.contains("COLOR", Qt::CaseInsensitive))
    {
        MSGColor(message);
    }
    else if (CMD.contains("TEXT", Qt::CaseInsensitive))
    {
        MSGText(message);
    }
    else if (CMD.contains("SIZE", Qt::CaseInsensitive))
    {
        MSGSize(message);
    }
    else if (CMD.contains("AFTERLIGHT", Qt::CaseInsensitive))
    {
        MSGAfterLight(message);
    }
    else if (CMD.contains("CLEAR", Qt::CaseInsensitive))
    {
        MSGClear();
    }
    else if (CMD.contains("FIXWINDOW", Qt::CaseInsensitive))
    {
        MSGFixWindow();
    }
}



// ==================================================================================================================
// ============================================= ДЕСТРУКТОР =========================================================
// ==================================================================================================================
MainWindow::~MainWindow()
{
    delete ui;
}
//******************************************************************************
bool MainWindow::event(QEvent *ev)
{
    if(ev->type() == QEvent::LayoutRequest)
    {
        if(!fixWindow) setFixedSize(sizeHint());
    }
    return QMainWindow::event(ev);
}
//******************************************************************************
void MainWindow::IndicationOn()
{
  if (Client->mTcpSocket != NULL)
  {
    qDebug() << "-------------------- On";

    QString Str = "SIG_ON";
    Str += "\r\n";
    Client->mTcpSocket->write(Str.toUtf8());
  }
}

void MainWindow::IndicationOff()
{
  if (Client->mTcpSocket != NULL)
  {
    qDebug() << "-------------------- Off";

    QString Str = "SIG_OFF";
    Str += "\r\n";
    Client->mTcpSocket->write(Str.toUtf8());
  }
}

