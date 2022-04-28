#ifndef CLIENT_H
#define CLIENT_H
#include <QDialog>
#include <QAbstractSocket>
#include <QDebug>
#include <QPushButton>
#include <QString>
class QTcpSocket;
class Client : public QDialog
{
    Q_OBJECT

public:
    explicit Client(QWidget *parent = 0);
    ~Client();
    void sendMessage(QString str);
    QString getMessage();
    QString getPort();
private:
    QTcpSocket *tcpSocket;
    QString message;
    // 用来存放数据的大小信息
    quint16 blockSize;
public slots:
    void newConnect(int port);
    void readyRead_Slot();
signals:
    void readyread_Signal();
};
#endif // CLIENT_H
