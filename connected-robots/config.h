#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QHash>
#include <QRegularExpression>

class Config {
public:
    Config(const QFileInfo &);
    ~Config();
    QString generatePublishString(const QString &, const QString &);
    const QHash<QString, QJsonObject>& getTopicHashMap(void) { return this->topicHashMap; }
    const QHash<QString, QRegularExpression>& getRegexHashMap(void) { return this->regexHashMap; }
    const QString getRobotName(void) { return this->jsonObject["robot"].toObject()["name"].toString(); }
    const QString getRobotTopic(void) { return this->jsonObject["robot"].toObject()["topic"].toString(); }
    const QJsonObject getMappingObject(void) { return this->jsonObject["substitutionMapping"].toObject(); }
    const QString getSubstitutionString(void) { return this->substitutionString; }

private:
    QString substitutionString;
    QJsonObject jsonObject;
    QHash<QString, QJsonObject> topicHashMap;
    QHash<QString, QRegularExpression> regexHashMap;
    void generateTopicHashMap(void);
    void generateRegexHashMap(void);
};

#endif // CONFIG_H
