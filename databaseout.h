#ifndef DATABASEOUT_H
#define DATABASEOUT_H

#include <QWidget>
#include <QMessageBox>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QString>

namespace Ui {
class databaseout;
}

class databaseout : public QWidget
{
    Q_OBJECT

public:
    explicit databaseout(QWidget *parent = nullptr);
    ~databaseout();
    void search_data();
    void update_table();


private slots:
    void on_search_button_clicked();

private:
    Ui::databaseout *ui;
    QSqlDatabase database1;
    QSqlQuery sqlquery;
};

#endif // DATABASEOUT_H
