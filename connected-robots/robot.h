#ifndef ROBOT_H
#define ROBOT_H

#include <QObject>
#include "tcp_client.h"
#include "config.h"
#include "instruction.h"

class Robot : public QObject
{
    Q_OBJECT
private:
    //QString parseInstruction(Instruction instruction);
    Config *config;
    TCP_Client *tcp_client;
    QString generateInstructionString(const Instruction &, const QString &);
public:
    Robot(const QString &ip, const quint16 &port, Config *config = nullptr);
    void sendInstruction(Instruction &instruction, QString &topic);
    void delay(double seconds);
    void handOpen(int handNumber);
    void handClose(int handNumber);
    void moveLin(Position &pos, int value);
    void movePtp(Position &pos, int value);
    void movePtpJoin(Position &pos, int value);
    void moveLinJoin(Position &pos, int value);
    void tool(int toolNumber);
    void override(int value);
    void stop();
    Config* getConfig() { return this->config; }

public slots:
    void onReceivedData(QString &data);
    void onConnected();
    void onDisconnected();
signals:
    void receivedData(QString &data);
    void sendData(QString &data);
    void connected();
    void disconnected();

};

#endif // ROBOT_H
