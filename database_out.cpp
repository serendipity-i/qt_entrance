#include "database_out.h"
#include "ui_database_out.h"

database_out::database_out(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::database_out)
{
    ui->setupUi(this);
    connect(this->ui->search_button,&QPushButton::clicked,this,&database_out::search_data);
    database1 = QSqlDatabase::addDatabase("QSQLITE","");
    database1.setDatabaseName("Entrance.db");
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->setColumnCount(4);
    QStringList header;
    header<<"Number"<<"House Number"<<"Personnel Number"<<"Time";
    ui->tableWidget->setHorizontalHeaderLabels(header);
}

void database_out::search_data()
{
    QString house_number=this->ui->lineEdit->text();
    QString personnel_number=this->ui->lineEdit_2->text();
    if((house_number.length()!=3)||(personnel_number.length()!=6))
    {
    QMessageBox::critical(this,"Error","Fill in the Information Again!","Yes");
    return;
    }
    database1.open();
    int i=0;
    int j=4;
    QSqlQuery data1=QSqlQuery(database1);
    QString sql = QString("select count(*) as i from entrance_guide where house_number == 1% and personnel_number == 2% ").arg(house_number).arg(personnel_number);
    data1.exec(sql);
    if(data1.first())
    {
      i = data1.value("i").toInt();
      ui ->tableWidget->setRowCount(i);
    }
    sql=QString("select * from Entrance wherehouse_number == 1% and personnel_number == 2% ").arg(house_number).arg(personnel_number);
    data1.exec(sql);
    for (int a=0;data1.next();a++)
     {
        for (int b=0;b<j;b++)
            ui->tableWidget->setItem(a,b,new QTableWidgetItem(data1.value(b).toString()));
    }
    database1.close();
}


database_out::~database_out()
{
    delete ui;
}
