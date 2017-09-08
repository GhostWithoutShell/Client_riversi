#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QTcpSocket>
#include<QTableWidgetItem>
#include"mycolor.h"
#include"updator.h"
#include"tcpclient.h"
#include<QTimer>
#include<QVariantMap>
namespace Ui {
class MainWindow;
}
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(const QString& strHost, int nPort,QWidget *parent = 0);
    ~MainWindow();
private slots:
    void on_tableWidget_itemClicked(QTableWidgetItem *item);
    void slotReadyRead   (                            );
    void slotError       (QAbstractSocket::SocketError);
    void connectIpslot();
    void slotTimer();
    void responceTimer();
private:
    void initTable();
    int calc_sum_green();
    void slotSendToServer();
    void requestToServer();
private:
    Ui::MainWindow *ui;
    QTcpSocket* socket;
    quint16 blocksize;
    MyColor curr_color;
    Updator updator;
    QTimer timer;
    QTimer timer_responce;
    QString str_responce = "";
    size_t id_room_player;
    //TcpClient *client;

};

#endif // MAINWINDOW_H
