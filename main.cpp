#include <QApplication>

#include "windows/mainwindow.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setApplicationName("AVR-Sensors");
    a.setApplicationVersion("1.2.0.0");
    a.setOrganizationName("Łukasz \"Kuszki\" Dróżdż");
    a.setOrganizationDomain("https://github.com/Kuszki/AVR-Sensors");

    MainWindow w;

    w.show();

    return a.exec();
}
