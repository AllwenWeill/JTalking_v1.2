#ifndef SERVER_H
#define SERVER_H
#include <QTcpSocket>
#include <QTcpServer>
#include <QDialog>
#include <QCoreApplication>
#include <QPushButton>
class QTcpServer;
class server : public QWidget
{
    Q_OBJECT
public:
    explicit server(int portNum);
    QString getMessage();
private:
    QTcpServer *tcpServer;
    QTcpSocket *tcpSocket;
    QString message;
private slots:
    void sendMessage(QString str);
    void newConnection_Slot();
    void readyRead_Slot();
signals:
    void readyread_Signal();
};

#endif // SERVER_H
