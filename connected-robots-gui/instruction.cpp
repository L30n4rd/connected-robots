#include <QJsonDocument>
#include <QJsonObject>
#include "instruction.h"

Instruction::Instruction(const QJsonDocument &json) {
    QJsonObject jsonPos1 = json["pos1"].toObject();
    this->pos1.x = jsonPos1["x"].toDouble();
    this->pos1.y = jsonPos1["y"].toDouble();
    this->pos1.z = jsonPos1["z"].toDouble();
    this->pos1.a = jsonPos1["a"].toDouble();
    this->pos1.b = jsonPos1["b"].toDouble();
    this->pos1.c = jsonPos1["c"].toDouble();
    QJsonObject jsonPos2 = json["pos2"].toObject();
    this->pos2.x = jsonPos2["x"].toDouble();
    this->pos2.y = jsonPos2["y"].toDouble();
    this->pos2.z = jsonPos2["z"].toDouble();
    this->pos2.a = jsonPos2["a"].toDouble();
    this->pos2.b = jsonPos2["b"].toDouble();
    this->pos2.c = jsonPos2["c"].toDouble();
    QJsonObject jsonPos3 = json["pos3"].toObject();
    this->pos3.x = jsonPos3["x"].toDouble();
    this->pos3.y = jsonPos3["y"].toDouble();
    this->pos3.z = jsonPos3["z"].toDouble();
    this->pos3.a = jsonPos3["a"].toDouble();
    this->pos3.b = jsonPos3["b"].toDouble();
    this->pos3.c = jsonPos3["c"].toDouble();

    this->instruction = json["instruction"].toString();
    this->value = json["value"].toDouble();
}

const QJsonDocument Instruction::toJsonDoc() {
    QJsonObject json;
    QJsonObject jsonPos1;
    jsonPos1["x"] = this->pos1.x;
    jsonPos1["y"] = this->pos1.y;
    jsonPos1["z"] = this->pos1.z;
    jsonPos1["a"] = this->pos1.a;
    jsonPos1["b"] = this->pos1.b;
    jsonPos1["c"] = this->pos1.c;
    QJsonObject jsonPos2;
    jsonPos2["x"] = this->pos2.x;
    jsonPos2["y"] = this->pos2.y;
    jsonPos2["z"] = this->pos2.z;
    jsonPos2["a"] = this->pos2.a;
    jsonPos2["b"] = this->pos2.b;
    jsonPos2["c"] = this->pos2.c;
    QJsonObject jsonPos3;
    jsonPos3["x"] = this->pos3.x;
    jsonPos3["y"] = this->pos3.y;
    jsonPos3["z"] = this->pos3.z;
    jsonPos3["a"] = this->pos3.a;
    jsonPos3["b"] = this->pos3.b;
    jsonPos3["c"] = this->pos3.c;

    json["instruction"] = this->instruction;
    json["value"] = this->value;
    json["pos1"] = jsonPos1;
    json["pos2"] = jsonPos2;
    json["pos3"] = jsonPos3;

    QJsonDocument doc(json);
    return doc;
}

const QString Instruction::toJsonString(QJsonDocument::JsonFormat format) {
    return this->toJsonDoc().toJson(format);
}

double& Instruction::operator[](const QString& key) {
    if (key == "pos1_x") return pos1.x;
    if (key == "pos1_y") return pos1.y;
    if (key == "pos1_z") return pos1.z;
    if (key == "pos1_a") return pos1.a;
    if (key == "pos1_b") return pos1.b;
    if (key == "pos1_c") return pos1.c;
    if (key == "pos2_x") return pos2.x;
    if (key == "pos2_y") return pos2.y;
    if (key == "pos2_z") return pos2.z;
    if (key == "pos2_a") return pos2.a;
    if (key == "pos2_b") return pos2.b;
    if (key == "pos2_c") return pos2.c;
    if (key == "pos3_x") return pos3.x;
    if (key == "pos3_y") return pos3.y;
    if (key == "pos3_z") return pos3.z;
    if (key == "pos3_a") return pos3.a;
    if (key == "pos3_b") return pos3.b;
    if (key == "pos3_c") return pos3.c;
    if (key == "value") return value;
    return bracketOperatorDefaultValue;
}
