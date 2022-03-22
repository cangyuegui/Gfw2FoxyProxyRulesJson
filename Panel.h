#ifndef PANEL_H
#define PANEL_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Panel; }
QT_END_NAMESPACE

class Panel : public QWidget
{
    Q_OBJECT

public:
    Panel(QWidget *parent = nullptr);
    ~Panel();

private slots:
    void on_convert_clicked();

private:
    Ui::Panel *ui;
};
#endif // PANEL_H
