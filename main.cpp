#include "reminder.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    if (!QSystemTrayIcon::isSystemTrayAvailable())
    {
        return 1;
    }
    QApplication::setQuitOnLastWindowClosed(false);

    Reminder w;
    w.show();

    return a.exec();
}
