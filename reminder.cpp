#include "reminder.h"
#include "ui_reminder.h"
#include <QCalendarWidget>
#include <QFile>
#include <QDataStream>
#include <QMessageBox>
#include <QAction>
#include <QMenu>

Reminder::Reminder(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Reminder),
    m_Hours(0),
    m_Name(QString()),
    m_Reminder(QString())
{
    ui->setupUi(this);
    ui->dateTimeEdit->setCalendarPopup(true);
    ui->hourSpin->setSuffix(" Hours");

    QFile file("data.dat");
    if(file.open(QIODevice::ReadOnly))
    {
        ui->dateTimeEdit->setDate(QDate::currentDate());
        QDataStream in(&file);
        in >> m_Name >> m_Time >> m_Hours >> m_Reminder;
        file.close();
    }
    else
    {
        m_Time = QDateTime::currentDateTime();
    }
    ui->nameEdit->setText(m_Name);
    ui->dateTimeEdit->setDateTime(m_Time);
    ui->hourSpin->setValue(m_Hours);
    ui->remindEdit->setDocument(new QTextDocument(m_Reminder));

    m_timer = new QTimer(this);
    QObject::connect(m_timer, SIGNAL(timeout()), this, SLOT(checkTime()));
    m_timer->start(1000);

    //托盘图标
    QSystemTrayIcon* trayIcon = new QSystemTrayIcon(this);

    QAction* quitAction = new QAction(tr("&Quit"), this);
    QAction* restoreAction = new QAction(tr("&Show"), this);
    QObject::connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));
    QObject::connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    QMenu* trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);
    trayIcon->setContextMenu(trayIconMenu);
    QIcon *icon = new QIcon(":/images/heart.svg");
    trayIcon->setIcon(*icon);
    setWindowIcon(*icon);
    trayIcon->show();
}

Reminder::~Reminder()
{
    delete ui;
}

void Reminder::checkTime()
{
    QDateTime dateTime = m_Time.addSecs(qint64(m_Hours));
    qWarning("now:%d", QDateTime::currentDateTime().toTime_t());
    qWarning("target:%d", dateTime.toTime_t());
    if(dateTime.toTime_t() == QDateTime::currentDateTime().toTime_t())
    {
        m_timer->stop();
        m_TimeOutWidget = new TimeOut();
        m_TimeOutWidget->setAttribute(Qt::WA_DeleteOnClose);
        m_TimeOutWidget->setName(m_Name);
        m_TimeOutWidget->setLCD(dateTime);
        m_TimeOutWidget->setReminder(m_Reminder);
        m_TimeOutWidget->setWindowFlags(Qt::X11BypassWindowManagerHint | Qt::WindowStaysOnTopHint);
        m_TimeOutWidget->show();
    }
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
