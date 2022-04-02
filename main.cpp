#ifdef ONLY_CONSOLE_APP
#include "Parse.h"
#include <QDebug>
#else
#include "Panel.h"
#endif

#include <QApplication>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
#ifdef ONLY_CONSOLE_APP
    QString gfwPath = ":/GFW/gfwlist.txt";
    QString errorCode;
    Convert(gfwPath, errorCode);
    qDebug() << errorCode;
    return 0;
#else
    Panel w;
    w.show();
    return a.exec();
#endif

}
