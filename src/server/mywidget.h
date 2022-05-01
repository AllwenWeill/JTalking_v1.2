#ifndef MYWIDGET_H
#define MYWIDGET_H
#include <QTcpSocket>
#include <QTcpServer>
#include <QDialog>
#include <QCoreApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>
class QTcpServer;
class myWidget : public QWidget
{
    Q_OBJECT

public:
    QTextEdit* Log_edit = new QTextEdit;//日志输入栏
    QWidget *wtr = new QWidget;
    myWidget(QWidget *parent = 0);
    ~myWidget();
    void main_edit(QWidget* ptr, QTextEdit* etr, int move_x, int move_y, int resize_x, int resize_y);
    void userLabel(QWidget *ptr, int num, QString value0, QString value1, QString value2, int move_x, int move_y, bool hideFlag);
    void detachBag(QString Bag_str);
    void clearStr();
    void Aw_label(myWidget* ptr,QString str,int move_x,int move_y);
private:
    QTcpServer *tcpServer;
    QTcpSocket *tcpSocket;
    QString message;

private slots:
    void sendMessage(QString str);
    void newConnection_Slot();
    void readyRead_Slot();
    void on_freshButton_clicked();
    void deleteUser(int num);
};
QPushButton* set_btn(QWidget* ptr, QString str, int rsize_x, int rsize_y, int move_x, int move_y);

#endif // MYWIDGET_H
