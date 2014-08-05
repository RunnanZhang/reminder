#ifndef REMINDER_H
#define REMINDER_H

#include "timeout.h"
#include <QMainWindow>
#include <QDateTime>
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

private:
    Ui::Reminder *ui;
    TimeOut* m_TimeOutWidget;
    QTimer* m_timer;
    QDateTime m_Time;
    qint64 m_Hours;
    QString m_Name;
    QString m_Reminder;
};

#endif // REMINDER_H
