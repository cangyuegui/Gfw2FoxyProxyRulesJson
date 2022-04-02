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
    QString errorCode;
    Convert(gfwPath, errorCode);
    ui->label->setText(errorCode);
}
