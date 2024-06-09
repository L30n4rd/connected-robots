#ifndef SUBSCRIBER_H
#define SUBSCRIBER_H

#include <qmqtt.h>

class Subscriber : public QMQTT::Client
{
    Q_OBJECT
public:
    QTextStream _qout;
    explicit Subscriber(const QHostAddress& host,
                        const quint16 port,
                        QObject* parent = nullptr);
    virtual ~Subscriber();

public slots:
    void onConnected();
    void onSubscribed(const QString& topic);
    void onReceived(const QMQTT::Message& message);

};

#endif // SUBSCRIBER_H
