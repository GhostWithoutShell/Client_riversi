#ifndef TCPCLIENT_H
#define TCPCLIENT_H
#include"updator.h"
#include<QTableWidget>
#include<QTableWidgetItem>
#include<QTcpSocket>
#include"mycolor.h"
class QTextEdit;

class TcpClient : public QWidget
{
    Q_OBJECT
public:
    TcpClient(QTableWidget* table, QWidget *obj = 0);
    ~TcpClient();
    void connectslot();
    void update_sheet(QTableWidget* table,QTableWidgetItem* item);
protected:
    void sendToServer();
private slots:
    void slotReadyRead   (                            );
    void slotError       (QAbstractSocket::SocketError);
    void slotSendToServer(                            );
private:
    QTcpSocket* socket;
    quint16 blocksize;
    QString current_color;
    MyColor curr_color;
    Updator updator;
    QTableWidget* table;
};

#endif // TCPCLIENT_H
