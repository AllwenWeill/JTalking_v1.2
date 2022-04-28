#ifndef MYWIDGET_H
#define MYWIDGET_H
#include <QDebug>
#include <QPushButton>
#include <QString>
#include <QWidget>
#include <QTextEdit>
#include "Client.h"
#include "Signin_page.h"
#include "Signup_page.h"
#include <QLabel>
class myWidget : public QWidget
{
    Q_OBJECT
    QTextEdit* Id_edit = new QTextEdit;//账号输入栏；妙！这样类中所有函数皆可使用该对象！
    QTextEdit* Pw_edit = new QTextEdit;//密码输入栏
public:
    myWidget(QWidget *parent = 0);
    ~myWidget();
    Client *client = new Client;
    void main_edit(myWidget* ptr, QTextEdit* etr, int move_x, int move_y);
    void main_label(myWidget* ptr, QString str, int move_x, int move_y, int rsize_x, int rsize_y);
    void getEdit();//获取输入栏的数据
    void Logo_label(myWidget* ptr);
    void Aw_label(myWidget* ptr);
public slots:
    void newMessage();
};
//设置Button的函数,分别传入myWidget的this指针，需要按键显示的字符串str,按键的长rsize_x,宽rsize_y;按键的初始位置横坐标move_x和纵坐标move_y
QPushButton* set_btn(myWidget* ptr, QString str, int rsize_x, int rsize_y, int move_x, int move_y);
#endif // MYWIDGET_H
