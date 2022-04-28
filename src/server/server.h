#ifndef SERVER_H
#define SERVER_H
#include <QTcpSocket>
#include <QTcpServer>
#include <QDialog>
#include <QCoreApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QPushButton>
#include <QLabel>
class QTcpServer;

namespace Ui {
class Server;
}

class Server : public QDialog
{
    Q_OBJECT

public:
    explicit Server(QWidget *parent = 0);
    ~Server();
    QLabel *lab = new QLabel;
private:
    Ui::Server *ui;
    QTcpServer *tcpServer;
    QTcpSocket *tcpSocket;
    QString message;

private slots:
    void sendMessage(QString str);
    void newConnection_Slot();
    void readyRead_Slot();
    void on_freshButton_clicked();
};
void detachBag(QString Bag_str);
QPushButton* set_btn(Server* ptr, QString str, int rsize_x, int rsize_y, int move_x, int move_y);
void clearStr();
#endif // SERVER_H
