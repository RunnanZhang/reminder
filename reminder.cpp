#include "reminder.h"
#include "ui_reminder.h"
#include <QCalendarWidget>
#include <QFile>
#include <QDataStream>
#include <QMessageBox>
#include <QAction>
#include <QMenu>
#include <QCloseEvent>

Reminder::Reminder(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Reminder)
{
    ui->setupUi(this);
    ui->dateTimeEdit->setCalendarPopup(true);
    ui->hourSpin->setSuffix(" Hours");
    ui->hourSpin->setSingleStep(1.0);

    //有文件信息则读取，没有则初始化
    QFile file("data.dat");
    if(file.open(QIODevice::ReadOnly))
    {
        ui->dateTimeEdit->setDate(QDate::currentDate());
        QDataStream in(&file);
        //in >> m_Name >> m_Time >> m_Hours >> m_Reminder;
        file.close();
    }
    else
    {
        model = new Listmodel(0, this);
    }
    ui->nameEdit->setText(m_Name);
    ui->dateTimeEdit->setDateTime(m_Time);
    ui->hourSpin->setValue(m_Hours);
    ui->remindEdit->setDocument(new QTextDocument(m_Reminder));

    m_timer = new QTimer(this);
    QObject::connect(m_timer, SIGNAL(timeout()), this, SLOT(checkTime()));
    m_timer->start(1000);

    //托盘图标
    m_trayIcon = new QSystemTrayIcon(this);
    QObject::connect(m_trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
               this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

    QAction* quitAction = new QAction(tr("&Quit"), this);
    QAction* showAction = new QAction(tr("&Show"), this);
    QObject::connect(showAction, SIGNAL(triggered()), this, SLOT(showNormal()));
    QObject::connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    QMenu* trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(showAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);
    m_trayIcon->setContextMenu(trayIconMenu);
    QIcon icon(":/images/heart.svg");
    m_trayIcon->setIcon(icon);
    setWindowIcon(icon);
    m_trayIcon->setToolTip("联合站设备维护保养提示器");
    m_trayIcon->show();
}

Reminder::~Reminder()
{
    delete ui;
}

void Reminder::checkTime()
{
    QDateTime dateTime = m_Time.addSecs(qint64(m_Hours*3600));
    if(dateTime.toTime_t() == QDateTime::currentDateTime().toTime_t())
    {
        m_timer->stop();
        m_TimeOutWidget = new TimeOut();
        m_TimeOutWidget->setAttribute(Qt::WA_DeleteOnClose);
        m_TimeOutWidget->setName(m_Name);
        m_TimeOutWidget->setLCD(dateTime);
        m_TimeOutWidget->setReminder(m_Reminder);
        m_TimeOutWidget->show();
        QApplication::alert(this);
    }
    ui->lcdNumber->setDigitCount(9);
    ui->lcdNumber->display(QString::number(QDateTime::currentDateTime().secsTo(dateTime)));
    ui->lcdNumber->setSegmentStyle(QLCDNumber::Flat);
}

void Reminder::on_applyBtn_clicked()
{
    QFile file("data.dat");
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);

    m_Name = ui->nameEdit->text();
    out << m_Name;

    m_Time = ui->dateTimeEdit->dateTime();
    out<<m_Time;

    m_Hours = ui->hourSpin->value();
    out<<m_Hours;

    m_Reminder = ui->remindEdit->document()->toPlainText();
    out<<m_Reminder;
    file.close();
    if(!m_timer->isActive())
    {
        m_timer->start(1000);
    }
}

void Reminder::closeEvent(QCloseEvent * event)
{
    if (m_trayIcon->isVisible())
    {
        m_trayIcon->showMessage(tr("提示"), tr("程序已最小化至托盘处"));
        hide();
        event->ignore();
    }
}

void Reminder::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if(reason == QSystemTrayIcon::DoubleClick)
    {
        showNormal();
    }
}
