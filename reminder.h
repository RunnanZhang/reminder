#ifndef REMINDER_H
#define REMINDER_H

#include "listmodel.h"
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
    void on_addBtn_clicked();
    void on_delBtn_clicked();
    void checkTime();
    void iconActivated(QSystemTrayIcon::ActivationReason);
    //when double clicked QListView
    void viewClicked(const QModelIndex &index);

protected:
    virtual void closeEvent(QCloseEvent * event);

private:
    void writeModelInfo();

private:
    Ui::Reminder *ui;
    TimeOut* m_TimeOutWidget;
    QSystemTrayIcon* m_trayIcon;
    QTimer* m_timer;
    Listmodel* model;
};

#endif // REMINDER_H
