#include "send_message.h"
#include "ui_send_message.h"

send_message::send_message(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::send_message)
{
    ui->setupUi(this);
    connect(ui->send_button,SIGNAL(clicked()),this,SLOT(on_send_button_clicked));

    connect(ui->cancel_button,&QPushButton::clicked,this,&send_message::close);
}

void send_message::on_send_button_clicked()
{
        QString house_number=this->ui->house_number->toPlainText();
        QString personnel_number=this->ui->personnel_number->toPlainText();
        QString interval=this->ui->interval_text->toPlainText();
        bool interval_index=(this->ui->comboBox->currentIndex()==0);
        if((house_number.length()!=3)||(personnel_number.length()!=6))
        {
            QMessageBox::critical(this,"Error","Fill in the Information Again!");
            return;
        }
        else if ((interval_index==true) && (interval.length()==0))
        {
            QMessageBox::critical(this,"Error","Fill in the Interval!");
            return;
        }
        QString current_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        QString allmessage=house_number+","+personnel_number+","+current_time;
        emit Whether_space(interval_index,interval.toInt());
        emit Message_to_Socket(allmessage);
        this->close();
}

send_message::~send_message()
{
    delete ui;
}



