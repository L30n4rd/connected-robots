#ifndef SUBSCRIBER_H
#define SUBSCRIBER_H

#include <qmqtt.h>
#include "config.h"

class Subscriber : public QMQTT::Client
{
    Q_OBJECT
public:
    QTextStream _qout;
    explicit Subscriber(const QHostAddress& host,
                        const quint16 port,
                        Config* config = nullptr,
                        QObject* parent = nullptr);
    virtual ~Subscriber();

private:
    Config* config;


public slots:
    void onConnected();
    void onSubscribed(const QString& topic);
    void onReceived(const QMQTT::Message &message);

signals:
    void sendData(const QMQTT::Message &message);

};

#endif // SUBSCRIBER_H
