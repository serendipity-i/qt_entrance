#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QString>
#include <QMessageBox>
#include <QTcpServer>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QDebug>
#include <QHostAddress>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QTimer>
#include "send_message.h"
#include "databaseout.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void Start_button();
    void Connect_button();
    void Register_button();
    void Database_button();
    void Select_protocol();
    void Send_button();
    void Stop_button();
    void Clear_button();
    void Receive_message(QString message);
    void Receive_interval(bool whether_s, int interval);
    void client_read();
    void server_read();
    void client_receive();
    void Send_button_2();
    void message_space();

private:
    Ui::Widget *ui;
    QTcpServer m_tcp;
    QTcpSocket *iqtcpsocket;
    QTcpSocket* m_tcpsocket;
    send_message *register_person;
    void Send(bool whether_space,int interval);
    void Savedata(QString message);
    void addmessage(QStringList subs);
    bool IsTable(QSqlQuery &data1,QString table);
    bool cTable(QSqlQuery &data1);
    QSqlDatabase database1;
    QTimer *timer;
    QTimer *timer_udp;
    QString trmessage;
    QSqlQuery qsqlquery;
    bool whether_space;
    bool whether_connect=false;
    bool whether_connect_udp=false;
    QUdpSocket* uclient;
    QUdpSocket* userver;
    int tspace;
    int current_state;
};
#endif // WIDGET_H
