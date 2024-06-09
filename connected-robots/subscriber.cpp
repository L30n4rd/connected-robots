#include "subscriber.h"
#include "config.h"
#include <QStringBuilder>

Subscriber::Subscriber(const QHostAddress& host,
                        const quint16 port,
                        Config* config,
                        QObject* parent)
    : QMQTT::Client(host, port, parent)
    , _qout(stdout)
    , config(config)
{
    connect(this, &Subscriber::connected, this, &Subscriber::onConnected);
    connect(this, &Subscriber::subscribed, this, &Subscriber::onSubscribed);
    connect(this, &Subscriber::received, this, &Subscriber::onReceived);

    QString robotName(config->getRobotName());
    this->setClientId(robotName.replace(" ", "_"));
    this->connectToHost();
}

Subscriber::~Subscriber() {}

void Subscriber::onConnected() {
    _qout << "SUBSCRIBER: connected" << Qt::endl;

    // Subscribe alle Topics, die in der Config-Datei vom Roboter enthalten sind und zum Abbonieren gekennzeichnet sind
    QString robotTopic = this->config->getRobotTopic();
    QHash<QString, QJsonObject> topics = this->config->getTopicHashMap();
    for (QHash<QString, QJsonObject>::const_iterator it = topics.cbegin(), end = topics.cend(); it != end; ++it) {
        if (it.value()["type"] == "sub") {
            subscribe(robotTopic % "/" % it.key(), 0);
        }
    }
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
