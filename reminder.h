#ifndef REMINDER_H
#define REMINDER_H

#include <listmodel.h>
#include "timeout.h"
#include <QMainWindow>
#include <QTimer>
#include <QSystemTrayIcon>

namespace Ui {
class Reminder;
}

class Reminder : public QMainWindow
{
    Q_OBJECT

public:
    explicit Reminder(QWidget *parent = 0);
    ~Reminder();

private Q_SLOTS:
    void on_applyBtn_clicked();
    void checkTime();
    void iconActivated(QSystemTrayIcon::ActivationReason);

protected:
    virtual void closeEvent(QCloseEvent * event);

private:
    Ui::Reminder *ui;
    TimeOut* m_TimeOutWidget;
    QSystemTrayIcon* m_trayIcon;
    QTimer* m_timer;
};

#endif // REMINDER_H
