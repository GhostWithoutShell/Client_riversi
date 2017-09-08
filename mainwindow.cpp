#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"ip_dialog.h"
#include<QMessageBox>
#include<QMenu>
#include<QAction>

MainWindow::MainWindow(const QString &str,int port,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),blocksize(0)
{
    //client = new TcpClient(ui->tableWidget);
    ui->setupUi(this);
    connect(&timer,&QTimer::timeout,this,&MainWindow::slotTimer);
    initTable();
    socket = new QTcpSocket(this);
    QMenu *menu = new QMenu("Game",this);
    QAction *aciton = new QAction("Connect",this);
    connect(aciton,&QAction::triggered,this,&MainWindow::connectIpslot);
    //connect(&timer_responce,&QTimer::timeout,this,&MainWindow::responceTimer);
    menu->addAction(aciton);
    this->menuBar()->addMenu(menu);
}
void MainWindow::initTable()
{
    ui->tableWidget->setRowCount(8);
    ui->tableWidget->setColumnCount(8);
    for(int i = 0;i<8;++i)
    {
        for(int j = 0;j<8;++j)
        {
            QTableWidgetItem* item = new QTableWidgetItem;
            item->setBackground(Qt::green);
            ui->tableWidget->setItem(i,j,item);
        }
    }
    ui->tableWidget->item(3,3)->setBackground(Qt::white);
    ui->tableWidget->item(3,4)->setBackground(Qt::black);
    ui->tableWidget->item(4,3)->setBackground(Qt::black);
    ui->tableWidget->item(4,4)->setBackground(Qt::white);
    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->resizeRowsToContents();
}
void MainWindow::slotReadyRead()
{
    QVariantMap data;
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_6);
    if(!blocksize)
    {
        if(socket->bytesAvailable() < (int)sizeof(quint16))
        {
            return;
        }
        in >> blocksize;
    }
    if(socket->bytesAvailable() < blocksize)
    {
        return;
    }
    timer.start(30000);
    blocksize = 0;
    str_responce = "responce";
    in >> data;
    qDebug()<<data["type"].toString();
    if(data["type"].toString() == "data")
    {
        size_t row = data["row"].toInt();
        size_t col = data["col"].toInt();
        updator(ui->tableWidget,ui->tableWidget->item(row,col),!curr_color);
        ui->tableWidget->setEnabled(true);
    }
    else if(data["type"] == "color")
    {
        ui->tableWidget->setEnabled(false);
        ui->textEdit->append(data["color"].toString());
        curr_color.set_color(data["color"].toString());
        id_room_player = data["id_room"].toInt();
    }
    else if(data["type"].toString() == "start" && curr_color == Qt::black)
    {
        ui->tableWidget->setEnabled(true);
    }
    //else if(data["type"] )
    if(updator.get_winner(ui->tableWidget) != "")
    {
        QMessageBox::information(this,"Результаты",updator.get_winner(ui->tableWidget));
    }
}
void MainWindow::slotError(QAbstractSocket::SocketError err)
{ 
    QString strError =
        "Error: " + (err == QAbstractSocket::HostNotFoundError ?
                     "The host was not found." :
                     err == QAbstractSocket::RemoteHostClosedError ?
                     "The remote host is closed." :
                     err == QAbstractSocket::ConnectionRefusedError ?
                     "The connection was refused." :
                     QString(socket->errorString())
                    );
    ui->textEdit->append(strError);
    socket->disconnect();
    timer.stop();
    timer_responce.stop();
}
void MainWindow::slotSendToServer()
{
    if(socket->ConnectedState == QAbstractSocket::ConnectedState)
    {
        QByteArray arr;
        QDataStream out(&arr,QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_6);
        QTableWidgetItem* item = ui->tableWidget->currentItem();
        QVariantMap data;
        data["type"] = "data";
        data["id_room"] = QString::number(id_room_player);
        data["row"] = QString::number(item->row());
        data["col"] = QString::number(item->column());
        out<<quint16(0)<<data;
        out.device()->seek(0);
        out<<quint16(arr.size() - sizeof(quint16));
        socket->write(arr);
    }
}
void MainWindow::requestToServer()
{
    if(socket->ConnectedState == QAbstractSocket::ConnectedState)
    {
        QByteArray arr;
        QDataStream out(&arr,QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_6);
        QVariantMap req;
        req["type"] = "time_out";
        out<<quint16(0)<<req;
        out.device()->seek(0);
        out<<quint16(arr.size() - sizeof(quint16));
        socket->write(arr);
    }
}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_tableWidget_itemClicked(QTableWidgetItem *item)
{
    if(socket->state() == QAbstractSocket::ConnectedState)
    {
        if(item->background() == Qt::green)
        {
            updator(ui->tableWidget,item,curr_color);
            ui->tableWidget->setEnabled(false);
            slotSendToServer();
        }
    }
    if(updator.get_winner(ui->tableWidget) != "")
    {
        QMessageBox::information(this,"Результаты",updator.get_winner(ui->tableWidget));
        socket->disconnectFromHost();
    }
}
int MainWindow::calc_sum_green()
{
    int sum = 0;
    for(int i = 0;i<ui->tableWidget->rowCount();++i)
    {
        for(int j = 0;j<ui->tableWidget->columnCount();++j)
        {
            if(ui->tableWidget->item(i,j)->background().color() != Qt::green)
            {
                ++sum;
            }
        }
    }
    return sum;
}
void MainWindow::connectIpslot()
{
    IP_Dialog *dialog = new IP_Dialog;
    if(dialog->exec() == QDialog::Accepted)
    {
        socket->connectToHost(dialog->get_ip(),2323);
        connect(socket,&QTcpSocket::readyRead,
                this,&MainWindow::slotReadyRead);
        connect(socket,SIGNAL(error(QAbstractSocket::SocketError)),
                this,SLOT(slotError(QAbstractSocket::SocketError)));
        timer.start(30000);
        initTable();
        ui->textEdit->clear();
    }
    delete dialog;
}
void MainWindow::slotTimer()
{
    qDebug()<<str_responce;
    if(str_responce == "responce")
    {
        str_responce = "";
    }
    else
    {
        requestToServer();
    }
    timer.start(30000);
}
void MainWindow::responceTimer()
{
    /*
    qDebug()<<"responce";
    if(str_responce == "")
    {
        ui->textEdit->append("Erorr connect was refused");
        socket->disconnect();
        timer.stop();
        timer_responce.stop();
    }
    else if(str_responce == "responce")
    {
        qDebug()<<"responce sucsess";
        str_responce = "";
    }
    */
}
