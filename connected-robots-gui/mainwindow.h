#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "instruction.h"
#include "qlabel.h"
#include "subscriber.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QLabel *mqttStatusLabel;
    Subscriber* subscriber;
    uint16_t messageId;
    QObject *qmlRobotMitsu; // Referenz des 3D-Modells vom Mitsubishi
    QObject *qmlRobotKuka; // Referenz des 3D-Modells vom Kuka
    const QString mitsubishiTopic = "mitsubishi";
    const QString kukaTopic = "kuka";

    void sendMessage(const QString &message, const QString &topic);
    void updateStatusLabels(const Instruction &instruction, const QString &topic);
    void update3dRobotMitsu(Instruction &instruction);
    void update3dRobotKuka(Instruction &instruction);

public slots:
    void onDebug(int value);
    void onConnectMqtt();
    void onMqttConnected();
    void onMqttDisconnected();
    void onMqttMessageReceived(const QMQTT::Message &message);
    void onMqttMessagePublished(const QMQTT::Message &message);

signals:
    void sendMqttMessage(const QMQTT::Message &message);


private slots:
    void onButtonConfirmOverride();
    void onButtonAusMitsubishiClicked();
    void onButtonAusKukaClicked();
    void onButtonMitsuRaumLINClicked();
    void onButtonMitsuRaumPTPClicked();
    void onButtonMitsuAchsLINClicked();
    void onButtonMitsuAchsPTPClicked();
    void onButtonMitsuHandOpenClicked();
    void onButtonMitsuHandCloseClicked();
    void onButtonKukaRaumLINClicked();
    void onButtonKukaRaumPTPClicked();
    void onButtonKukaAchsLINClicked();
    void onButtonKukaAchsPTPClicked();
    void onButtonKukaHandOpenClicked();
    void onButtonKukaHandCloseClicked();
    void onButtonGetMitsuRaumClicked();
    void onButtonGetKukaRaumClicked();
    void onButtonGetKukaAchsClicked();
    void onButtonGetMitsuAchsClicked();
    void on_start_predefined_clicked();
    void on_stop_predefined_clicked();
};
#endif // MAINWINDOW_H
