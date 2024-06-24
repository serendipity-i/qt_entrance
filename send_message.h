#ifndef SEND_MESSAGE_H
#define SEND_MESSAGE_H

#include <QWidget>
#include <QString>
#include <QMessageBox>
#include <QDebug>
#include <QDateTime>

namespace Ui {
class send_message;
}

class send_message : public QWidget
{
    Q_OBJECT

public:
    explicit send_message(QWidget *parent = nullptr);
    ~send_message();
signals:
    void Message_to_Socket(QString message);
    void Whether_space(bool whether_space, int interval);
private slots:
    void on_send_button_clicked();

private:
    Ui::send_message *ui;
};

#endif // SEND_MESSAGE_H
