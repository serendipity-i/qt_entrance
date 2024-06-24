#include "databaseout.h"
#include "ui_databaseout.h"

databaseout::databaseout(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::databaseout)
{
    ui->setupUi(this);
    connect(this->ui->search_button,SIGNAL(clicked()),this,SLOT(on_search_button_clicked()));
    database1 = QSqlDatabase::addDatabase("QSQLITE","looksql");
    database1.setDatabaseName("Entrance.db");
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->setColumnCount(4);
    QStringList header;
    header<<"Number"<<"House Number"<<"Personnel Number"<<"Time";
    ui->tableWidget->setHorizontalHeaderLabels(header);
}


databaseout::~databaseout()
{
    delete ui;
}

void databaseout::on_search_button_clicked()
{
    QString house_number=this->ui->lineEdit->text();
    QString personnel_number=this->ui->lineEdit_2->text();
    if((house_number.length()!=3)||(personnel_number.length()!=6))
    {
    QMessageBox::critical(this,"Error","Fill in the Information Again!","Yes");
    return;
    }
    else{
    database1.open();
    int row=0;
    int col=4;
    QSqlQuery data1=QSqlQuery(database1);
    QString sql = QString("select count(*) as row from entrance_guard where house_number == 1% and personnel_number == 2% ").arg(house_number).arg(personnel_number);
    data1.exec(sql);
    if(data1.first())
    {
      row = data1.value("row").toInt();
      ui ->tableWidget->setRowCount(row);
    }
    sql=QString("select * from entrance_guard where house_number == 1% and personnel_number == 2% ").arg(house_number).arg(personnel_number);
    data1.exec(sql);
    for (int a=0;data1.next();a++)
     {
        for (int b=0;b<col;b++)
            ui->tableWidget->setItem(a,b,new QTableWidgetItem(data1.value(b).toString()));
    }
    database1.close();
    }
}

void databaseout::update_table()
{
    if(database1.open())
    {
        int row=0;
        int col=4;
        QSqlQuery data1=QSqlQuery(database1);
        QString sql = QString("select count(*) as row from entrance_guard ");
        data1.exec(sql);
        if(data1.first())
        {
          row = data1.value("row").toInt();
          ui ->tableWidget->setRowCount(row);
        }
        sql=QString("select * from entrance_guard ");
        data1.exec(sql);
        for (int a1=0;data1.next();a1++)
         {
            for (int b1=0;b1<col;b1++)
                ui->tableWidget->setItem(a1,b1,new QTableWidgetItem(data1.value(b1).toString()));
        }
        database1.close();
    }
}
