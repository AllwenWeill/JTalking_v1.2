#include "server.h"
#include <QtNetwork>
server::server(int portNum)
{
    tcpServer = new QTcpServer(this);
    tcpSocket = new QTcpSocket(this);
    if (!tcpServer->listen(QHostAddress::LocalHost, portNum)) {
        qDebug() << tcpServer->errorString();
        close();
    }
    connect(tcpServer, &QTcpServer::newConnection,this, &server::newConnection_Slot);
}
void server::newConnection_Slot()
{
    tcpSocket=tcpServer->nextPendingConnection(); //获取已经连接的客户端套接字
    //ui->label->setText(message);
    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(readyRead_Slot()));
}
void server::readyRead_Slot()
{
     message=tcpSocket->readAll();
     qDebug()<<"message:"<<message;
     emit readyread_Signal();//只要读取数据就发送该信号
}
void server::sendMessage(QString str)
{
    tcpSocket->write(str.toUtf8());//此处必须将QString类型转化
}
QString server::getMessage(){
    return message;
}
