#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "robot.h"
#include "config.h"
#include <iostream>
#include <QFile>
#include <QDir>
#include <QFileDialog>
#include <QStringBuilder>
#include <QRegularExpression>

const QHostAddress DEFAULT_MQTT_HOST_ADDRESS = QHostAddress::LocalHost;
const quint16 DEFAULT_MQTT_PORT = 1883;

const QHostAddress DEFAULT_ROBOT_HOST_ADDRESS = QHostAddress("172.31.1.146");
const quint16 DEFAULT_ROBOT_PORT = 10003;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , config(nullptr)
    , robot(nullptr)
    , subscriber(nullptr)
    , messageId(0)

{
    ui->setupUi(this);

    // setze Standardwerte in der Ui
    ui->lineEdit_robot_ip_address->setText(DEFAULT_ROBOT_HOST_ADDRESS.toString());
    ui->lineEdit_robot_port->setText(QString::number(DEFAULT_ROBOT_PORT));
    ui->lineEdit_mqtt_ip_address->setText(DEFAULT_MQTT_HOST_ADDRESS.toString());
    ui->lineEdit_mqtt_port->setText(QString::number(DEFAULT_MQTT_PORT));

    connect(ui->button_select_config, &QPushButton::clicked, this, &MainWindow::onSelectConfigFile);
    connect(ui->button_robot_connect, &QPushButton::clicked, this, &MainWindow::onConnectRobot);
    connect(ui->button_robot_cmd_home, &QPushButton::clicked, this, &MainWindow::onRobotCmdHome);
    connect(ui->button_mqtt_connect, &QPushButton::clicked, this, &MainWindow::onConnectMqtt);

    // gibt eine Standard Instruction auf der Konsole aus, um den Aufbau für eine MQTT Nachricht zu zeigen
    Instruction exampleInstruction;
    std::cout << exampleInstruction.toJsonString(QJsonDocument::Compact).toStdString() << std::endl;
}

MainWindow::~MainWindow() {
    delete config;
    delete robot;
    delete ui;
}

/**
 * @brief MainWindow::onSelectConfigFile Zeigt einen Datei-Auswahldialog an und erzeugt bei Auswahl einer Datei ein passendes Config Objekt.
 */
void MainWindow::onSelectConfigFile() {
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Configuration File"), QDir::currentPath().append("/../connected-robots"), tr("Configuration Files (*.json)"));

    QFileInfo fileInfo = QFileInfo(fileName);
    if (fileInfo.isFile()) {
        this->config = new Config(QFileInfo(fileName));
        ui->label_current_config_file_path->setText(fileInfo.fileName());
        ui->label_current_config_file_path->setStyleSheet("color: rgb(0, 170, 0);");
    } else {
        delete this->config;
        this->config = nullptr;
        ui->label_current_config_file_path->setText("None");
        ui->label_current_config_file_path->setStyleSheet("color: rgb(255, 0, 0);");
    }

    ui->button_robot_connect->setEnabled(this->config != nullptr);
    ui->button_mqtt_connect->setEnabled(this->config != nullptr);
}

/**
 * @brief MainWindow::onConnectRobot Erstellt ein Robot Objekt, welches eine Verbindung zum Roboter herstellt.
 */
void MainWindow::onConnectRobot() {
    if (this->robot != nullptr) {
        // besthende Verbindung zum Roboter trennen
        // TODO: Benutzer fragen, ob er die bestehende Verbindung trennen möchte
        delete this->robot;
    }
    QString ipAddress = ui->lineEdit_robot_ip_address->text();
    qint16 port = ui->lineEdit_robot_port->text().toInt();
    this->robot = new Robot(ipAddress, port, this->config);
    connect(this->robot, &Robot::connected, this, &MainWindow::onRobotConnected);
    connect(this->robot, &Robot::disconnected, this, &MainWindow::onRobotDisconnected);
    connect(this->robot, &Robot::sendData, this, &MainWindow::onRobotSendData);
    connect(this->robot, &Robot::receivedData, this, &MainWindow::onRobotReceivedData);
}

/**
 * @brief MainWindow::onRobotConnected Macht eine erfolgreiche Verbindung mit dem Roboter in der Ui ersichtlich.
 */
void MainWindow::onRobotConnected() {
    ui->label_robot_current_connect_status->setText("Connected");
    ui->label_robot_current_connect_status->setStyleSheet("color: rgb(0, 170, 0);");
}

/**
 * @brief MainWindow::onRobotDisconnected Macht eine Unterbrechung der Verbindung zum Roboter in der Ui ersichtlich.
 */
void MainWindow::onRobotDisconnected() {
    ui->label_robot_current_connect_status->setText("Disconnected");
    ui->label_robot_current_connect_status->setStyleSheet("color: rgb(255, 0, 0);");
    delete this->robot;
    this->robot = nullptr;
}

/**
 * @brief MainWindow::onRobotSendData Zeigt alle Daten an, die an den Roboter gesendet wurden.
 * @param data
 */
void MainWindow::onRobotSendData(QString &data) {
    ui->plainTextEdit_robot_send_to->appendPlainText(data);
}

/**
 * @brief MainWindow::onRobotReceivedData Zeigt vom Roboter empfangene Nachrichten in der Ui an und sendet diese ggfs. weiter an den MQTT Broker.
 * @param data
 */
void MainWindow::onRobotReceivedData(QString &data) {
    ui->plainTextEdit_robot_received_from->appendPlainText(data);

    // Instruction mit der aktuellen Pose des Roboters an den MQTT Broker senden
    bool isSubscriberConnected = this->subscriber != nullptr && this->subscriber->isConnectedToHost();
    bool isRobotConnectedToMqtt = this->ui->checkBox_connect_robot_mqtt->isChecked();
    if (isSubscriberConnected && isRobotConnectedToMqtt) {
        // Daten in Instruction umwandeln
        Instruction instruction;
        QHash<QString, QRegularExpression> regexHashMap = this->config->getRegexHashMap();
        QHash<QString, QRegularExpression>::const_iterator regexIterator = regexHashMap.constBegin();
        while (regexIterator != regexHashMap.constEnd()) {
            QRegularExpression regex = regexIterator.value();
            QString key = regexIterator.key();
            instruction[key] = regex.match(data).captured(1).toDouble();
            ++regexIterator;
        }
        // Instruction senden
        QMQTT::Message message(
                    this->messageId,
                    this->config->getRobotTopic() + "/status/pos",
                    instruction.toJsonString(QJsonDocument::Compact).toUtf8());
        this->subscriber->publish(message);
    }
}

/**
 * @brief MainWindow::onConnectMqtt Erstellt ein Subscriber Objekt, welches eine Verbindung zum MQTT Broker herstellt.
 */
void MainWindow::onConnectMqtt() {
    // falls bereits ein Subscriber existiert, lösche diesen
    if (this->subscriber != nullptr) {
        delete this->subscriber;
    }
    this->subscriber = new Subscriber(QHostAddress(ui->lineEdit_mqtt_ip_address->text()), ui->lineEdit_mqtt_port->text().toInt(), this->config);
    connect(this->subscriber, &Subscriber::connected, this, &MainWindow::onMqttConnected);
    connect(this->subscriber, &Subscriber::disconnected, this, &MainWindow::onMqttDisconnected);
    connect(this->subscriber, &Subscriber::received, this, &MainWindow::onMqttMessageReceived);
    connect(this->subscriber, &Subscriber::published, this, &MainWindow::onMqttMessagePublished);
}

/**
 * @brief MainWindow::onMqttConnected Macht eine erfolgreiche Verbindung mit dem Mqtt Broker in der Ui ersichtlich.
 */
void MainWindow::onMqttConnected() {
    ui->label_mqtt_current_connect_status->setText("Connected");
    ui->label_mqtt_current_connect_status->setStyleSheet("color: rgb(0, 170, 0);");
}

/**
 * @brief MainWindow::onMqttDisconnected Macht eine Unterbrechung der Verbindung zum Mqtt Broker in der Ui ersichtlich.
 */
void MainWindow::onMqttDisconnected() {
    ui->label_mqtt_current_connect_status->setText("Disconnected");
    ui->label_mqtt_current_connect_status->setStyleSheet("color: rgb(255, 0, 0);");
}

/**
 * @brief MainWindow::onMqttMessageReceived Zeigt empfangene MQTT Nachrichten in der Ui an und sendet diese ggfs. weiter an den Roboter.
 * @param message
 */
void MainWindow::onMqttMessageReceived(const QMQTT::Message &message) {
    bool isRobotConnectedToMqtt = ui->checkBox_connect_robot_mqtt->isChecked();
    bool isRobotConnected = this->robot != nullptr;
    if (isRobotConnected && isRobotConnectedToMqtt) {
        // Nachricht an Roboter senden
        translateMqttMessageToRobot(message);
    }

    QString text = message.topic() % ": " % message.payload();
    ui->plainTextEdit_mqtt_received_from->appendPlainText(text);
}

/**
 * @brief MainWindow::onMqttMessagePublished Zeigt vom Subscriber gesendete Nachrichten in der Ui an.
 * @param message
 */
void MainWindow::onMqttMessagePublished(const QMQTT::Message &message) {
    ui->plainTextEdit_mqtt_send_to->appendPlainText(message.payload());
}

/**
 * @brief MainWindow::translateMqttMessageToRobot Übersetzt empfangene MQTT Nachrichten für den Roboter.
 * @param message
 */
void MainWindow::translateMqttMessageToRobot(const QMQTT::Message &message) {
    // übersetze eintreffende MQTT Nachrichten für den Roboter
    QString baseTopic = this->config->getRobotTopic();
    if (message.topic() == baseTopic % "/command/stop") {
        this->robot->stop();
        return;
    }

    QJsonDocument json = QJsonDocument::fromJson(message.payload());
    Instruction instruction(json);
    if (message.topic().endsWith("/command/move")) {
        if (instruction.instruction == "LIN") {
            this->robot->moveLin(instruction.pos1, instruction.value);
        }
        if (instruction.instruction == "PTP") {
            this->robot->movePtp(instruction.pos1, instruction.value);
        }
        if (instruction.instruction == "LINJOINT") {
            this->robot->moveLinJoin(instruction.pos2, instruction.value);
        }
        if (instruction.instruction == "PTPJOINT") {
            this->robot->movePtpJoin(instruction.pos1,instruction.value);
        }
        if (instruction.instruction == "HCLOSE") {
            this->robot->handClose(instruction.value);
        }
        if (instruction.instruction == "HOPEN") {
            this->robot->handOpen(instruction.value);
        }
        if (instruction.instruction == "OVRD") {
            this->robot->override(instruction.value);
        }
        // TODO: weitere Befehle implementieren
    }
}

void MainWindow::onRobotCmdHome() {
    bool isRobotConnected = this->robot != nullptr;
    if (!isRobotConnected) {
        return;
    }

    robot->handOpen(1);
    int value;
    value = 0;
    Position pos;
    pos.x = 0;
    pos.y = 0;
    pos.z = 0;
    pos.a = 0;
    pos.b = 0;
    pos.c = -175;
    robot->moveLin(pos, value);
}
