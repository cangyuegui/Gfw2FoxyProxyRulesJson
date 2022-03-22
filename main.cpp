#include "Panel.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Panel w;
    w.show();
    return a.exec();
}
