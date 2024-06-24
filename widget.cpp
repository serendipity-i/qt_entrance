#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    current_state=this->ui->protocol_select->currentIndex();

    m_tcpsocket=new QTcpSocket(this);
    userver=new QUdpSocket(this);
    uclient=new QUdpSocket(this);
    iqtcpsocket = new QTcpSocket();
    timer = new QTimer(this);
    timer_udp=new QTimer(this);
    register_person = new send_message();


    connect(ui->start_button,&QPushButton::clicked,this,&Widget::Start_button);
    connect(ui->connect_button,&QPushButton::clicked,this,&Widget::Connect_button);
    connect(this->ui->protocol_select,SIGNAL(currentIndexChanged(int)),this,SLOT(Select_protocol()));
    connect(ui->register_button,&QPushButton::clicked,this,&Widget::Register_button);
    connect(ui->send_button,&QPushButton::clicked,this,&Widget::Send_button);
    connect(ui->stop_button,&QPushButton::clicked,this,&Widget::Stop_button);
    connect(ui->database_button,&QPushButton::clicked,this,&Widget::Database_button);
    connect(ui->send_button_2,&QPushButton::clicked,this,&Widget::Send_button_2);
    connect(ui->clean_button,&QPushButton::clicked,this,&Widget::Clear_button);
    connect(uclient,&QUdpSocket::readyRead,this,&Widget::client_read);
    connect(userver,&QUdpSocket::readyRead,this,&Widget::server_read);
    connect(&m_tcp,&QTcpServer::newConnection,this,&Widget::client_receive);
    connect(iqtcpsocket,&QTcpSocket::readyRead,this,&Widget::client_read);
    connect(register_person,&send_message::Message_to_Socket,this,&Widget::Receive_message);
    connect(register_person,&send_message::Whether_space,this,&Widget::Receive_interval);

    database1 = QSqlDatabase::addDatabase("QSQLITE","Write");
    database1.setDatabaseName("Entrance.db");
    if (database1.open())
    {
        qDebug() << "Success!";
    }
    else
    {
        qDebug() << "Error!";
    }
    qsqlquery=QSqlQuery(database1);
    bool t_exist=IsTable(qsqlquery,"entrance_guard");
    if (t_exist==false)
    {
        qDebug() << "Create";
        cTable(qsqlquery);
    }
    database1.close();

    this->ui->IP_text->setPlainText("127.0.0.1");
    this->ui->port_text->setPlainText("8888");
    this->ui->port_text_2->setPlainText("8888");
}

bool Widget::IsTable(QSqlQuery &data1, QString table)
{
    QString sql = QString("select * from sqlite_master where name = '%1'").arg(table);
    data1.exec(sql);
    return data1.next();
}

bool Widget::cTable(QSqlQuery &data1)
{
    return data1.exec("Create Entrance("
                     "EG INTEGER PRIMARY KEY AUTOINCREMENT"
                     "house_number VARCHAR(40) NOT NULL."
                     "personnel_number VARCHAR(40) NOT NULL."
                     "Time VARCHAR(40) NOT NULL)");
}

void Widget::Start_button()
{
    if(current_state==0)
    {
        m_tcp.listen(QHostAddress::AnyIPv4,ui->port_text_2->toPlainText().toInt());
        qDebug() << "Start TCP";
    }
    else
    {
        if(ui->port_text_2->toPlainText().toInt()==ui->port_text->toPlainText().toInt())
        {
            QMessageBox::critical(this,"ERROR","Port Number Cannot Be the Same","Yes");
            return;
        }
        whether_connect_udp=true;
        userver->bind(ui->port_text_2->toPlainText().toInt());
        uclient->bind(ui->port_text->toPlainText().toInt());
        qDebug() << "Start UDP";
    }
}

void Widget::Connect_button()
{
    iqtcpsocket->abort();
    iqtcpsocket->connectToHost(this->ui->IP_text->toPlainText(),this->ui->port_text->toPlainText().toInt());
    qDebug() << "Trying to Connect";
}

void Widget::Register_button()
{
    register_person->show();
}

void Widget::Select_protocol()
{
    current_state=this->ui->protocol_select->currentIndex();
    Stop_button();
    QMessageBox::critical(this,"","Interrupted");
    qDebug() << "TCP_state" << whether_connect;
    qDebug() << "UDP_state" << whether_connect_udp;
}

void Widget::Send_button()
{
    Send(whether_space,tspace);
}

void Widget::Send(bool whether_space, int interval)
{
    if(current_state==0)
    {
        if (whether_connect==false)
        {
            QMessageBox::critical(this,"Error!","Please link to the server first");
            return;
        }
        if(whether_space==false)
        {
            iqtcpsocket->write(trmessage.toUtf8());
            ui->textEdit->append(trmessage);
        }
        else
        {
            connect(timer,SIGNAL(timeout()),this,SLOT(message_space()));
            interval=1000*interval;
            timer->start(interval);
        }
    }
    else
    {
        if(whether_connect_udp==false)
        {
            QMessageBox::critical(this,"Erroe!","Please link to the server first");
            return;
        }
        if(whether_space==false)
        {
            uclient->writeDatagram(trmessage.toUtf8(),QHostAddress(this->ui->IP_text->toPlainText()),this->ui->port_text_2->toPlainText().toInt());
        }
        else
        {
            connect(timer_udp,SIGNAL(timeout()),this,SLOT(message_space()));
            interval=1000*interval;
            timer_udp->start(interval);
        }
    }
}

void Widget::message_space()
{
    if(current_state==0)
    {
        iqtcpsocket->write(trmessage.toUtf8());
        ui->textEdit->append(trmessage);
    }
    else
    {
        ui->textEdit->append(trmessage);
        uclient->writeDatagram(trmessage.toUtf8(),QHostAddress(this->ui->IP_text->toPlainText()),this->ui->port_text_2->toPlainText().toInt());
    }
}

void Widget::Stop_button()
{
    timer->stop();
    timer_udp->stop();
}

void Widget::Send_button_2()
{
    QString message1=this->ui->textEdit_4->toPlainText();
    if(current_state==0)
    {
        if(whether_connect==false)
        {
            QMessageBox::critical(this,"Error","Please link to the server first");
            return;
        }
        m_tcpsocket->write(message1.toUtf8());
    }
    else
    {
        userver->writeDatagram(message1.toUtf8(),QHostAddress(this->ui->IP_text->toPlainText()),this->ui->port_text->toPlainText().toInt());
    }
}

void Widget::Clear_button()
{
    this->ui->textEdit_4->clear();
}

void Widget::Database_button()
{
    databaseout* databaseout11 =new databaseout();
    databaseout11->show();
    //databaseout11->update_table();
}

void Widget::Receive_interval(bool whether_s , int interval)
{
    whether_space=whether_s;
    tspace=interval;
}

void Widget::Savedata(QString message)
{
    message=message.trimmed();
    QStringList subs = message.split(",");
    addmessage(subs);
}

void Widget::addmessage(QStringList subs)
{
    database1.open();
    QSqlQuery data1=QSqlQuery(database1);
    data1.prepare("INSERT INTO Entrance(house_number,personnel_number,Time)"
                      "VALUES(x,x,x)");
    data1.addBindValue(subs.at(0));
    data1.addBindValue(subs.at(1));
    data1.addBindValue(subs.at(2));
    data1.exec();
    database1.close();
}

void Widget::client_receive()
{
    m_tcpsocket=m_tcp.nextPendingConnection();
    connect(m_tcpsocket,&QTcpSocket::disconnected,
            m_tcpsocket,&QTcpSocket::deleteLater);
    m_tcpsocket->write("Connect");
    connect(m_tcpsocket,&QTcpSocket::readyRead,
            this,&Widget::server_read);
    whether_connect=true;
    qDebug() << "Success";
}

void Widget::server_read()
{
    if(current_state==0)
    {
        QString message1=m_tcpsocket->readAll();
        Savedata(message1);
        ui->textEdit_3->append(message1);
    }
    else
    {
        QByteArray buffer;
        buffer.resize(userver->pendingDatagramSize());
        QHostAddress oppo_addr;
        quint16 port;
        userver->readDatagram(buffer.data(),buffer.size(),&oppo_addr,&port);
        Savedata(buffer);
        ui->textEdit_3->append(buffer);
    }
}

void Widget::client_read()
{
    if(current_state==0)
    {
        QString message1 = iqtcpsocket->readAll();
        this->ui->textEdit_2->append(message1);
    }
    else
    {
        QByteArray buffer;
        buffer.resize(uclient->pendingDatagramSize());
        uclient->readDatagram(buffer.data(),buffer.size());
        this->ui->textEdit_2->append(buffer);
    }
}

void Widget::Receive_message(QString message)
{
    trmessage = message;
    qDebug() << trmessage;
}








Widget::~Widget()
{
    delete ui;
}

