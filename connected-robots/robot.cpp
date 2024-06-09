#include "robot.h"
#include "config.h"

Robot::Robot(const QString &ip, const quint16 &port, Config *config)
        : config(config),
          tcp_client(new TCP_Client(ip, port)) {

    connect(tcp_client, &TCP_Client::receivedData, this, &Robot::onReceivedData);
    connect(tcp_client, &TCP_Client::connected, this, &Robot::onConnected);
    connect(tcp_client, &TCP_Client::disconnected, this, &Robot::onDisconnected);

    // TCP-Verbindung öffnen
    tcp_client->openConnection();
}

/**
 * @brief Robot::sendInstruction Wandelt eine Instruction in einen String um und sendet diesen an den Roboter.
 * @param instruction
 * @param topic
 */
void Robot::sendInstruction(Instruction &instruction, QString &topic) {
    QString parsedInstructionString = this->generateInstructionString(instruction, topic);
    this->tcp_client->sendData(parsedInstructionString);
    emit sendData(parsedInstructionString);
}

/**
 * @brief Robot::generateInstructionString Generiert aus einer Instruction einen String, der an den Roboter gesendet werden kann.
 * @param instruction
 * @param topicString
 * @return generierten String
 */
QString Robot::generateInstructionString(const Instruction &instruction, const QString &topicString) {
    if (!this->config->getTopicHashMap().contains(topicString)) {
        qCritical("ROBOT: Couldn't find topic '%s' in hash map.", qUtf8Printable(topicString));
        return "";
    }

    QJsonObject topicObject = this->config->getTopicHashMap().value(topicString);
    if (topicObject["type"].toString() != "sub") {
        qCritical("ROBOT: The topic '%s' is not used to be subscribed by this robot.", qUtf8Printable(topicString));
        return "";
    }

    QString instructionString = this->config->getSubstitutionString();
    QJsonObject mappingObject = this->config->getMappingObject();

    // instruction
    instructionString.replace(mappingObject["instruction"].toString(), instruction.instruction);
    instructionString.replace(mappingObject["value"].toString(), QString::number(instruction.value));
    // pos1
    instructionString.replace(mappingObject["pos1_x"].toString(), QString::number(instruction.pos1.x));
    instructionString.replace(mappingObject["pos1_y"].toString(), QString::number(instruction.pos1.y));
    instructionString.replace(mappingObject["pos1_z"].toString(), QString::number(instruction.pos1.z));
    instructionString.replace(mappingObject["pos1_a"].toString(), QString::number(instruction.pos1.a));
    instructionString.replace(mappingObject["pos1_b"].toString(), QString::number(instruction.pos1.b));
    instructionString.replace(mappingObject["pos1_c"].toString(), QString::number(instruction.pos1.c));
    // pos2
    instructionString.replace(mappingObject["pos2_x"].toString(), QString::number(instruction.pos2.x));
    instructionString.replace(mappingObject["pos2_y"].toString(), QString::number(instruction.pos2.y));
    instructionString.replace(mappingObject["pos2_z"].toString(), QString::number(instruction.pos2.z));
    instructionString.replace(mappingObject["pos2_a"].toString(), QString::number(instruction.pos2.a));
    instructionString.replace(mappingObject["pos2_b"].toString(), QString::number(instruction.pos2.b));
    instructionString.replace(mappingObject["pos2_c"].toString(), QString::number(instruction.pos2.c));
    // pos3
    instructionString.replace(mappingObject["pos3_x"].toString(), QString::number(instruction.pos3.x));
    instructionString.replace(mappingObject["pos3_y"].toString(), QString::number(instruction.pos3.y));
    instructionString.replace(mappingObject["pos3_z"].toString(), QString::number(instruction.pos3.z));
    instructionString.replace(mappingObject["pos3_a"].toString(), QString::number(instruction.pos3.a));
    instructionString.replace(mappingObject["pos3_b"].toString(), QString::number(instruction.pos3.b));
    instructionString.replace(mappingObject["pos3_c"].toString(), QString::number(instruction.pos3.c));

    return instructionString;
}

void Robot::onReceivedData(QString &data) {
    emit receivedData(data);
}

void Robot::onConnected() {
    emit connected();
}

void Robot::onDisconnected() {
    emit disconnected();
}

void Robot::handClose(int handNumber){
    QString topic = "command/move";
    Instruction i;
    i.instruction = "HCLOSE";
    i.value = handNumber;
    sendInstruction(i, topic);
}

void Robot::handOpen(int handNumber){
    QString topic = "command/move";
    Instruction i;
    i.instruction = "HOPEN";
    i.value = handNumber;
    sendInstruction(i, topic);
}

void Robot::delay(double seconds){
    QString topic = "command/move";
    Instruction i;
    i.instruction = "DLY";
    i.value = seconds;
    sendInstruction(i, topic);
}

void Robot::moveLin(Position &pos, int value){
    QString topic = "command/move";
    Instruction i;
    i.instruction = "LIN";
    i.pos1 = pos;
    i.value = value;
    sendInstruction(i, topic);
}

void Robot::movePtp(Position &pos, int value){
    QString topic = "command/move";
    Instruction i;
    i.instruction = "PTP";
    i.pos1 = pos;
    i.value = value;
    sendInstruction(i, topic);
}

void Robot::movePtpJoin(Position &pos,int value){  //hier sind jetzt in pos natürlich Winkel drin
    QString topic = "command/move";
    Instruction i;
    i.instruction = "PTPJOINT";
    i.pos1 = pos;
    i.value = value;
    sendInstruction(i, topic);
}

void Robot::moveLinJoin(Position &pos, int value){  //hier sind jetzt in pos natürlich Winkel drin
    QString topic = "command/move";
    Instruction i;
    i.instruction = "LINJOINT";
    i.pos2 = pos;
    i.value = value;
    sendInstruction(i, topic);
}

void Robot::tool(int toolNumber){
    QString topic = "command/move";
    Instruction i;
    i.instruction = "TOOL";
    i.value = toolNumber;
    sendInstruction(i, topic);
}

void Robot::override(int value){
    QString topic = "command/move";
    Instruction i;
    i.instruction = "OVRD";
    i.value = value;
    sendInstruction(i, topic);
}

void Robot::stop(){
    QString topic = "command/stop";
    Instruction i;
    i.instruction = "STOP";
    sendInstruction(i, topic);
}
