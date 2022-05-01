#include "Signin_page.h"
QString sendStr;
extern QString message;
extern QString realPort;
extern QString ID, PassWord;
SignIn_page::SignIn_page()
{
}
void SignIn_page::showPage(){
    main_label(wtr,"好友列表",350,0);
    main_label(wtr,"聊天框",100,0);
    main_edit(wtr,Send_edit,20,200,220,60);
    main_edit(wtr,Recv_edit,20,40,220,130);
    connect(set_btn(wtr,"发送",60,30,160,280), &QPushButton::clicked, this, &SignIn_page::getEdit);
    connect(set_btn(wtr,"刷新",60,30,280,0), &QPushButton::clicked, this, &SignIn_page::freshOperation_slot);
    qDebug()<<"realPort2:"<<realPort;
    egoServer = new server(realPort.toInt());
    qDebug()<<"realPort3:"<<realPort;
    connect(egoServer, &server::readyread_Signal, this, &SignIn_page::friendShow);
    wtr->resize(450,350);
    wtr->show();
}
void SignIn_page::main_label(QWidget *ptr, QString str, int move_x, int move_y){
    QFont ft;               //设置字体
    QPalette pa;            //设置画盘
    ft.setPointSize(16);
    pa.setColor(QPalette::WindowText,Qt::blue);
    QLabel *lab = new QLabel;
    lab->setParent(ptr);
    lab->setText(str);
    lab->setFont(ft);
    lab->setPalette(pa);
    lab->resize(100,40);
    lab->move(move_x,move_y);
    lab->show();
}
void SignIn_page::main_edit(QWidget *ptr, QTextEdit *etr, int move_x, int move_y, int resize_x, int resize_y){
    etr->move(move_x,move_y);
    etr->resize(resize_x,resize_y);
    etr->setParent(ptr);
    etr->show();
}
void SignIn_page::getEdit(){
    sendStr=Send_edit->toPlainText();
    qDebug()<<"发送:"<<sendStr;
    Fri_client->sendMessage(sendStr);
    Send_edit->clear();
    sendStr.clear();
}
QPushButton* set_btn(QWidget* ptr, QString str, int rsize_x, int rsize_y, int move_x, int move_y)
{
    QPushButton *btn = new QPushButton;
    btn->setParent(ptr);                    //将btn对象成为myWidget的子类，从而依附于myWidget窗口
    btn->setText(str);
    btn->resize(rsize_x,rsize_y);
    btn->move(move_x,move_y);
    btn->show();
    return btn;
}
void SignIn_page::freshOperation_slot(){
    client->newConnect(5000);
    QString Bagtemp;
    Bagtemp.append("4");
    Bagtemp.append(ID);
    Bagtemp.append(PassWord);
    Bagtemp.append(realPort);
    qDebug()<<"Bagtemp:"<<Bagtemp;
    client->sendMessage(Bagtemp);
    connect(client, &Client::readyread_Signal, this, &SignIn_page::newSMessage);//收到读取信号后响应响应newSMessage
}
void SignIn_page::newSMessage(){
    //QVector<QVector<QString>> temp = detachBag(client->getMessage());
    QString bagStr = client->getMessage();
    qDebug()<<"message1:"<<bagStr;
    for(int i=0;i<bagStr.size();i+=11){//该包仅包含6位ID号+4位端口号+1位"/"分界符
        QString btnName, btnPort;      //拆完包后仅剩6位ID号+4位端口号
        for(int j=i;j<=i+5;j++)
            btnName.append(bagStr[j]);
        for(int j=i+6;j<=i+9;j++)
            btnPort.append(bagStr[j]);
        QVector<QString> str;
        str.append(btnName);
        str.append(btnPort);
        storeSpace.append(str);
    }
    qDebug()<<"storeSpace:"<<storeSpace;
    for(int i=0;i<storeSpace.size();i++){//创建好友列表按键序列
        tempPort = storeSpace[i][1];//该方法会让tempPort等于最后一个好友的端口号
        QPushButton *btn =  set_btn(wtr,storeSpace[i][0],80,30,350,0+((i+1)*40));
        connect(btn, &QPushButton::clicked, [=] { friendButton_slot(i);});
    }
    //storeSpace.clear();
}
void SignIn_page::friendButton_slot(int num){//BUG
    qDebug()<<"该"<<num<<"号好友端口号为："<<storeSpace[num][1];
    Fri_client->newConnect(storeSpace[num][1].toInt());
    Fri_client->sendMessage("您的好友："+QString::number(ID.toInt())+"向你发起了聊天请求！");
    connect(client, &Client::readyread_Signal, this, &SignIn_page::friendHint);//收到读取信号后响应创建弹框函数
    //然后client连接该端口号的客户端（还需定制化client类中的newconnect函数,可传入任意端口号），同时该客户端也要利用realPort开启服务器监听模式
    //向该好友发送连接，根据接受到的ACK帧判断是否连接成功，并提供弹窗提示
    //然后可以向该好友发送数据
}
void SignIn_page::friendHint(){
    QWidget *w = new QWidget;
    QLabel *lab = new QLabel;
    lab->setParent(w);
    lab->move(20,40);
    w->resize(260,80);
    if(Fri_client->getMessage()=="Y"){
        lab->setText("连接成功!");
        lab->show();
        w->show();
    }
    else{
        lab->setText("连接失败！");
        lab->show();
        w->show();
    }
}
void SignIn_page::friendShow(){
    Recv_edit->append(egoServer->getMessage());
    qDebug()<<"fri_lab->setText:"<<egoServer->getMessage();
}
SignIn_page::~SignIn_page(){
    egoServer->disconnect();
    Fri_client->disconnect();
    client->disconnect();
}
