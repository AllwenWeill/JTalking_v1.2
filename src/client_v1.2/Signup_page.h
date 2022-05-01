#ifndef SIGNUP_PAGE_H
#define SIGNUP_PAGE_H
#include <QObject>
#include <QWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>
#include <QDialog>
#include <synchapi.h>//延时库
#include "Client.h"
#include "Signin_page.h"
//注册页面
class SignUp_page : public QWidget
{
    Q_OBJECT
    QTextEdit* newId_edit = new QTextEdit;//新id输入栏
    QTextEdit* newPw_edit = new QTextEdit;//新password输入栏
    QTextEdit* newAckPw_edit = new QTextEdit;//确认password输入栏
public:
    Client *client = new Client;
    explicit SignUp_page(QWidget *parent = nullptr);
    void showPage();
    void main_label(QWidget *ptr, QString str, int move_x, int move_y, int rsize_x, int rsize_y, int ftSize);
    void main_edit(QWidget* ptr, QTextEdit* etr, int move_x, int move_y);
    void getEdit();//获取输入栏的数据
signals:

public slots:
    void newMessage();
};
#endif // SIGNUP_PAGE_H
