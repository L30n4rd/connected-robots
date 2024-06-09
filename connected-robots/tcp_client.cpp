#include "tcp_client.h"

TCP_Client::TCP_Client(QString ipAddress, quint16 portNumber)
{
    this->ipAddress = ipAddress;
    this->portNumber = portNumber;
}

void TCP_Client::sendData(QString data){
    qDebug() << "TCP_CLIENT: Writing: " << data << " on Socket";
    if(tcpSocket->state() == QTcpSocket::ConnectedState){
        tcpSocket->flush();
        tcpSocket->write(data.toLatin1());
        tcpSocket->waitForBytesWritten();
        QThread::msleep(100);
    }
}

void TCP_Client::openConnection(){
    tcpSocket = new QTcpSocket();
    qDebug() << "TCP_CLIENT: Init Connection...";
    QHostAddress mitsubishiAddress = QHostAddress(ipAddress);
    tcpSocket->connectToHost(mitsubishiAddress, portNumber);

    //connectReadyReadSignal
    connect(tcpSocket, &QTcpSocket::readyRead, this, &TCP_Client::read, Qt::QueuedConnection);
    connect(tcpSocket, &QTcpSocket::connected, this, &TCP_Client::onConnected);
    connect(tcpSocket, &QTcpSocket::disconnected, this, &TCP_Client::onDisconnected);
}

void TCP_Client::onDisconnected() {
    qDebug() << "TCP_CLIENT::onDisconnected: tcpSocket->state(): " << tcpSocket->state();
    emit this->disconnected();
}

void TCP_Client::onConnected() {
    qDebug() << "TCP_CLIENT::onConnected: tcpSocket->state(): " << tcpSocket->state();
    emit this->connected();
}

void TCP_Client::read(){
    QString result;

    if(tcpSocket->state() == QTcpSocket::ConnectedState){

        QByteArray readData = tcpSocket->readAll();
        result = readData;
        qDebug() << result;

        //Sende signal mit empfangenen Daten
        emit receivedData(result);
    }
}
