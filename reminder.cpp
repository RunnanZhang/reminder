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

    //有文件信息则读取，没有则初始化空的model
    QFile file("data.dat");
    if(file.open(QIODevice::ReadOnly))
    {
        ui->dateTimeEdit->setDate(QDate::currentDate());
        QList<ModelData> list;
        QDataStream in(&file);
        in >> list;
        file.close();
        int count = list.count();
        model = new Listmodel(count, this);
        QVariant var;
        for(int i = 0; i < count; ++i)
        {
            var.setValue(list[i]);
            model->setData(model->index(i), var, Qt::UserRole);
        }
    }
    else
    {
        model = new Listmodel(0, this);
    }
    ui->listView->setModel(model);
    ui->listView->setSelectionMode(QAbstractItemView::SingleSelection);
    QObject::connect(ui->listView, SIGNAL(doubleClicked(const QModelIndex&)),
                     this, SLOT(viewClicked(const QModelIndex &)));
    QObject::connect(ui->listView, SIGNAL(clicked(const QModelIndex&)),
                     this, SLOT(viewClicked(const QModelIndex &)));

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
    writeModelInfo();
    delete ui;
}

void Reminder::writeModelInfo()
{
    QFile file("data.dat");
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);
    out << model->m_list;
    file.close();
}

void Reminder::on_addBtn_clicked()
{
    int count = model->rowCount();
    model->insertRow(count);
}

void Reminder::on_delBtn_clicked()
{
    if(ui->listView->currentIndex().isValid())
    {
        int row = ui->listView->currentIndex().row();
        model->removeRow(row);
    }
}

void Reminder::checkTime()
{
    int count = model->rowCount();
    if(count <= 0)
    {
        return;
    }
    QDateTime dateTime;
    ModelData data;
    QVariant var = QVariant();
    for(int i = 0; i < count; ++i)
    {
        var = model->data(model->index(i), Qt::UserRole);
        data = var.value<ModelData>();
        dateTime = data.dateTime.addSecs(qint64(data.hours*3600));
        if(dateTime.toTime_t() == QDateTime::currentDateTime().toTime_t())
        {
            m_TimeOutWidget = new TimeOut();
            m_TimeOutWidget->setAttribute(Qt::WA_DeleteOnClose);
            m_TimeOutWidget->setName(data.name);
            m_TimeOutWidget->setLCD(data.dateTime);
            m_TimeOutWidget->setReminder(data.reminder);
            m_TimeOutWidget->setInfo(data.infomation);
            m_TimeOutWidget->show();
            QApplication::alert(this);
        }
    }
    if(!ui->listView->currentIndex().isValid())
    {
        return;
    }
    var = model->data(ui->listView->currentIndex(), Qt::UserRole);
    data = var.value<ModelData>();
    dateTime = data.dateTime.addSecs(qint64(data.hours*3600));
    ui->lcdNumber->setDigitCount(9);
    ui->lcdNumber->display(QString::number(QDateTime::currentDateTime().secsTo(dateTime)));
    ui->lcdNumber->setSegmentStyle(QLCDNumber::Flat);
}

void Reminder::on_applyBtn_clicked()
{
    QVariant var;
    ModelData currentData;
    currentData.name = ui->nameEdit->text();
    currentData.hours = ui->hourSpin->value();
    currentData.dateTime = ui->dateTimeEdit->dateTime();
    currentData.reminder = ui->remindEdit->text();
    currentData.infomation = ui->InfoEdit->document()->toPlainText();
    var.setValue(currentData);
    model->setData(ui->listView->currentIndex(), var, Qt::UserRole);
}

void Reminder::viewClicked(const QModelIndex &index)
{
    QVariant var = model->data(index, Qt::UserRole);
    ModelData currentData = var.value<ModelData>();
    ui->nameEdit->setText(currentData.name);
    ui->hourSpin->setValue(currentData.hours);
    ui->dateTimeEdit->setDateTime(currentData.dateTime);
    ui->remindEdit->setText(currentData.reminder);
    ui->InfoEdit->setDocument(new QTextDocument(currentData.infomation));
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
