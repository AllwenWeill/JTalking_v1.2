#ifndef SIGNIN_PAGE_H
#define SIGNIN_PAGE_H
#include <QObject>
#include <QWidget>
#include "Client.h"
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>
#include <QVector>
#include <QSignalMapper>
#include "server.h"
//登录页面
class SignIn_page : public QWidget
{
    QTextEdit* Send_edit = new QTextEdit;//发送输入栏
    QTextEdit* Recv_edit = new QTextEdit;//接受输入栏
public:
    server *egoServer;
    Client *client = new Client;
    Client  *Fri_client = new Client;
    QWidget *wtr = new QWidget;
    QVector<QVector<QString>> storeSpace;
    QString tempPort;
    SignIn_page();
    ~SignIn_page();
    void showPage();
    void main_label(QWidget *ptr, QString str, int move_x, int move_y);
    void main_edit(QWidget* ptr, QTextEdit* etr, int move_x, int move_y, int resize_x, int resize_y);
    void getEdit();//获取输入栏的数据
    void storeBag(QVector<QString> str);//该包仅包含6位ID号+4位端口号+1位"/"分界符
public slots:
    void freshOperation_slot();//刷新按键响应的操作
    void newSMessage();//服务器端新消息
    void newCMessage();//其它客户端新消息
    void friendButton_slot(int num);
    void friendHint();//与好友连接成功提示
    void friendShow();
};
QPushButton* set_btn(QWidget* ptr, QString str, int rsize_x, int rsize_y, int move_x, int move_y);
#endif // SIGNIN_PAGE_H
