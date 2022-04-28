#include "server.h"
#include "ui_server.h"
#include <QtNetwork>
QString stateFlag, Id_str, Pd_str,Port_str;
bool isY = 0;
Server::Server(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Server)
{
    ui->setupUi(this);
    tcpServer = new QTcpServer(this);
    tcpSocket = new QTcpSocket(this);
    // 使用了IPv4的本地主机地址，等价于QHostAddress("127.0.0.1")
    if (!tcpServer->listen(QHostAddress::LocalHost, 5000)) {
        qDebug() << tcpServer->errorString();
        close();
    }
    //添加数据库驱动
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    //设置数据库名称
    db.setDatabaseName(":memory:");//表示使用内存数据库
    //打开数据库
    if(!db.open())
        qDebug()<<"Error:database opening failed!";
    else
        qDebug()<<"Success:database opened!";
    QSqlQuery query;
    if (!query.exec("create table user(id varchar primary key,pd varchar,port varchar)")) {
        qDebug() << "执行sql语句失败";
    }
    connect(tcpServer, &QTcpServer::newConnection,this, &Server::newConnection_Slot);

    lab->setParent(this);
    lab->resize(300,100);
    lab->show();
}
Server::~Server()
{
    delete ui;
}
void Server::newConnection_Slot()
{
    tcpSocket=tcpServer->nextPendingConnection(); //获取已经连接的客户端套接字
    //ui->label->setText(message);
    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(readyRead_Slot()));
}
void Server::readyRead_Slot()
{
     QSqlQuery query;
     message=tcpSocket->readAll();
     qDebug()<<"message:"<<message;
     lab->setText(message);
     ui->label->setText(message);
     detachBag(message);
     if(stateFlag=="1"||stateFlag=="3"){//登录——校验数据库，并表示该用户已上线
         query.exec("select id,pd,port from user");
         while(query.next())
         {
             QString value0 = query.value(0).toString();
             QString value1 = query.value(1).toString();
             if(value0==Id_str&&value1==Pd_str){
                 isY=1;
                 qDebug() << value0 << value1 ;
             }
         }
         if(isY)
             sendMessage("Y");
         else
             sendMessage("N");
         clearStr();
         tcpSocket->disconnectFromHost();//断开与当前客户端的连接
         isY=0;
     }
     else if(stateFlag=="2"){//注册——添加数据库
         query.prepare("insert into user (id, pd, port) "
                           "values (:id, :pd, :port)");
         query.bindValue(0, Id_str);
         query.bindValue(1, Pd_str);
         query.bindValue(2, Port_str);
         query.exec();
         if(query.exec("select id,pd,port from user")){
             sendMessage("Y");
             while(query.next()){
                 QString value0 = query.value(0).toString();
                 QString value1 = query.value(1).toString();
                 QString value2 = query.value(2).toString();
                 qDebug() << value0 << value1 <<value2;
                 lab->setText(value2);
             }
         }
         else {
             sendMessage("N");
             qDebug()<<"Error:数据库操作失败！";
         }
         clearStr();
         tcpSocket->disconnectFromHost();//断开与当前客户端的连接
     }
     else if(stateFlag=="4"){//客户端想要获取数据库当前好友
         query.exec("select id,pd,port from user");
         QString tempStr;
         while(query.next()){
             QString value0 = query.value(0).toString();
             QString value2 = query.value(2).toString();
             tempStr.append(value0);
             tempStr.append(value2);
             tempStr.append("/");//分节符
         }
         sendMessage(tempStr);
         qDebug()<<"tempStr:"<<tempStr;
         lab->setText(tempStr);
     }
}
void Server::sendMessage(QString str)
{
    tcpSocket->write(str.toUtf8());//此处必须将QString类型转化
    // 发送数据成功后，显示提示
    ui->label->setText(tr("发送:")+str);
}

/*
@func:将接受到的数据包进行拆分，格式：状态码/ID/Password
@param:QString类型的数据包，1位状态码+6位ID+6位密码
@return:null
*/
void detachBag(QString str){
    stateFlag = str[0];
    for(int i=1;i<7;i++)//1-6
        Id_str.append(str[i]);
    for(int i=7;i<=12;i++)//7-12
        Pd_str.append(str[i]);
    for(int i=13;i<=16;i++)//13-16
        Port_str.append(str[i]);
    qDebug()<<"Id_str:"<<Id_str;
    qDebug()<<"Pd_str"<<Pd_str;
    qDebug()<<"Port_str"<<Port_str;
}
QPushButton* set_btn(Server* ptr, QString str, int rsize_x, int rsize_y, int move_x, int move_y)
{
    QPushButton *btn = new QPushButton;
    btn->setParent(ptr);
    btn->setText(str);
    btn->resize(rsize_x,rsize_y);                 //设置按键1的固定大小
    btn->move(move_x,move_y);
    btn->show();                          //show以顶层方式弹出窗口控件
    return btn;
}

void Server::on_freshButton_clicked()
{
    QSqlQuery query;
    QString value0 = query.value(0).toString();
    QString value1 = query.value(1).toString();
    qDebug()<<"输出数据库：";
    qDebug() << value0 << value1 ;
    while(query.next())
    {
        QString value0 = query.value(0).toString();
        QString value1 = query.value(1).toString();
        qDebug()<<"输出数据库：";
        qDebug() << value0 << value1 ;
    }
}
void clearStr(){
    Id_str.clear();
    Pd_str.clear();
    Port_str.clear();
}
