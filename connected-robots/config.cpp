#include "config.h"
#include <QFile>
#include <QIODevice>
#include <QHash>
#include <QJsonArray>

#include <iostream>

Config::Config(const QFileInfo &fileInfo) {
    // Datei öffnen und lesen
    QFile file = QFile(fileInfo.absoluteFilePath());
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning("CONFIG: Couldn't open config file.");
    }
    QByteArray configData = file.readAll();
    file.close();

    // in JSON-Objekt umwandeln
    QJsonDocument loadDoc(QJsonDocument::fromJson(configData));
    this->jsonObject = loadDoc.object();

    this->substitutionString = loadDoc["substitutionString"].toString();

    generateTopicHashMap();
    generateRegexHashMap();
}

Config::~Config() {}

/**
 * @brief Config::generateTopicHashMap Generiere eine HashMap mit den Topics als Key und den
 *  entsprechenden Informationen zum jeweiligen Topic als Value.
 */
void Config::generateTopicHashMap() {
    this->topicHashMap.clear();
    QJsonArray topicArray = this->jsonObject["topics"].toArray();
    for (int topicIndex = 0; topicIndex < topicArray.size(); topicIndex++) {
        QJsonObject topicObject = topicArray[topicIndex].toObject();
        this->topicHashMap.insert(topicObject["topic"].toString(), topicObject);
    }
}

/**
 * @brief Config::generateRegexHashMap Generiert eine HashMap mit den Instruction-Properties-String als Key und
 * der passenden RegularExpression, mit der man den Wert für die Instruction-Property
 * aus dem String, den der Roboter gesendet hat, extrahieren kann.
 */
void Config::generateRegexHashMap() {
    this->regexHashMap.clear();
    QJsonObject regexJsonObject = this->jsonObject["regexMapping"].toObject();
    for (const QString& regexKey : regexJsonObject.keys()) {
        QString regexString = regexJsonObject[regexKey].toString();
        QRegularExpression regexObject(regexString);
        this->regexHashMap.insert(regexKey, regexObject);
    }
}
