#ifndef TIMEOUT_H
#define TIMEOUT_H

#include <QWidget>
#include <QDateTime>

namespace Ui {
class TimeOut;
}

class TimeOut : public QWidget
{
    Q_OBJECT

public:
    explicit TimeOut(QWidget *parent = 0);
    ~TimeOut();
    void setName(QString);
    void setLCD(QDateTime);
    void setReminder(QString);

private:
    Ui::TimeOut *ui;
};

#endif // TIMEOUT_H
