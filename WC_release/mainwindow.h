#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include "auth_window.h"
#include <QtSql/QtSql>
#include "reg_window.h"
#include <QWidget>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QDialog *parent = nullptr);
    ~MainWindow();
    void display();                                                 //прототип функции отображения
    bool connectDB();
    void wipeDB();

private:
    Ui::MainWindow *ui_Main;
    Ui::MainWindow* mainLayout;
    auth_window ui_Auth;                                            //композит - экземпляр класса Auth_Window
    reg_window ui_Reg;
    QString m_username;
    QString m_userpass;
    QString db_input;
    QSqlDatabase mw_db;

    int user_counter;
    bool m_loginSuccesfull;
private slots:
    void authorizeUser();
    void registerWindowShow();
    void registerUser();
    void on_pushButton_clicked();
};

#endif // MAINWINDOW_H
