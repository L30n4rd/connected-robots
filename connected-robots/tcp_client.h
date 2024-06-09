#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <QObject>
#include <QThread>
#include <QTcpSocket>

class TCP_Client: public QObject
{
    Q_OBJECT
private:
    QTcpSocket* tcpSocket;
    QString ipAddress;
    quint16 portNumber;
public:
    TCP_Client(QString ipAddress, quint16 portNumber);
    void read();
    void openConnection();
public slots:
    void sendData(QString data);
    void onConnected();
    void onDisconnected();

signals:
    void receivedData(QString &s);
    void connected();
    void disconnected();

};

#endif // TCP_CLIENT_H
