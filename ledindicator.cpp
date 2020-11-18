#include "ledindicator.h"
#include <QPainter>
#include <QDebug>

LedIndicator::LedIndicator(QWidget *parent):
    QWidget(parent)
{
    setFixedSize(28, 28);
    lit = false;
    ledOnColor=Qt::green;
    ledOffColor=Qt::red;
    ledOnPattern = Qt::SolidPattern;
    ledOffPattern = Qt::SolidPattern;
    flagSwitch = false;
    ledSize=20;
}
// ==================================================================================================================
void LedIndicator::paintEvent(QPaintEvent *) {
  QPainter p(this);
  //lit ?  p.setBrush(QBrush(ledOnColor, ledOnPattern)) : p.setBrush(QBrush(ledOffColor, ledOffPattern));
  if (flagSwitch)
  {
      p.setBrush(QBrush(ledOnColor));
      p.drawEllipse(0,0,ledSize,ledSize);
  }else{
      p.setBrush(QBrush(ledOffColor));
      p.drawEllipse(0,0,ledSize,ledSize);
  }
}
// ==================================================================================================================
void LedIndicator::switchLedIndicator() {
  lit = ! lit;
  repaint();
}
// ==================================================================================================================
void LedIndicator::setState(bool state)
{
    lit = state;
    repaint();
}
// ==================================================================================================================
void LedIndicator::toggle()
{
    lit = ! lit;
    repaint();
}
// ==================================================================================================================
void LedIndicator::setOnColor(QColor onColor)
{
    //qDebug() << "Включение диода";
    flagSwitch = true;
    ledOnColor=onColor;
    repaint();
}
// ==================================================================================================================
void LedIndicator::setOffColor(QColor offColor)
{
    //qDebug() << "Отключение диода";
    flagSwitch = false;
    ledOffColor=offColor;
    repaint();
}
// ==================================================================================================================
void LedIndicator::setOnPattern(Qt::BrushStyle onPattern)
{
    ledOnPattern=onPattern;
    repaint();
}
// ==================================================================================================================
void LedIndicator::setOffPattern(Qt::BrushStyle offPattern)
{
    ledOffPattern=offPattern;
    repaint();
}
// ==================================================================================================================
void LedIndicator::setLedSize(int size)
{
    ledSize=size;
    setFixedSize(8+size, 8+size);
    repaint();
}
