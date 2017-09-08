#include "ip_dialog.h"
#include<QGridLayout>
#include<QLineEdit>
#include<QLabel>
#include<QRegExpValidator>
#include<QPushButton>
#include<QRegExp>
IP_Dialog::IP_Dialog(QWidget *obj):QDialog(obj,Qt::WindowTitleHint | Qt::WindowSystemMenuHint)
{
    QGridLayout *layer = new QGridLayout(this);
    QPushButton *accept = new QPushButton("Accept");
    QPushButton *reject = new QPushButton("Cancle");
    ip_line = new QLineEdit(this);
    connect(accept,SIGNAL(clicked(bool)),SLOT(accept()));
    connect(reject,SIGNAL(clicked(bool)),SLOT(reject()));
    QString iprange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    QRegExp reg("^" + iprange+
                "\\." + iprange+
                "\\." + iprange+
                "\\." + iprange + "$");
    QRegExpValidator *validator = new QRegExpValidator(reg,this);
    ip_line->setValidator(validator);
    layer->addWidget(new QLabel("IP",this),0,0);
    layer->addWidget(accept,1,0);
    layer->addWidget(reject,1,1);
    layer->addWidget(ip_line,0,1);
    setLayout(layer);
}
QString IP_Dialog::get_ip()const
{
    return ip_line->text();
}


