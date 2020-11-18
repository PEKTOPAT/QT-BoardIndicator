#ifndef BOARDINDIATOROBJECTS_H
#define BOARDINDIATOROBJECTS_H

#include <ledindicator.h>

#include <QString>
#include <QLabel>
#include <QGridLayout>
#include <QGroupBox>
#include <QTimer>

class BoardIndiatorObjects
{
public:
    BoardIndiatorObjects();
    virtual ~BoardIndiatorObjects() {}
    virtual void Text(QString str);
    virtual void Color(QColor variableColor);
    virtual void Size(int size);
    virtual void AfterLight (int time);
    virtual QGridLayout* GetGrid() { return NULL; }
};

// ==================================================================================================================
class BIString: public QLabel, public BoardIndiatorObjects
{
public:
    void Text(QString str);

    void Color(QColor variableColor);

    void Size(int size);
};
// ==================================================================================================================
class BILED: public LedIndicator, public BoardIndiatorObjects
{
    Q_OBJECT
public:
    BILED();
    void Color(QColor variableColor);

    void Size(int size);

    void AfterLight(int time);
private slots:
    void slotOffLED();

private:
    QTimer Timer;
};
// ==================================================================================================================
class BIPanel: public QGroupBox, public BoardIndiatorObjects
{
public:
    BIPanel();

    void Text(QString str);
    void Size(int size);
    QGridLayout* GetGrid();

private:
    QGridLayout GridLayout;


};
// ==================================================================================================================

#endif // BOARDINDIATOROBJECTS_H
