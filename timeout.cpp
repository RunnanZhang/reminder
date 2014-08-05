#include "timeout.h"
#include "ui_timeout.h"

TimeOut::TimeOut(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TimeOut)
{
    ui->setupUi(this);
}

TimeOut::~TimeOut()
{
    delete ui;
}

void TimeOut::setName(QString name)
{
    ui->nameEdit->setText(name);
}

void TimeOut::setLCD(QDateTime time)
{
    ui->lcdNumber->setDigitCount(19);
    ui->lcdNumber->display(time.toString("yyyy-MM-dd hh:mm:ss"));
    ui->lcdNumber->setSegmentStyle(QLCDNumber::Flat);
}

void TimeOut::setReminder(QString str)
{
    ui->remindEdit->setDocument(new QTextDocument(str));
}
