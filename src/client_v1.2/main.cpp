#include "mywidget.h"
#include "client.h"
#include <QApplication>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    myWidget w;
    w.show();

    return a.exec();
}
