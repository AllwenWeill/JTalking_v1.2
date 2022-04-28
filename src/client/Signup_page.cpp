#include "Signup_page.h"
QString newId, newPw, newAckPw;
QString realPort;
SignUp_page::SignUp_page(QWidget *parent) : QWidget(parent)
{
    newId_edit->setPlaceholderText("请输入6位数字 ");
    newPw_edit->setPlaceholderText("请输入6位密码 ");
    newAckPw_edit->setPlaceholderText("请再次输入密码 ");
}
void SignUp_page::showPage(){
    QWidget *wtr = new QWidget;
    main_label(wtr,"欢迎使用JTalking!",120,50,240,70,18);
    main_label(wtr,"新账号：",105,130,60,20,10);
    main_label(wtr,"密码：",115,170,60,20,10);
    main_label(wtr,"确认密码：",100,210,60,20,10);
    main_edit(wtr,newId_edit,160,130);
    main_edit(wtr,newPw_edit,160,170);
    main_edit(wtr,newAckPw_edit,160,210);
   // connect(set_btn(wtr,"登录",60,60,260,260), &QPushButton::clicked, sitr, &SignIn_page::showPage);//仅供测试使用
    connect(set_btn(wtr,"注册",60,30,260,260), &QPushButton::clicked, this, &SignUp_page::getEdit);
    resize(450,350);
    wtr->setWindowTitle("注册页面");
    wtr->resize(450,350);
    wtr->show();
}
void SignUp_page::main_label(QWidget *ptr, QString str, int move_x, int move_y, int rsize_x, int rsize_y, int ftSize){
    QFont ft;               //设置字体
    QPalette pa;            //设置画盘
    ft.setPointSize(ftSize);
    pa.setColor(QPalette::WindowText,Qt::blue);
    QLabel *lab = new QLabel;
    lab->setParent(ptr);
    lab->setText(str);
    lab->setFont(ft);
    lab->setPalette(pa);
    lab->resize(rsize_x,rsize_y);
    lab->move(move_x,move_y);
    lab->show();
}
void SignUp_page::main_edit(QWidget *ptr, QTextEdit *etr, int move_x, int move_y){
    etr->move(move_x,move_y);
    etr->resize(160,25);
    etr->setParent(ptr);
    etr->show();
}
void SignUp_page::getEdit(){
    newId=newId_edit->toPlainText();
    newPw=newPw_edit->toPlainText();
    newAckPw=newAckPw_edit->toPlainText();
    qDebug()<<"新ID:"<<newId;
    qDebug()<<"新PW:"<<newPw;
    qDebug()<<"新ackPW:"<<newAckPw;
    if(newPw!=newAckPw){//若两次密码不同则清空两个密码输入栏的内容
        newPw_edit->clear();
        newAckPw_edit->clear();
        qDebug()<<"Error:两次密码不一致";
    }
    else {
        qDebug()<<"Success:两次密码一致";
        client->newConnect(5000);
        QString Bagtemp;
        Bagtemp.append("2");
        Bagtemp.append(newId);
        Bagtemp.append(newPw);
        realPort = client->getPort();
        Bagtemp.append(realPort);
        //将注册时要发送给服务器并存储在数据库的端口号记录下来，之后P2P时都用该端口号
        qDebug()<<"Bagtemp1:"<<Bagtemp<<";realPort:"<<realPort;
        client->sendMessage(Bagtemp);
        connect(client, &Client::readyread_Signal, this, &SignUp_page::newMessage);//收到读取信号后响应创建弹框函数
    }
}
void SignUp_page::newMessage(){//创建弹框函数
    QWidget *w = new QWidget;
    QLabel *lab = new QLabel;
    lab->setParent(w);
    lab->move(20,40);
    w->resize(260,80);
    qDebug()<<"message1:"<<client->getMessage();
    if(client->getMessage()=="Y")
        lab->setText("注册成功！您已是JTalking第"+QString::number(rand()%100)+"位用户");
    else
        lab->setText("注册失败！");
    lab->show();
    w->show();
}
