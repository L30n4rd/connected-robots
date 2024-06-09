#include "subscriber.h"
#include <QStringBuilder>

Subscriber::Subscriber(const QHostAddress& host,
                        const quint16 port,
                        QObject* parent)
    : QMQTT::Client(host, port, parent)
    , _qout(stdout)
{
    connect(this, &Subscriber::connected, this, &Subscriber::onConnected);
    connect(this, &Subscriber::subscribed, this, &Subscriber::onSubscribed);
    connect(this, &Subscriber::received, this, &Subscriber::onReceived);

    this->setClientId("gui");
    this->connectToHost();
}

Subscriber::~Subscriber() {}

void Subscriber::onConnected() {
    _qout << "SUBSCRIBER: connected" << Qt::endl;
}

void Subscriber::onSubscribed(const QString& topic) {
    _qout << "SUBSCRIBER: subscribed " << topic << Qt::endl;
}

void Subscriber::onReceived(const QMQTT::Message& message) {
    QString payload = QString::fromUtf8(message.payload());
    _qout << "SUBSCRIBER: publish on "
          << message.topic()
          << " received: \"" << payload << "\""
          << Qt::endl;
}
