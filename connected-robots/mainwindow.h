#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "robot.h"
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
    QFileInfo configFileInfo;
    Config* config;
    Robot* robot;
    Subscriber* subscriber;
    uint16_t messageId;
    void ptphome();
    void translateMqttMessageToRobot(const QMQTT::Message &message);

public slots:
    void onSelectConfigFile();

    void onConnectRobot();
    void onRobotConnected();
    void onRobotDisconnected();
    void onRobotCmdHome();
    void onRobotReceivedData(QString &data);
    void onRobotSendData(QString &data);

    void onConnectMqtt();
    void onMqttConnected();
    void onMqttDisconnected();
    void onMqttMessageReceived(const QMQTT::Message &message);
    void onMqttMessagePublished(const QMQTT::Message &message);

signals:
    void sendData(QString data);

};
#endif // MAINWINDOW_H
