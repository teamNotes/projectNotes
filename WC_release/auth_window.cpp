#include "auth_window.h"
#include "ui_auth_window.h"

auth_window::auth_window(QDialog *parent) :
    QDialog(parent),
    ui(new Ui::auth_window)
{
    ui->setupUi(this);
    ui->loginPushButton->setFixedHeight(90);
    ui->loginPushButton->setFixedWidth(230);
    ui->loginPushButton->setStyleSheet("QPushButton {color: rgb(120, 65, 81); background-color: white; border-width: 1px; font: italic 12pt \"Montserrat\"; border-color: white; border-radius: 45px;}");
    ui->registerPushButton_2->setText("Регистрация");
    ui->registerPushButton_2->setStyleSheet("QPushButton {color: white; background-color: rgba(255, 255, 255, 0); border-width: 1px; font: italic 12pt \"Montserrat\"; border-style: solid;  border-color: white; border-radius: 45px;}");
    ui->registerPushButton_2->setFixedHeight(90);
    ui->registerPushButton_2->setFixedWidth(230);
    ui->lineEdit->setStyleSheet("QLineEdit {color: white; background-color: rgba(255, 255, 255, 0); font: italic 12pt \"Montserrat\"; border-top-width: 0px;  border-style: solid; border-color: white; border-bottom-width: 1px;}");
    ui->lineEdit_2->setStyleSheet("QLineEdit {color: white; background-color: rgba(255, 255, 255, 0); font: italic 12pt \"Montserrat\"; border-top-width: 0px;  border-style: solid; border-color: white; border-bottom-width: 1px;}");
}

auth_window::~auth_window()
{
    delete ui;
}

void auth_window::on_lineEdit_textEdited(const QString &arg1)
{
    auth_window::m_username = arg1;
}

void auth_window::on_lineEdit_2_textEdited(const QString &arg1)
{
    auth_window::m_userpass = arg1;
}

void auth_window::on_loginPushButton_clicked()
{
    emit login_button_clicked();
}

void auth_window::on_registerPushButton_2_clicked()
{
    emit register_button_clicked();
}

QString auth_window::getLogin()
{
    return auth_window::m_username;
}

QString auth_window::getPass()
{
    return auth_window::m_userpass;
}
