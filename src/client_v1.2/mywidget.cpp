#include "mywidget.h"
QString ID;//6位
QString PassWord;//6位
extern QString message;
extern QString realPort;
myWidget::myWidget(QWidget *parent)
    : QWidget(parent)
{
    SignUp_page *sutr = new SignUp_page;
    Id_edit->setPlaceholderText("请输入6位账号 ");
    Pw_edit->setPlaceholderText("请输入6位密码 ");
    main_edit(this,Id_edit,160,130);
    main_edit(this,Pw_edit,160,170);
    Logo_label(this);
    Aw_label(this,"  客户端",190,315);
    Aw_label(this,"@Aw_Tech",190,330);
    main_label(this,"账号",115,130,40,20);
    main_label(this,"密码",115,170,40,20);
   // connect(set_btn(this,"登录",60,30,300,250), &QPushButton::clicked, sitr, &SignIn_page::showPage);
    connect(set_btn(this,"注册",60,30,120,230), &QPushButton::clicked, sutr, &SignUp_page::showPage);
    connect(set_btn(this,"确认",60,30,250,230), &QPushButton::clicked, this, &myWidget::getEdit);
    resize(450,350);
    setWindowTitle("JTalking_Client v1.1-Test");//设置窗口标题
}

myWidget::~myWidget()
{
    client->disconnect();
    client->close();
}
QPushButton* set_btn(myWidget* ptr, QString str, int rsize_x, int rsize_y, int move_x, int move_y)
{
    QPushButton *btn = new QPushButton;
    btn->setParent(ptr);                    //将btn对象成为myWidget的子类，从而依附于myWidget窗口
    btn->setText(str);
    btn->resize(rsize_x,rsize_y);                 //设置按键1的固定大小
    btn->move(move_x,move_y);
    btn->show();                          //show以顶层方式弹出窗口控件
    return btn;
}
void myWidget::main_edit(myWidget *ptr, QTextEdit* etr, int move_x, int move_y){
    etr->move(move_x,move_y);
    etr->resize(160,25);
    etr->setParent(ptr);
    etr->show();
}
void myWidget::main_label(myWidget *ptr, QString str, int move_x, int move_y, int rsize_x, int rsize_y){
    QFont ft;               //设置字体
    QPalette pa;            //设置画盘
    ft.setPointSize(16);
    pa.setColor(QPalette::WindowText,Qt::black);
    QLabel *lab = new QLabel;
    lab->setParent(ptr);
    lab->setText(str);
    lab->setFont(ft);
    lab->setPalette(pa);
    lab->resize(rsize_x,rsize_y);
    lab->move(move_x,move_y);
    lab->show();
}
void myWidget::Logo_label(myWidget *ptr){
    QFont ft = QFont("SimSun",40,2);               //设置字体
    QPalette pa;            //设置画盘
    pa.setColor(QPalette::WindowText,Qt::black);
    QLabel *lab = new QLabel;
    ft.setItalic(true);//设置斜体
    lab->setParent(ptr);
    lab->setText("JTalking");
    lab->setFont(ft);
    lab->setPalette(pa);
    lab->resize(220,80);
    lab->move(110,20);
    lab->show();
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
void myWidget::getEdit(){
    ID=Id_edit->toPlainText();
    PassWord=Pw_edit->toPlainText();
    qDebug()<<"ID:"<<ID;
    qDebug()<<"PW:"<<PassWord;
    client->newConnect(5000);
    QString Bagtemp;
    Bagtemp.append("3");
    Bagtemp.append(ID);
    Bagtemp.append(PassWord);
    Bagtemp.append(realPort);
    qDebug()<<"loginBagtemp:"<<Bagtemp;
    client->sendMessage(Bagtemp);
    connect(client, &Client::readyread_Signal, this, &myWidget::newMessage);//收到读取信号后响应创建弹框函数
}
void myWidget::newMessage(){
    if(client->getMessage()=="Y"){
        SignIn_page *sitr = new SignIn_page;
        sitr->showPage();
    }
    else if(client->getMessage()=="N"){
        QWidget *w = new QWidget;
        QLabel *lab = new QLabel;
        lab->setParent(w);
        lab->move(20,40);
        w->resize(260,80);
        lab->setText("账号或密码错误！");
        lab->show();
        w->show();
    }
    client->disconnect();//断开信号连接（我本意是断开tcp连接，不小心写错了结果解决了多次弹窗的Bug,无心插柳柳成荫...）
}
