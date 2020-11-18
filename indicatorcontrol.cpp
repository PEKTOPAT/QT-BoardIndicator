//******************************************************************************
#include "indicatorcontrol.h"
//******************************************************************************

IndicatorControl::IndicatorControl()
{
    this->setWindowTitle("Telnet client");
    setMinimumSize(200,200);
    flagConnected = 0;
    GridLayout_Form = new QGridLayout(this);
}
//******************************************************************************
void IndicatorControl::ReceiveMessage(QString message)
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
                    qDebug()<<"<***Server is connected!***>";
                    flagConnected = 1;
                }else
                {
                    qDebug() << "Error! Server is not connected!";
                }
            }else
            {
//                qDebug() << "inSplitMassiv" << inSplitMassiv;
                MSGselector(inSplitMassiv);
            }
        }
        inSplitMassiv.clear();
        outSplitMassiv.clear();
        return;
    }
}


void IndicatorControl::MSGPlace(QStringList message)
{
    this->show();
    bool flagPozition;
    int row, column,rowSpan, columnSpan;
    bool ok;
    if(message.size() == 4)
    {
        flagPozition = true;
        row = message[2].toUInt(&ok);
        if(!ok) return;
        column = message[3].toUInt(&ok);
        if(!ok) return;
    }else if (message.size() == 6)
    {
        flagPozition = false;
        row = message[2].toUInt(&ok);
        if(!ok) return;
        column = message[3].toUInt(&ok);
        if(!ok) return;
        rowSpan = message[4].toUInt(&ok);
        if(!ok) return;
        columnSpan = message[5].toUInt(&ok);
        if(!ok) return;

    }else return;
    if(message[1] == "STRING")
    {
        if (BIObjectName.indexOf(message[0]) != -1) return;
        else BIObjectName.append(message[0]);

        BIString *Obj = new BIString;
        BIObjects.append(Obj);

        if(flagPozition) GridLayout_Form->addWidget(Obj, row, column);
        else GridLayout_Form->addWidget(Obj, row, column, rowSpan, columnSpan);

    }else if(message[1] == "DIOD")
    {
        if (BIObjectName.indexOf(message[0]) != -1) return;
        else BIObjectName.append(message[0]);

        BIDiod *Obj = new BIDiod;
        BIObjects.append(Obj);

        if(flagPozition) GridLayout_Form->addWidget(Obj, row, column);
        else GridLayout_Form->addWidget(Obj, row, column, rowSpan, columnSpan);
    }
}

void IndicatorControl::MSGColor(QStringList message)
{
    this->show();
    if(message.size() != 4) return;
    int ind = BIObjectName.indexOf(message[0]);
    if (ind < 0) return;
    int r, g, b;
    bool ok;
    r = message[1].toUInt(&ok);
    if(!ok) return;
    g = message[2].toUInt(&ok);
    if(!ok) return;
    b = message[3].toUInt(&ok);
    if(!ok) return;
    BIObjects[ind]->Color(r, g, b);
}

void IndicatorControl::MSGText(QStringList message)
{
    this->show();
    if(message.size() != 2) return;
    int ind = BIObjectName.indexOf(message[0]);
    if (ind < 0) return;
    BIObjects[ind]->Text(message[1]);

}

void IndicatorControl::MSGDiod(QStringList message)
{
    this->show();
    qDebug() << "message -" << message;
    if(message.size() != 1) return;
    int ind = BIObjectName.indexOf(message[0]);
    if (ind < 0) return;
    BIObjects[ind]->Diod();
}

//******************************************************************************
void IndicatorControl::MSGselector(QStringList message)
{
    if(message.size() == 0) return;
    QString CMD = message[0];
    message.removeAt(0);
    if(CMD == "PLACE")
    {
        MSGPlace(message);
    }
    else if (CMD == "COLOR")
    {
        MSGColor(message);
    }
    else if (CMD == "TEXT")
    {
        MSGText(message);
    }
}
