#include "mywidget.h"
#include <QtNetwork>
int recordNum = 0;
QString stateFlag, Id_str, Pd_str,Port_str;
bool isY = 0;
QVector<int> deleteStore;//存放被删除用户的序号
myWidget::myWidget(QWidget *parent)
{
    Aw_label(this,"  服务器端",190,315);
    Aw_label(this,"@Aw_Tech",190,330);
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
    QPushButton *userBtn = new QPushButton;
    userBtn = set_btn(this,"序号|  ID  |  密码  |  端口号  ",200,30,200,0);
    QPushButton *logBtn = new QPushButton;
    logBtn = set_btn(this, "日志记录",100,30,20,0);
    main_edit(wtr,Log_edit,0,30,160,280);
    connect(tcpServer, &QTcpServer::newConnection,this, &myWidget::newConnection_Slot);
    wtr->setParent(this);
    wtr->setWindowTitle("JTalking_Server v2.0-Test");
    wtr->resize(450,350);
    wtr->show();
}
void myWidget::Aw_label(myWidget *ptr,QString str,int move_x,int move_y){
    QFont ft = QFont("Microsoft JhengHei",8,2);               //设置字体
    QPalette pa;            //设置画盘
    pa.setColor(QPalette::WindowText,Qt::gray);
    QLabel *lab = new QLabel;
    //ft.setItalic(true);
    lab->setParent(ptr);
    lab->setText(str);
    lab->setFont(ft);
    lab->setPalette(pa);
    lab->resize(100,15);
    lab->move(move_x,move_y);
    lab->show();
}
myWidget::~myWidget()
{

}

void myWidget::main_edit(QWidget *ptr, QTextEdit *etr, int move_x, int move_y, int resize_x, int resize_y){
    etr->move(move_x,move_y);
    etr->resize(resize_x,resize_y);
    etr->setParent(ptr);
    etr->show();
}

void myWidget::newConnection_Slot()
{
    tcpSocket=tcpServer->nextPendingConnection(); //获取已经连接的客户端套接字
    //ui->label->setText(message);
    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(readyRead_Slot()));
}
void myWidget::readyRead_Slot()
{
     QSqlQuery query;
     message=tcpSocket->readAll();
     qDebug()<<"message:"<<message;
     Log_edit->append(message);
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
         if(isY){
             sendMessage("Y");
             Log_edit->append("发送：Y");
         }
         else{
             sendMessage("N");
             Log_edit->append("发送：N");
         }
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
             query.seek(recordNum);
             QString value0 = query.value(0).toString();
             QString value1 = query.value(1).toString();
             QString value2 = query.value(2).toString();
             qDebug() <<"userLabel:"<< value0 << value1 <<value2;
             userLabel(wtr,recordNum,value0,value1,value2,200,(recordNum+1)*30,false);
             QPushButton *btn = set_btn(wtr,"删除",50,30,400,(recordNum+1)*30);
             int i = recordNum;
             connect(btn, &QPushButton::clicked,  [=] { deleteUser(i);});
             recordNum++;
             tcpSocket->disconnect();
             Log_edit->append("添加数据库：");
             Log_edit->append(value0+value1+value2);
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
         Log_edit->append("向客户端发送数据库好友信息：");
         Log_edit->append(tempStr);
         qDebug()<<"tempStr:"<<tempStr;
     }
     qDebug()<<"readAll:"<<tcpSocket->readAll();
}
void myWidget::deleteUser(int num){
    userLabel(wtr,recordNum,0,0,0,200,(num+1)*30,true);
    Log_edit->append("删除"+QString::number(num)+"号用户");
    qDebug()<<"该"<<num<<"号好友";
}

void myWidget::sendMessage(QString str)
{
    tcpSocket->write(str.toUtf8());//此处必须将QString类型转化
    // 发送数据成功后，显示提示
    Log_edit->append("发送："+str);
}

/*
@func:将接受到的数据包进行拆分，格式：状态码/ID/Password
@param:QString类型的数据包，1位状态码+6位ID+6位密码
@return:null
*/
void myWidget::detachBag(QString str){
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
    Log_edit->append("已完成解包...");
}
QPushButton* set_btn(QWidget* ptr, QString str, int rsize_x, int rsize_y, int move_x, int move_y)
{
    QPushButton *btn = new QPushButton;
    btn->setParent(ptr);
    btn->setText(str);
    btn->resize(rsize_x,rsize_y);                 //设置按键1的固定大小
    btn->move(move_x,move_y);
    btn->show();                          //show以顶层方式弹出窗口控件
    return btn;
}

void myWidget::on_freshButton_clicked()
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
void myWidget::userLabel(QWidget *ptr, int num, QString value0, QString value1, QString value2, int move_x, int move_y, bool hideFlag){
    QLabel *ltr = new QLabel;
    ltr->setParent(ptr);
    ltr->move(move_x,move_y);
    if(!hideFlag){
        ltr->setStyleSheet("QLabel{background-color:rgb(205,179,139);}");
        ltr->setText("  "+QString::number(num)+"  |"+value0+"| "+value1+" |  "+value2);
    }
    else{
        ltr->setStyleSheet("QLabel{background-color:rgb(200,101,102);}");
        ltr->setText("               NULL     ");
    }
    ltr->resize(200,30);
    ltr->show();
}

void myWidget::clearStr(){
    Id_str.clear();
    Pd_str.clear();
    Port_str.clear();
    Log_edit->append("已清空Id/Pd/Port缓存...");
}
