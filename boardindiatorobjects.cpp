#include "boardindiatorobjects.h"
#include <QDebug>



BoardIndiatorObjects::BoardIndiatorObjects() {}

void BoardIndiatorObjects::Text(QString str)
{
    QString TStr = str;
}

void BoardIndiatorObjects::Color(QColor variableColor)
{
    QString TStr = QString::number(variableColor.red()) +
            QString::number(variableColor.green()) +
            QString::number(variableColor.blue());
}

void BoardIndiatorObjects::Size(int size)
{
    int Tsize = size;
    Tsize++;
}


void BoardIndiatorObjects::AfterLight(int time)
{
    int Tsize = time;
    Tsize++;
}

// ==================================================================================================================
void BIString::Text(QString str)
{
    setText(str);
}
// ==================================================================================================================
void BIString::Color(QColor variableColor)
{

    setStyleSheet(QString("color: rgba(%1, %2, %3)").arg(variableColor.red()).arg(variableColor.green()).
                  arg(variableColor.blue()).arg(variableColor.alpha()));
}
// ==================================================================================================================
void BIString::Size(int size)
{
    QFont objectfont = font();
    objectfont.setPixelSize(size);
    setFont(objectfont);
}
// ==================================================================================================================
BILED::BILED()
{
    connect(&Timer, SIGNAL(timeout()), this, SLOT(slotOffLED()));
}
// ==================================================================================================================
void BILED::Color(QColor variableColor)
{
    if(Timer.isActive()) Timer.start();
    setOnColor(variableColor);
}

// ==================================================================================================================
void BILED::Size(int size)
{
    setLedSize(size);
}
// ==================================================================================================================
void BILED::AfterLight(int time)
{
    Timer.setInterval(time);
    Timer.start();
}
// ==================================================================================================================
void BILED::slotOffLED()
{
    QColor variableColor;
    variableColor.setRgb(255,0,0);
    setOffColor(variableColor);
}
// ==================================================================================================================
BIPanel::BIPanel():GridLayout(this)
{

}
// ==================================================================================================================
void BIPanel::Text(QString str)
{
    setTitle(str);
}
// ==================================================================================================================
void BIPanel::Size(int size)
{
    setStyleSheet(QString("QGroupBox { font-size: %1px } ").arg(size));
}
// ==================================================================================================================
QGridLayout* BIPanel::GetGrid()
{
    return &GridLayout;
}
// ==================================================================================================================
