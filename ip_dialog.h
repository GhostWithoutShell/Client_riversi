#ifndef IP_DIALOG_H
#define IP_DIALOG_H

#include<QDialog>
class QLineEdit;
class IP_Dialog : public QDialog
{
    Q_OBJECT
public:
    IP_Dialog(QWidget   *obj = 0);
    QString get_ip()const;
private:
    QLineEdit *ip_line;

};

#endif // IP_DIALOG_H
