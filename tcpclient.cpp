#include "tcpclient.h"
#include"ip_dialog.h"
#include<QMessageBox>
#include<QDataStream>
#include<QDebug>
TcpClient::TcpClient(QTableWidget* table_, QWidget *obj):QWidget(obj),
    blocksize(0)
{
    table = table_;
    socket = new QTcpSocket(this);
}
TcpClient::~TcpClient()
{
    //delete socket;
}
void TcpClient::slotReadyRead()
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_6);
    QString str;
    QString str2;
    bool isok_row = false;
    bool isok_col = false;
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
    in >> str >> str2;
    blocksize = 0;
    if(str == "white" || str == "black")
    {
        QMessageBox::information(this,"Color",str);
        qDebug()<<str;
        curr_color.set_color(str);
        return;
    }
    int row = str.toInt(&isok_row);
    int col = str2.toInt(&isok_col);
    if(isok_row && isok_col)
    {
        updator(table,table->item(row,col),!curr_color);
        table->setEnabled(true);
    }
    else
    {
        return;
    }
}
void TcpClient::slotError(QAbstractSocket::SocketError err)
{
    QString strError =
        (err == QAbstractSocket::HostNotFoundError ?
                     "The host was not found." :
                     err == QAbstractSocket::RemoteHostClosedError ?
                     "The remote host is closed." :
                     err == QAbstractSocket::ConnectionRefusedError ?
                     "The connection was refused." :
                     QString(socket->errorString())
                    );
    QMessageBox::information(this,"Error",strError);
    /*
    if(strError != "")
    {
        socket->disconnectFromHost();
    }
    */
    //ui->textEdit->append(strError);
}
void TcpClient::slotSendToServer()
{
    //if(socket->ConnectedState == QAbstractSocket::ConnectedState)
    //{
    QByteArray arr;
    QDataStream out(&arr,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_6);
    QTableWidgetItem* item = table->currentItem();
    out<<quint16(0)<<QString::number(item->row())
       <<QString::number(item->column());
    out.device()->seek(0);
    out<<quint16(arr.size() - sizeof(quint16));
    socket->write(arr);
    //}
}
void TcpClient::connectslot()
{
    //if(socket->ConnectingState != QAbstractSocket::SocketState::ConnectedState)
    //{
    IP_Dialog *dialog = new IP_Dialog;
    if(dialog->exec() == QDialog::Accepted)
    {
        socket->connectToHost(dialog->get_ip(),2323);
        connect(socket,&QTcpSocket::readyRead,
                this,&TcpClient::slotReadyRead);
        connect(socket,SIGNAL(error(QAbstractSocket::SocketError)),
                this,SLOT(slotError(QAbstractSocket::SocketError)));
    }
    delete dialog;
    //}
}
void TcpClient::update_sheet(QTableWidget *table1,QTableWidgetItem *item)
{
    //if(socket->state() == QAbstractSocket::ConnectedState)
    //{
        if(item->background() == Qt::green)
        {
            updator(table1,item,curr_color);
            table1->setEnabled(false);
            slotSendToServer();
        }
   //}
}
