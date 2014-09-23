#include "timeout.h"
#include "ui_timeout.h"

TimeOut::TimeOut(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TimeOut)
{
    ui->setupUi(this);
    setWindowFlags(Qt::X11BypassWindowManagerHint | Qt::WindowStaysOnTopHint);
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

void TimeOut::setInfo(QString str)
{
    ui->InfoEdit->setDocument(new QTextDocument(str));
}

void TimeOut::setReminder(QString str)
{
    ui->remindEdit->setText(str);
}
