#ifndef DATABASE_H
#define DATABASE_H

#include <QWidget>
#include <QMessageBox>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>

namespace Ui {
class database;
}

class database : public QWidget
{
    Q_OBJECT

public:
    explicit database(QWidget *parent = nullptr);
    ~database();
    void search_data();
    void update_table();

private:
    Ui::database *ui;
    QSqlDatabase database1;
    QSqlQuery sqlquery;
};

#endif // DATABASE_H
