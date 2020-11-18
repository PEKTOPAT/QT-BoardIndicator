#ifndef INDICATORCONTROL_H
#define INDICATORCONTROL_H
//******************************************************************************
#include <QWidget>
#include <QGridLayout>
#include <QTextEdit>
#include <QPushButton>
#include <QLineEdit>
#include <QDebug>
#include <QLabel>
#include <QWidget>
#include <boardindiatorobjects.h>
//******************************************************************************
class IndicatorControl: public QWidget
{
    Q_OBJECT
public:
    IndicatorControl();

    void MSGselector(QStringList message);

private:
    int flagConnected;
    QStringList outSplitMassiv;
    QStringList inSplitMassiv;
    QString allMassiv;

    QGridLayout *GridLayout_Form;
    QLabel *textLblCarry1;
    QLabel *textLblCarry2;
    QLabel *textLblCarry3;
    QLabel *textLblCarry4;
    QLabel *textLblCarry5;
    QLabel *textLblCarry6;
    QLineEdit *text;
    QLineEdit *text2;

    QStringList BIObjectName;
//    QList<QWidget*> BIWidjet;
    QList<BoardIndiatorObjects*> BIObjects;



    void MSGPlace(QStringList message);
    void MSGColor(QStringList message);
    void MSGText(QStringList message);
    void MSGDiod(QStringList message);

private slots:

public slots:
    void ReceiveMessage(QString message);


};
//******************************************************************************
#endif // INDICATORCONTROL_H
//******************************************************************************
