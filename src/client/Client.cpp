#include "Client.h"
#include <QtNetwork>
#include <QDebug>

Client::Client(QWidget *parent)
{
    tcpSocket = new QTcpSocket(this);
    //newConnect();
}
Client::~Client()
{
    qDebug()<<"客户端下线！";
}
void Client::newConnect(int port)
{
    // 初始化数据大小信息为0
    blockSize = 0;
    // 取消已有的连接
    tcpSocket->abort();
    tcpSocket->connectToHost(QHostAddress::LocalHost,port);
}
void Client::sendMessage(QString str){
    //SignIn-1,SignUp-2,Ack-3
    tcpSocket->write(str.toUtf8());//此处必须将QString类型转化
    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(readyRead_Slot()));
}
void Client::readyRead_Slot(){
    message=tcpSocket->readAll();
    qDebug()<<"message2:"<<message;
    emit readyread_Signal();//只要读取数据就发送该信号
}
QString Client::getMessage(){
    return message;
}
QString Client::getPort(){
    QString portNum;
    for(int i=0;i<4;i++){
        QString temp = QString::number(rand()%9);
        portNum.append(temp);
    }
    return portNum;
}
