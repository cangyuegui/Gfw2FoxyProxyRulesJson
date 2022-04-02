#include "Panel.h"
#include "ui_Panel.h"

#include "Parse.h"

Panel::Panel(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Panel)
{
    ui->setupUi(this);
}

Panel::~Panel()
{
    delete ui;
}


void Panel::on_convert_clicked()
{
    QString gfwPath = ":/GFW/gfwlist.txt";
    QString urlContent;
    if (!GetGfwCode(gfwPath, urlContent))
    {
        ui->label->setText(QString("Cannot Parse File \"%1\"").arg(gfwPath));
        return;
    }

    QFile filetemp(qApp->applicationDirPath() + "/decordGFW.abp");
    filetemp.open(QFile::WriteOnly);
    filetemp.write(urlContent.toUtf8());
    filetemp.flush();
    filetemp.close();

    QFile addones(":/RES/add_ones.txt");
    addones.open(QFile::ReadOnly);

    urlContent += "\n";
    urlContent += addones.readAll();
    addones.close();

    QStringList urls = GetUrlList(urlContent);
    QJsonDocument jsonDoc = ToRule(urls);

    QString saveUrl;
    if (!SaveFileToJson(jsonDoc, saveUrl))
    {
        ui->label->setText(QString("Cannot Save File \"%1\"").arg(saveUrl));
        return;
    }

    ui->label->setText(QString("Success \"%1\"").arg(saveUrl));
#ifdef Q_OS_WINDOWS
    QStringList args;
    saveUrl.replace("/", "\\");
    args.append(QString("/select,\"%1\"").arg(saveUrl));

    QString fullExe = QString("explorer %1").arg(QString("/select,\"%1\"").arg(saveUrl));
    QProcess::execute(fullExe);
    //qDebug() << fullExe.toStdString().c_str();

#endif

}
