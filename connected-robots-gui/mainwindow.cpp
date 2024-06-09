#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QFileDialog>
#include <QStringBuilder>
#include <QQmlEngine>
#include <QQuickItem>
#include "instruction.h"
#include<windows.h>

const QHostAddress DEFAULT_MQTT_HOST_ADDRESS = QHostAddress("149.201.183.150");
const quint16 DEFAULT_MQTT_PORT = 1883;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , subscriber(nullptr)
    , messageId(0)

{
    ui->setupUi(this);

    /* 3D Modelle */
    // initialisiere quickWidget3dRobotMitsu
    QQmlEngine *qmlEngineMitsu = ui->quickWidget3dRobotMitsu->engine();
    qmlEngineMitsu->addImportPath("qrc:/3d/3d");
    ui->quickWidget3dRobotMitsu->setSource(QUrl("qrc:/3d/3d/main.qml"));
    this->qmlRobotMitsu = ui->quickWidget3dRobotMitsu->rootObject()->findChild<QObject*>("robotArm", Qt::FindDirectChildrenOnly);
    // initialisiere quickWidget3dRobotKuka
    QQmlEngine *qmlEngineKuka = ui->quickWidget3dRobotKuka->engine();
    qmlEngineKuka->addImportPath("qrc:/3d/3d");
    ui->quickWidget3dRobotKuka->setSource(QUrl("qrc:/3d/3d/main.qml"));
    this->qmlRobotKuka = ui->quickWidget3dRobotKuka->rootObject()->findChild<QObject*>("robotArm", Qt::FindDirectChildrenOnly);

    /* MQTT */
    // setze Standardwerte in der Ui
    ui->lineEdit_mqtt_ip_address->setText(DEFAULT_MQTT_HOST_ADDRESS.toString());
    ui->lineEdit_mqtt_port->setText(QString::number(DEFAULT_MQTT_PORT));
    // Statusbar
    mqttStatusLabel = new QLabel("Disconnected");
    mqttStatusLabel->setAlignment(Qt::AlignRight);
    mqttStatusLabel->setStyleSheet("color: red;");
    ui->statusBar->addWidget(mqttStatusLabel);
    // Buttons
    connect(ui->button_mqtt_connect, &QPushButton::clicked, this, &MainWindow::onConnectMqtt);
    connect(ui->buttonConfirmOverride, &QPushButton::clicked, this, &MainWindow::onButtonConfirmOverride);

    /* Buttons der Roboter */
    // Mitsubishi
    connect(ui->buttonMitsuAchsPTP, &QPushButton::clicked, this, &MainWindow::onButtonMitsuAchsPTPClicked);
    connect(ui->buttonMitsuAchsLIN, &QPushButton::clicked, this, &MainWindow::onButtonMitsuAchsLINClicked);
    connect(ui->buttonMitsuRaumPTP, &QPushButton::clicked, this, &MainWindow::onButtonMitsuRaumPTPClicked);
    connect(ui->buttonMitsuRaumLIN, &QPushButton::clicked, this, &MainWindow::onButtonMitsuRaumLINClicked);
    connect(ui->buttonMitsuHandClose, &QPushButton::clicked, this, &MainWindow::onButtonMitsuHandCloseClicked);
    connect(ui->buttonMitsuHandOpen, &QPushButton::clicked, this, &MainWindow::onButtonMitsuHandOpenClicked);
    connect(ui->buttonMitsuAus, &QPushButton::clicked, this, &MainWindow::onButtonAusMitsubishiClicked);
    connect(ui->buttonGetMitsuRaum, &QPushButton::clicked, this, &MainWindow::onButtonGetMitsuRaumClicked);
    connect(ui->buttonGetMitsuAchs, &QPushButton::clicked, this, &MainWindow::onButtonGetMitsuAchsClicked);
    // Kuka
    connect(ui->buttonKukaAchsPTP, &QPushButton::clicked, this, &MainWindow::onButtonKukaAchsPTPClicked);
    connect(ui->buttonKukaAchsLIN, &QPushButton::clicked, this, &MainWindow::onButtonKukaAchsLINClicked);
    connect(ui->buttonKukaRaumPTP, &QPushButton::clicked, this, &MainWindow::onButtonKukaRaumPTPClicked);
    connect(ui->buttonKukaRaumLIN, &QPushButton::clicked, this, &MainWindow::onButtonKukaRaumLINClicked);
    connect(ui->buttonKukaHandClose, &QPushButton::clicked, this, &MainWindow::onButtonKukaHandCloseClicked);
    connect(ui->buttonKukaHandOpen, &QPushButton::clicked, this, &MainWindow::onButtonKukaHandOpenClicked);
    connect(ui->buttonKukaAus, &QPushButton::clicked, this, &MainWindow::onButtonAusKukaClicked);
    connect(ui->buttonGetKukaRaum, &QPushButton::clicked, this, &MainWindow::onButtonGetKukaRaumClicked);
    connect(ui->buttonGetKukaAchs, &QPushButton::clicked, this, &MainWindow::onButtonGetKukaAchsClicked);

    /* DEBUGGING */
    // gibt eine Standard Instruction auf der Konsole aus, um den Aufbau für eine MQTT Nachricht zu zeigen
    Instruction exampleInstruction;
    std::cout << exampleInstruction.toJsonString(QJsonDocument::Compact).toStdString() << std::endl;
    // Button um eine Aktion zum Testen auszuführen (in der UI entfernt)
    // connect(ui->pushButton_debug, &QPushButton::clicked, this, &MainWindow::onDebug);
}

MainWindow::~MainWindow() {
    delete ui;
}

// Methode fürs Debuggen
void MainWindow::onDebug(int value) {
    int foo = value;
    foo++;
    QMQTT::Message message(
                this->messageId,
                "mitsubishi/status/pos",
                "{\"instruction\":\"NOP\",\"pos1\":{\"a\":4,\"b\":5,\"c\":6,\"x\":1,\"y\":2,\"z\":3},\"pos2\":{\"a\":45,\"b\":45,\"c\":90,\"x\":45,\"y\":0,\"z\":90},\"pos3\":{\"a\":0,\"b\":0,\"c\":0,\"x\":0,\"y\":0,\"z\":0},\"value\":0}");
    onMqttMessageReceived(message);
}

/**
 * @brief MainWindow::onConnectMqtt Erstellt ein Subscriber Objekt, welches eine Verbindung zum MQTT Broker herstellt.
 */
void MainWindow::onConnectMqtt() {
    // falls bereits ein Subscriber existiert, lösche diesen
    if (this->subscriber != nullptr) {
        delete this->subscriber;
    }
    this->subscriber = new Subscriber(QHostAddress(ui->lineEdit_mqtt_ip_address->text()), ui->lineEdit_mqtt_port->text().toInt());
    connect(this->subscriber, &Subscriber::connected, this, &MainWindow::onMqttConnected);
    connect(this->subscriber, &Subscriber::disconnected, this, &MainWindow::onMqttDisconnected);
    connect(this->subscriber, &Subscriber::received, this, &MainWindow::onMqttMessageReceived);
    connect(this->subscriber, &Subscriber::published, this, &MainWindow::onMqttMessagePublished);
}

/**
 * @brief MainWindow::onMqttConnected Macht eine erfolgreiche Verbindung mit dem Mqtt Broker in der Ui ersichtlich und
 *        abboniert alle relevanten Topics.
 */
void MainWindow::onMqttConnected() {
    mqttStatusLabel->setText("Connected");
    mqttStatusLabel->setStyleSheet("color: rgb(0, 170, 0);"); // grün

    this->subscriber->subscribe(this->kukaTopic + "/#");
    this->subscriber->subscribe(this->mitsubishiTopic + "/#");
}

/**
 * @brief MainWindow::onMqttDisconnected Macht eine Unterbrechung der Verbindung zum Mqtt Broker in der Ui ersichtlich.
 */
void MainWindow::onMqttDisconnected() {
    mqttStatusLabel->setText("Disconnected");
    mqttStatusLabel->setStyleSheet("color: rgb(255, 0, 0);"); // rot
}

/**
 * @brief MainWindow::onMqttMessageReceived Behandelt eine empfangene MQTT Nachricht.
 * @param message
 */
void MainWindow::onMqttMessageReceived(const QMQTT::Message &message) {
    QString text = message.topic() % ": " % message.payload();
    ui->plainTextEdit_mqtt_received_from->appendPlainText(text);

    // falls es sich nicht um einen Status handelt, ignoriere die Nachricht
    if (!message.topic().contains("/status")) {
        return;
    }

    QJsonDocument jsonDoc = QJsonDocument::fromJson(message.payload());
    Instruction instruction(jsonDoc);

    // Message -> Instruction
    if (message.topic().startsWith(this->kukaTopic)) {
        updateStatusLabels(instruction, this->kukaTopic);
        // A1 und A2 für die 3D-Darstellung korrigieren
        instruction.pos2.x -= 30;
        instruction.pos2.y += 90;
        update3dRobotKuka(instruction);
    } else if (message.topic().startsWith(this->mitsubishiTopic)) {
        QJsonDocument jsonDoc = QJsonDocument::fromJson(message.payload());
        Instruction statusMitsubishi(jsonDoc);
        updateStatusLabels(statusMitsubishi, this->mitsubishiTopic);
        update3dRobotMitsu(instruction);
    }
}

/**
 * @brief MainWindow::update3dRobotMitsu Aktualisiert das 3D-Modell vom Mitsubishi
 * @param instruction
 */
void MainWindow::update3dRobotMitsu(Instruction &instruction) {
    qmlRobotMitsu->setProperty("rotation1", instruction.pos2.x);
    qmlRobotMitsu->setProperty("rotation2", instruction.pos2.y);
    qmlRobotMitsu->setProperty("rotation3", instruction.pos2.z);
    qmlRobotMitsu->setProperty("rotation4", instruction.pos2.a);
    qmlRobotMitsu->setProperty("rotation5", instruction.pos2.b);
    qmlRobotMitsu->setProperty("rotation6", instruction.pos2.c);
    // TODO: Greiferstatus aktualisieren -> wird vom Roboter noch nicht gesendet
    // qmlRobotMitsu->setProperty("clawsClosed", false);
}

/**
 * @brief MainWindow::update3dRobotMitsu Aktualisiert das 3D-Modell vom Kuka
 * @param instruction
 */
void MainWindow::update3dRobotKuka(Instruction &instruction) {
    qmlRobotKuka->setProperty("rotation1", instruction.pos2.x);
    qmlRobotKuka->setProperty("rotation2", instruction.pos2.y);
    qmlRobotKuka->setProperty("rotation3", instruction.pos2.z);
    qmlRobotKuka->setProperty("rotation4", instruction.pos2.a);
    qmlRobotKuka->setProperty("rotation5", instruction.pos2.b);
    qmlRobotKuka->setProperty("rotation6", instruction.pos2.c);
    // TODO: Greiferstatus aktualisieren -> wird vom Roboter noch nicht gesendet
    // qmlRobotKuka->setProperty("clawsClosed", false);
}

/**
 * @brief MainWindow::onMqttMessagePublished Zeigt eine ausgehende Nachricht in der UI an.
 *        Wird immer aufgerufen, wenn eine Nachricht an den Broker gesendet wird.
 * @param message
 */
void MainWindow::onMqttMessagePublished(const QMQTT::Message &message) {
    QString text = message.topic() % ": " % message.payload();
    ui->plainTextEdit_mqtt_send_to->appendPlainText(text);
}

/**
 * @brief MainWindow::updateStatusLabels aktualisiert die numerische Darstellung in der UI
 * @param instruction
 * @param topic Topic der eingegangenen Nachricht für die Differenzierung der Roboter
 */
void MainWindow::updateStatusLabels(const Instruction &instruction, const QString &topic){
    // Kuka
    if (topic.startsWith(this->kukaTopic)) {
        // Raumkoordinaten
        ui->recieveposx_kuka->setPlainText(QString::number(instruction.pos1.x));
        ui->recieveposy_kuka->setPlainText(QString::number(instruction.pos1.y));
        ui->recieveposz_kuka->setPlainText(QString::number(instruction.pos1.z));
        ui->recieveposa_kuka->setPlainText(QString::number(instruction.pos1.a));
        ui->recieveposb_kuka->setPlainText(QString::number(instruction.pos1.b));
        ui->recieveposc_kuka->setPlainText(QString::number(instruction.pos1.c));
        // Achswinkel
        ui->recievejointA1_kuka->setPlainText(QString::number(instruction.pos2.x));
        ui->recievejointA2_kuka->setPlainText(QString::number(instruction.pos2.y));
        ui->recievejointA3_kuka->setPlainText(QString::number(instruction.pos2.z));
        ui->recievejointA4_kuka->setPlainText(QString::number(instruction.pos2.a));
        ui->recievejointA5_kuka->setPlainText(QString::number(instruction.pos2.b));
        ui->recievejointA6_kuka->setPlainText(QString::number(instruction.pos2.c));
    // Mitsubishi
    } else if (topic.startsWith(this->mitsubishiTopic)) {
        // Raumkoordinaten
        ui->recieveposx_mitsu->setPlainText(QString::number(instruction.pos1.x));
        ui->recieveposy_mitsu->setPlainText(QString::number(instruction.pos1.y));
        ui->recieveposz_mitsu->setPlainText(QString::number(instruction.pos1.z));
        ui->recieveposa_mitsu->setPlainText(QString::number(instruction.pos1.a));
        ui->recieveposb_mitsu->setPlainText(QString::number(instruction.pos1.b));
        ui->recieveposc_mitsu->setPlainText(QString::number(instruction.pos1.c));
        // Achswinkel
        ui->recievejointA1_mitsu->setPlainText(QString::number(instruction.pos2.x));
        ui->recievejointA2_mitsu->setPlainText(QString::number(instruction.pos2.y));
        ui->recievejointA3_mitsu->setPlainText(QString::number(instruction.pos2.z));
        ui->recievejointA4_mitsu->setPlainText(QString::number(instruction.pos2.a));
        ui->recievejointA5_mitsu->setPlainText(QString::number(instruction.pos2.b));
        ui->recievejointA6_mitsu->setPlainText(QString::number(instruction.pos2.c));
    }
}

/**
 * @brief MainWindow::sendMessage sendet eine Nachricht an den Broker
 * @param message
 * @param topic
 * @details Die Funktion überprüft, ob eine Verbindung zu einem Broker besteht. Wenn keine Verbindung besteht, wird die Nachricht für
 *          Debugging-Zwecke auf der Konsole ausgegeben. Wenn die Synchronisierung aktiviert ist, werden die Nachrichten an beide Roboter gesendet.
 *          Es ist zu beachten, dass derzeit keine Umrechnung der Achswinkel stattfindet, wodurch unerwartete Posen von einem der beiden Roboter
 *          eingenommen werden können, wenn die Synchronisierung aktiviert ist.
 */
void MainWindow::sendMessage(const QString &message, const QString &topic) {
    bool isSubscriberConnected = this->subscriber != nullptr && this->subscriber->isConnectedToHost();
    bool isSyncRobotsChecked = ui->checkBoxSyncRobots->isChecked();

    QString topicForKuka;
    QString topicForMitsu;
    if (isSyncRobotsChecked) {
        QStringList topicParts = topic.split("/");
        topicParts[0] = this->kukaTopic;
        topicForKuka = topicParts.join("/");
        topicParts[0] = this->mitsubishiTopic;
        topicForMitsu = topicParts.join("/");
    }

    // gib bei fehlender Verbindung die Nachricht für Debugging-Zwecke auf der Konsole aus
    if (!isSubscriberConnected) {
        if (isSyncRobotsChecked) {
            QString text = "[sendMessage(disconnected)] " % topicForKuka % ": " % message;
            std::cout << text.toStdString() << std::endl;
            text = "[sendMessage(disconnected)] " % topicForMitsu % ": " % message;
            std::cout << text.toStdString() << std::endl;
        } else {
            QString text = "[sendMessage(disconnected)] " % topic % ": " % message;
            std::cout << text.toStdString() << std::endl;
        }
        return;
    }

    // an beide Roboter senden
    /* ACHTUNG es findet noch KEINE UMRECHNUNG der Achswinkel statt, sodass beim Anfahren von Achswinkeln eine UNERWARTETE POSE
     * von einem der beiden Roboter eingenommen werden kann!) */
    if (isSyncRobotsChecked) {
        QMQTT::Message mqttMessage(this->messageId, topicForKuka, message.toUtf8());
        this->subscriber->publish(mqttMessage); // an Kuka senden
        mqttMessage.setTopic(topicForMitsu);
        this->subscriber->publish(mqttMessage); // an Mitsubishi senden
        return;
    }
    // nur an den ursprünglichen Roboter senden
    QMQTT::Message mqttMessage(this->messageId, topic, message.toUtf8());
    this->subscriber->publish(mqttMessage);
}

/**
 * @brief MainWindow::onButtonConfirmOverride Sendet an beide Roboter den Befehl zum Setzen der Geschwindigkeit
 */
void MainWindow::onButtonConfirmOverride() {
    Instruction instructionOverride;
    instructionOverride.instruction = "OVRD";
    instructionOverride.value = ui->spinBoxOverride->value();

    QString topicMitsubishi = this->mitsubishiTopic + "/command/move";
    QString topicKuka = this->kukaTopic + "/command/move";
    QString jsonString = instructionOverride.toJsonString(QJsonDocument::Compact);
    sendMessage(jsonString, topicMitsubishi);
    sendMessage(jsonString, topicKuka);
}

/* Mitsubishi */

void MainWindow::onButtonAusMitsubishiClicked() {
    Instruction sendAus;
    sendAus.instruction = "STOP";
    QString topic = this->mitsubishiTopic + "/command/stop";
    sendMessage(sendAus.toJsonString(QJsonDocument::Compact), topic);
}

void MainWindow::onButtonMitsuRaumLINClicked() {
    Instruction sendMitsu;
    sendMitsu.instruction="LIN";
    sendMitsu.value = ui->spinBoxOverride->value();
    sendMitsu.pos1.x = ui->sendposx->text().toDouble();
    sendMitsu.pos1.y = ui->sendposy->text().toDouble();
    sendMitsu.pos1.z = ui->sendposz->text().toDouble();
    sendMitsu.pos1.a = ui->sendposa->text().toDouble();
    sendMitsu.pos1.b = ui->sendposb->text().toDouble();
    sendMitsu.pos1.c = ui->sendposc->text().toDouble();

    QString topic = this->mitsubishiTopic + "/command/move";
    sendMessage(sendMitsu.toJsonString(QJsonDocument::Compact), topic);
}

void MainWindow::onButtonMitsuRaumPTPClicked() {
    Instruction sendMitsu;
    sendMitsu.instruction="PTP";
    sendMitsu.value = ui->spinBoxOverride->value();
    sendMitsu.pos1.x = ui->sendposx->text().toDouble();
    sendMitsu.pos1.y = ui->sendposy->text().toDouble();
    sendMitsu.pos1.z = ui->sendposz->text().toDouble();
    sendMitsu.pos1.a = ui->sendposa->text().toDouble();
    sendMitsu.pos1.b = ui->sendposb->text().toDouble();
    sendMitsu.pos1.c = ui->sendposc->text().toDouble();

    QString topic = this->mitsubishiTopic + "/command/move";
    sendMessage(sendMitsu.toJsonString(QJsonDocument::Compact), topic);
}

void MainWindow::onButtonMitsuAchsLINClicked() {
    Instruction sendMitsu;
    sendMitsu.instruction="LINJOINT";
    sendMitsu.value = ui->spinBoxOverride->value();
    sendMitsu.pos2.x = ui->sendJointA1_mitsu->text().toDouble();
    sendMitsu.pos2.y = ui->sendJointA2_mitsu->text().toDouble();
    sendMitsu.pos2.z = ui->sendJointA3_mitsu->text().toDouble();
    sendMitsu.pos2.a = ui->sendJointA4_mitsu->text().toDouble();
    sendMitsu.pos2.b = ui->sendJointA5_mitsu->text().toDouble();
    sendMitsu.pos2.c = ui->sendJointA6_mitsu->text().toDouble();
    QString topic = this->mitsubishiTopic + "/command/move";
    sendMessage(sendMitsu.toJsonString(QJsonDocument::Compact), topic);
}

void MainWindow::onButtonMitsuAchsPTPClicked() {
    Instruction sendMitsu;
    sendMitsu.instruction="PTPJOINT";
    sendMitsu.value = ui->spinBoxOverride->value();
    sendMitsu.pos2.x = ui->sendJointA1_mitsu->text().toDouble();
    sendMitsu.pos2.y = ui->sendJointA2_mitsu->text().toDouble();
    sendMitsu.pos2.z = ui->sendJointA3_mitsu->text().toDouble();
    sendMitsu.pos2.a = ui->sendJointA4_mitsu->text().toDouble();
    sendMitsu.pos2.b = ui->sendJointA5_mitsu->text().toDouble();
    sendMitsu.pos2.c = ui->sendJointA6_mitsu->text().toDouble();

    QString topic = this->mitsubishiTopic + "/command/move";
    sendMessage(sendMitsu.toJsonString(QJsonDocument::Compact), topic);
}

void MainWindow::onButtonMitsuHandCloseClicked() {
    Instruction sendMitsu;
    sendMitsu.instruction = "HCLOSE";
    QString topic = this->mitsubishiTopic + "/command/move";
    sendMessage(sendMitsu.toJsonString(QJsonDocument::Compact), topic);
    // TODO: sollte als Information vom Roboter kommen
    qmlRobotMitsu->setProperty("clawsClosed", true);
}

void MainWindow::onButtonMitsuHandOpenClicked() {
    Instruction sendMitsu;
    sendMitsu.instruction = "HOPEN";
    QString topic = this->mitsubishiTopic + "/command/move";
    sendMessage(sendMitsu.toJsonString(QJsonDocument::Compact), topic);
    // TODO: sollte als Information vom Roboter kommen
    qmlRobotMitsu->setProperty("clawsClosed", false);
}

/* Kuka */

void MainWindow::onButtonAusKukaClicked() {
    Instruction sendAus;
    sendAus.instruction = "STOP";
    QString topic = this->kukaTopic + "/command/stop";
    sendMessage(sendAus.toJsonString(QJsonDocument::Compact), topic);
}

void MainWindow::onButtonKukaRaumLINClicked() {
    Instruction sendKuka;
    sendKuka.instruction="LIN";
    sendKuka.value = ui->spinBoxOverride->value();
    sendKuka.pos1.x = ui->sendposx_kuka->text().toDouble();
    sendKuka.pos1.y = ui->sendposy_kuka->text().toDouble();
    sendKuka.pos1.z = ui->sendposz_kuka->text().toDouble();
    sendKuka.pos1.a = ui->sendposa_kuka->text().toDouble();
    sendKuka.pos1.b = ui->sendposb_kuka->text().toDouble();
    sendKuka.pos1.c = ui->sendposc_kuka->text().toDouble();

    QString topic = this->kukaTopic + "/command/move";
    sendMessage(sendKuka.toJsonString(QJsonDocument::Compact), topic);
}

void MainWindow::onButtonKukaRaumPTPClicked() {
    Instruction sendKuka;
    sendKuka.instruction="PTP";
    sendKuka.value = ui->spinBoxOverride->value();
    sendKuka.pos1.x = ui->sendposx_kuka->text().toDouble();
    sendKuka.pos1.y = ui->sendposy_kuka->text().toDouble();
    sendKuka.pos1.z = ui->sendposz_kuka->text().toDouble();
    sendKuka.pos1.a = ui->sendposa_kuka->text().toDouble();
    sendKuka.pos1.b = ui->sendposb_kuka->text().toDouble();
    sendKuka.pos1.c = ui->sendposc_kuka->text().toDouble();

    QString topic = this->kukaTopic + "/command/move";
    sendMessage(sendKuka.toJsonString(QJsonDocument::Compact), topic);
}

void MainWindow::onButtonKukaAchsLINClicked() {
    Instruction sendKuka;
    sendKuka.instruction="LINJOINT";
    sendKuka.value = ui->spinBoxOverride->value();
    sendKuka.pos1.x = ui->sendJointA1_kuka->text().toDouble();
    sendKuka.pos1.y = ui->sendJointA2_kuka->text().toDouble();
    sendKuka.pos1.z = ui->sendJointA3_kuka->text().toDouble();
    sendKuka.pos1.a = ui->sendJointA4_kuka->text().toDouble();
    sendKuka.pos1.b = ui->sendJointA5_kuka->text().toDouble();
    sendKuka.pos1.c = ui->sendJointA6_kuka->text().toDouble();

    QString topic = this->kukaTopic + "/command/move";
    sendMessage(sendKuka.toJsonString(QJsonDocument::Compact), topic);
}

void MainWindow::onButtonKukaAchsPTPClicked() {
    Instruction sendKuka;
    sendKuka.instruction="PTPJOINT";
    sendKuka.value = ui->spinBoxOverride->value();
    sendKuka.pos1.x = ui->sendJointA1_kuka->text().toDouble();
    sendKuka.pos1.y = ui->sendJointA2_kuka->text().toDouble();
    sendKuka.pos1.z = ui->sendJointA3_kuka->text().toDouble();
    sendKuka.pos1.a = ui->sendJointA4_kuka->text().toDouble();
    sendKuka.pos1.b = ui->sendJointA5_kuka->text().toDouble();
    sendKuka.pos1.c = ui->sendJointA6_kuka->text().toDouble();

    QString topic = this->kukaTopic + "/command/move";
    sendMessage(sendKuka.toJsonString(QJsonDocument::Compact), topic);
}

void MainWindow::onButtonKukaHandCloseClicked() {
    Instruction sendKuka;
    sendKuka.instruction = "HCLOSE";
    QString topic = this->kukaTopic + "/command/move";
    sendMessage(sendKuka.toJsonString(QJsonDocument::Compact), topic);
    // TODO: sollte als Information vom Roboter kommen
    qmlRobotKuka->setProperty("clawsClosed", true);
}

void MainWindow::onButtonKukaHandOpenClicked() {
    Instruction sendKuka;
    sendKuka.instruction = "HOPEN";
    QString topic = this->kukaTopic + "/command/move";
    sendMessage(sendKuka.toJsonString(QJsonDocument::Compact), topic);
    // TODO: sollte als Information vom Roboter kommen
    qmlRobotKuka->setProperty("clawsClosed", false);
}


void MainWindow::onButtonGetMitsuRaumClicked() {
    ui->sendposx_kuka->setText(ui->recieveposx_mitsu->toPlainText());
    ui->sendposy_kuka->setText(ui->recieveposy_mitsu->toPlainText());
    ui->sendposz_kuka->setText(ui->recieveposz_mitsu->toPlainText());
    ui->sendposa_kuka->setText(ui->recieveposa_mitsu->toPlainText());
    ui->sendposb_kuka->setText(ui->recieveposb_mitsu->toPlainText());
    ui->sendposc_kuka->setText(ui->recieveposc_mitsu->toPlainText());
}

void MainWindow::onButtonGetKukaRaumClicked() {
    ui->sendposx->setText(ui->recieveposx_kuka->toPlainText());
    ui->sendposy->setText(ui->recieveposy_kuka->toPlainText());
    ui->sendposz->setText(ui->recieveposz_kuka->toPlainText());
    ui->sendposa->setText(ui->recieveposa_kuka->toPlainText());
    ui->sendposb->setText(ui->recieveposb_kuka->toPlainText());
    ui->sendposc->setText(ui->recieveposc_kuka->toPlainText());
}

void MainWindow::onButtonGetMitsuAchsClicked() {
    /*Die Achskoordinaten sind bei beiden Robotern etwas unterschiedlich.
      Um etwa die gleiche Pose zu erzeugen findet hier eine Umrechnung für A1 und A2 statt.*/
    double correctedA1 = ui->recievejointA1_mitsu->toPlainText().toDouble() + 30.0;
    double correctedA2 = ui->recievejointA2_mitsu->toPlainText().toDouble() - 90.0;
    ui->sendJointA1_kuka->setText(QString::number(correctedA1));
    ui->sendJointA2_kuka->setText(QString::number(correctedA2));
    ui->sendJointA3_kuka->setText(ui->recievejointA3_mitsu->toPlainText());
    ui->sendJointA4_kuka->setText(ui->recievejointA4_mitsu->toPlainText());
    ui->sendJointA5_kuka->setText(ui->recievejointA5_mitsu->toPlainText());
    ui->sendJointA6_kuka->setText(ui->recievejointA6_mitsu->toPlainText());
}

void MainWindow::onButtonGetKukaAchsClicked() {
    /*Die Achskoordinaten sind bei beiden Robotern etwas unterschiedlich.
      Um etwa die gleiche Pose zu erzeugen findet hier eine Umrechnung für A1 und A2 statt.*/
    double correctedA1 = ui->recievejointA1_kuka->toPlainText().toDouble() - 30.0;
    double correctedA2 = ui->recievejointA2_kuka->toPlainText().toDouble() + 90.0;
    ui->sendJointA1_mitsu->setText(QString::number(correctedA1));
    ui->sendJointA2_mitsu->setText(QString::number(correctedA2));
    ui->sendJointA3_mitsu->setText(ui->recievejointA3_kuka->toPlainText());
    ui->sendJointA4_mitsu->setText(ui->recievejointA4_kuka->toPlainText());
    ui->sendJointA5_mitsu->setText(ui->recievejointA5_kuka->toPlainText());
    ui->sendJointA6_mitsu->setText(ui->recievejointA6_kuka->toPlainText());
}

/**
 * @brief MainWindow::on_start_predefined_clicked Hier sollte eine vordefinierte Abfolge von Punkten angefahren werden.
 *        Funktioniert so noch nicht.
 */
void MainWindow::on_start_predefined_clicked() {
    Instruction sendKuka;
    sendKuka.instruction="PTP";
    sendKuka.pos1.x = 1;
    sendKuka.pos1.y = 0;
    sendKuka.pos1.z = 0;
    sendKuka.pos1.a = 0;
    sendKuka.pos1.b = 0;
    sendKuka.pos1.c = 0;

    QString topic = this->kukaTopic + "/command/move";
    sendMessage(sendKuka.toJsonString(QJsonDocument::Compact), topic);


    Instruction sendMitsu;
    sendMitsu.instruction="PTP";
    sendMitsu.pos1.x = 1;
    sendMitsu.pos1.y = 0;
    sendMitsu.pos1.z = 0;
    sendMitsu.pos1.a = 0;
    sendMitsu.pos1.b = 0;
    sendMitsu.pos1.c = 0;

    topic = this->mitsubishiTopic + "/command/move";
    sendMessage(sendMitsu.toJsonString(QJsonDocument::Compact), topic);

    // sleep 2000 milliseconds
    Sleep(2000);
    // repetion 2

    sendKuka.instruction="PTP";
    sendKuka.pos1.x = 3;
    sendKuka.pos1.y = 0;
    sendKuka.pos1.z = 0;
    sendKuka.pos1.a = 0;
    sendKuka.pos1.b = 0;
    sendKuka.pos1.c = 0;
    topic = this->kukaTopic + "/command/move";
    sendMessage(sendKuka.toJsonString(QJsonDocument::Compact), topic);
    //TODO: senden


    sendMitsu.instruction="PTP";
    sendMitsu.pos1.x = 3;
    sendMitsu.pos1.y = 0;
    sendMitsu.pos1.z = 0;
    sendMitsu.pos1.a = 0;
    sendMitsu.pos1.b = 0;
    sendMitsu.pos1.c = 0;

    topic = this->mitsubishiTopic + "/command/move";
    sendMessage(sendMitsu.toJsonString(QJsonDocument::Compact), topic);
}

void MainWindow::on_stop_predefined_clicked() {
    Instruction sendAus;
    sendAus.instruction = "STOP";
    QString topic = this->kukaTopic + "/command/stop";
    sendMessage(sendAus.toJsonString(QJsonDocument::Compact), topic);
    topic = this->mitsubishiTopic + "/command/stop";
    sendMessage(sendAus.toJsonString(QJsonDocument::Compact), topic);
}
