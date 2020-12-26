#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "auth_window.h"
#include "reg_window.h"
#include <QDebug>
#include <QWidget>
#include <QLayout>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QDesktopWidget>
#include <QPoint>



MainWindow::MainWindow(QWidget *parent) :               //реализация конструктора главного окна
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    ui_Auth(nullptr),
    ui_Reg(nullptr)//список инициализации

{
    /*QDesktopWidget desktop;
    QRect rect = desktop.availableGeometry(this);
    QPoint center = rect.center();
    int x = center.x() - (width()/2);
    int y = center.y() - (height()/2);
    center.setX(x);
    center.setY(y);
    move(center);*/

    /*QRect desktopRect = QApplication::desktop()->availableGeometry(this);
    QPoint center = desktopRect.center();

    move(center.x()-width()*0.5, center.y()-height()*0.5);*/

    ui->setupUi(this);  //установка интерфейса главного окна

    lineEdits = QList<QLineEdit*>()
            << ui->lineEdit
            << ui->lineEdit2
            << ui->lineEdit3
            << ui->lineEdit4
            << ui->lineEdit5
            << ui->lineEdit6
            << ui->lineEdit7
            << ui->lineEdit8
            << ui->lineEdit9;

    checkBoxes = QList<QCheckBox*>()
            << ui->checkBox
            << ui->checkBox2
            << ui->checkBox3
            << ui->checkBox4
            << ui->checkBox5
            << ui->checkBox6
            << ui->checkBox7
            << ui->checkBox8
            << ui->checkBox9;

    labels = QList<QLabel*>()
            << ui->label
            << ui->label2
            << ui->label3
            << ui->label4
            << ui->label5
            << ui->label6
            << ui->label7
            << ui->label8
            << ui->label9;


    user_id = 0;
    changed = false;
    m_loginSuccesfull = false;

    ui_Reg = new reg_window();
    ui_Auth = new auth_window();

    lastDateStr = ui->calendarWidget->selectedDate().toString("dd-MM-yyyy");

    connect(ui_Reg, &reg_window::register_button_clicked2, //соединение кнопки регистрации экземпляра окна авторизации
            this,&MainWindow::registerUser);                 //со слотом-обработчиком регистраци
    connect(ui_Reg,&reg_window::destroyed,
            ui_Auth, &auth_window::show);

    connect(ui_Auth, &auth_window::login_button_clicked,   //соединение сигнала кнопки авторизации экземпляра окна авторизации
            this, &MainWindow::authorizeUser);               //со слотом-обработчиком авторизации
    connect(ui_Auth, &auth_window::destroyed,               //соединение сигнала уничтожения экземпляра окна авторизации
            this, &MainWindow::show);                        //с методом отображения главного окна

    connect(ui_Auth, &auth_window::register_button_clicked, //соединение сигнала кнопки регистрации экземпляра окна авторизации
            this,&MainWindow::registerWindowShow);           //со слотом вызывающим окно регистрации

    // Чекбоксы
    connect(ui->checkBox,  &QCheckBox::clicked , this, &MainWindow::on_checkBoxes_clicked);
    connect(ui->checkBox2,  &QCheckBox::clicked, this, &MainWindow::on_checkBoxes_clicked);
    connect(ui->checkBox3,  &QCheckBox::clicked, this, &MainWindow::on_checkBoxes_clicked);
    connect(ui->checkBox4,  &QCheckBox::clicked, this, &MainWindow::on_checkBoxes_clicked);
    connect(ui->checkBox5,  &QCheckBox::clicked, this, &MainWindow::on_checkBoxes_clicked);
    connect(ui->checkBox6,  &QCheckBox::clicked, this, &MainWindow::on_checkBoxes_clicked);
    connect(ui->checkBox7,  &QCheckBox::clicked, this, &MainWindow::on_checkBoxes_clicked);
    connect(ui->checkBox8,  &QCheckBox::clicked, this, &MainWindow::on_checkBoxes_clicked);
    connect(ui->checkBox9,  &QCheckBox::clicked, this, &MainWindow::on_checkBoxes_clicked);
    connect(ui->checkBox10,  &QCheckBox::clicked, this, &MainWindow::on_checkBoxes_clicked);
  //  connect(ui->btnAddNote, &QPushButton::clicked, this, &MainWindow::on_btnAddNote_clicked);

    //connect(&ui_Auth,SIGNAL(destroyed()), this, SLOT(AuthAquired()));

}

void MainWindow::authorizeUser()
{
   username          = ui_Auth->getLogin();
   userpass          = ui_Auth->getPass();

   queryStr = "SELECT * "
              "FROM users2 "
              "WHERE name = '%1'";
   QString uname       = "";
   QString upass       = "";
   int xPos            = 0;
   int yPos            = 0;
   int width           = 0;
   int length          = 0;

    QStringList tables = db.tables();
    qDebug() << "table " << tables << db.isValid() << db.isOpen();
    queryStr = queryStr.arg(username);

    QSqlQuery query(queryStr, db);
    if(!query.exec())
    {
        qDebug() << "Unable to execute query - exiting" << query.lastError() << " : " << query.lastQuery();
        QMessageBox::critical(this, tr("Error"),tr("Error to get user info from table users in database %1. %2!")
                              .arg(dbName)
                              .arg(db.lastError().text()));
        ::exit(EXIT_FAILURE);
    }


    bool nameExists = query.next();
    qDebug() << query.lastError() << nameExists;
    if(!nameExists)
    {
        m_loginSuccesfull = false;
        qDebug() << "User doesn't exists" << queryStr << query.lastError() << query.lastQuery() << db.databaseName() << query.isSelect();
        ui_Auth->setErrorMessage("Такого пользователя не существует");
        return;
    }

    user_id   = query.value("user_id").toInt();
    uname    = query.value("name").toString();
    upass    = query.value("pass").toString();
    if(uname != username || upass != userpass)
    {
        //qDebug() << "Password missmatch" << uname << " " << username;
        //qDebug() << "Password missmatch" << upass << " " << userpass;
        m_loginSuccesfull = false;
        ui_Auth->setErrorMessage("Введенный пароль не правильный!");
    }
    else
  {
      m_loginSuccesfull = true;
      xPos    = query.value("xpos").toInt();
      yPos    = query.value("ypos").toInt();
      width   = query.value("width").toInt();
      length  = query.value("length").toInt();
      username = uname;
      userpass = upass;
      on_calendarWidget_selectionChanged();
      ui_Auth->close();
      ui_Reg->close();
      this->setGeometry(xPos,yPos,width, length);
      this->show();
      ui->progressBar->setAlignment(Qt::AlignCenter);
      ui->progressBar->setStyleSheet("QProgressBar{"
                                     "border: 1px solid transparent; text-align: center;"
                                     "color: rgba(120, 65, 81);"
                                     "border-radius: 20px;"
                                     "border-width: 2px;"

                                     "background-color: white;"
                                      "}"
                                     "QProgressBar::chunk {background-color: rgba(224, 185, 203, 255); border-radius: 20px;}");
      qDebug() << "User authorization success";

  }
}

void MainWindow::registerWindowShow()
{


    ui_Auth->hide();
    ui_Reg->show();
}

void MainWindow::registerUser()
{
    QSqlQuery query(db);
    QString name = ui_Reg->getName();
    QString pass = ui_Reg->getPass();

    queryStr = QString(
                "SELECT * "
                "FROM users2 "
                "WHERE name = '%1';")
            .arg(name);
    if(!query.exec(queryStr))
    {
        qDebug() << "Unable to get number " << query.lastError() << " : " << query.lastQuery();
        ui_Reg->setErrorText(tr("Произошла ошибка при регистрации пользователя %1. Попробуйте позже.").arg(name));
        return;
    }

    // Если пользователь уже существует
    if(query.size() > 0)
    {
        qDebug() << "User already exists";
        ui_Reg->setErrorText(tr("Пользователь %1 уже существует. Выберите другое имя пользователя или войти в свою учетную запись.").arg(name));
        return;
    }

    // Добавляем нового пользователя в БД
    username          = ui_Reg->getName();
    userpass          = ui_Reg->getPass();

    queryStr =  QString(
                "INSERT INTO users2(name, pass, xpos, ypos, width, length) "
                "VALUES('%1', '%2', %3, %4, %5, %6);")
                .arg(username)
                .arg(userpass)
                .arg(0)
                .arg(0)
                .arg(800)
                .arg(400);

    if(!query.exec(queryStr))
    {
        qDebug() << "Unable to insert user on reg "  << query.lastError() << " : " << query.lastQuery();
        ui_Reg->setErrorText(tr("Произошла ошибка при регистрации пользователя %1. Попробуйте позже.").arg(name));
        return;
    }
    else
    {
        ui_Reg->hide();
        ui_Auth->show();
        QMessageBox::information(this, "Регистрация завершена успешно!", "Регистрация завершена успешно. Теперь Вы можете войти в свою учетную запись!");
    }
}


void MainWindow::display()                                                              //реализация пользотвальского метода отображения главного окна
{
    this->hide();
    ui_Auth->show();                                                                     //отобразить окно авторизации(НЕ главное окно)
}


MainWindow::~MainWindow()                                                               //реализация деструктора
{
   if(m_loginSuccesfull)
    {
        QString str_t       =   "UPDATE users2 "
                                "SET xpos = %2, ypos = %3, width = %4, length = %5 "
                                "WHERE name = '%1';";
        queryStr            = str_t .arg(username)
                                    .arg(this->x())
                                    .arg(this->y())
                                    .arg(this->width())
                                    .arg(this->height());
        QSqlQuery query;
        if(!query.exec(queryStr))
        {
            qDebug() << "Unable to update data on exit "  << query.lastError() << " : " << query.lastQuery() ;
        }
    }
    //db.removeDatabase(dbName);
    qDebug() << "MainWindow Destroyed";
    delete ui;                                                                     //удалить указатель на экземпляр главного окна
    //exit(0);                                                                       //завершить приложение
}

bool MainWindow::connectDB()
{
    dbName = "db";
    db = QSqlDatabase::addDatabase("QSQLITE", dbName);
    qDebug() << "db is valid " << db.isValid();
    db.setDatabaseName(qApp->applicationDirPath() + "/" + dbName + ".db");
    bool opened =  db.open();

    if(!opened)
    {
        qDebug() << "Failed to connect DB";
        QMessageBox::critical(this, tr("Error"),tr("Error to open database %1!")
                              .arg(dbName));
        ::exit(EXIT_FAILURE);
    }

    // Создание таблиц базы данных
    // Сначала проверям, существует ли таблица.
    // Если не существует, то создаем заново
    //QSqlDatabase db = QSqlDatabase::database(dbName);

    QStringList tables = db.tables();
    QSqlQuery query(db);

    if(!tables.contains("users2"))
    {
        queryStr = "CREATE TABLE users2 ( "
                   "user_id INTEGER,"
                   "name TEXT NOT NULL, "
                   "pass TEXT NOT NULL, "
                   "xpos INTEGER, "
                   "ypos INTEGER, "
                   "width INTEGER, "
                   "length INTEGER, "
                   "PRIMARY KEY (user_id));";
         if(!query.exec(queryStr))
        {
            qDebug() << "Unable to create a table" << query.lastError() << queryStr;
            QMessageBox::critical(this, tr("Error"),tr("Error to create table users2 in database %1!")
                                  .arg(dbName));
            ::exit(EXIT_FAILURE);
        }


    }

    if(!tables.contains("notes"))
    {
        queryStr = "CREATE TABLE notes ( "
                   "note_id INTEGER,"
                   "note_num INTEGER NOT NULL,"
                   "user_id INTEGER NOT NULL,"
                   "notedate DATETIME NOT NULL, "
                   "note TEXT NOT NULL, "
                   "done INTEGER NOT NULL,"
                   "PRIMARY KEY (note_id),"
                   "UNIQUE(user_id, notedate, note_num));";
         if(!query.exec(queryStr))
        {
            qDebug() << "Unable to create a table" << query.lastError() << queryStr;
            QMessageBox::critical(this, tr("Error"),tr("Error to create table notes in database %1!")
                                  .arg(dbName));
            ::exit(EXIT_FAILURE);
        }


    }

    if (ui->checkBox->isChecked())
        ui->lineEdit->setStyleSheet("QLineEdit {color: red; background-color: rgba(255, 255, 255, 0); font: italic 12pt \"Montserrat\"; border-top-width: 0px;  border-style: solid; border-color: white; border-bottom-width: 1px;}");
    else
        ui->lineEdit->setStyleSheet("QLineEdit {color: white; background-color: rgba(255, 255, 255, 0); font: italic 12pt \"Montserrat\"; border-top-width: 0px;  border-style: solid; border-color: white; border-bottom-width: 1px;}");



}

void MainWindow::clearLine(int line)
{
    if(line < 1 || line > 10)   return;
    lineEdits.at(line-1)->setStyleSheet("");
    lineEdits.at(line-1)->setText("");
    labels.at(line-1)->setVisible(true);
    checkBoxes.at(line-1)->setChecked(false);
    progres -= 1;
    return;
}
void MainWindow::on_checkBoxes_clicked()
{
    QCheckBox *checkBox = qobject_cast<QCheckBox*>(sender());
    if(!checkBox) return;

    int index = checkBoxes.indexOf(checkBox);
    if(index == -1) return;

    if (checkBox->isChecked())
    {
        lineEdits.at(index)->setStyleSheet("QLineEdit {color: gray; background-color: rgba(255, 255, 255, 0); text-decoration: line-through; font: italic 12pt \"Montserrat\"; border-top-width: 0px;  border-style: solid; border-color: gray; border-bottom-width: 1px;}");
        real_progres += 100;
        double i = real_progres/progres;
        ui->progressBar->setValue((int)i);
        ui->progressBar->repaint();
    }
    else
    {
        lineEdits.at(index)->setStyleSheet("QLineEdit {color: white; background-color: rgba(255, 255, 255, 0); font: italic 12pt \"Montserrat\"; border-top-width: 0px;  border-style: solid; border-color: white; border-bottom-width: 1px;}");
        real_progres -= 100;
        double i = real_progres/progres;
        ui->progressBar->setValue((int)i);
        ui->progressBar->repaint();
    }
    changed = true;
}



void MainWindow::on_btnAddNote_clicked()
{
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(100);



    if(number_click > 10)   return;

    lineEdits.at(number_click-1)->setStyleSheet("QLineEdit {color: white; background-color: rgba(255, 255, 255, 0); font: italic 12pt \"Montserrat\"; border-top-width: 0px;  border-style: solid; border-color: white; border-bottom-width: 1px;}");
    labels.at(number_click-1)->setVisible(false);
    number_click++;
    progres += 1;
    changed = true;
    return;
}

void MainWindow::on_btnSaveInDB_clicked()
{

    QDate date = ui->calendarWidget->selectedDate();
    QString dateStr = date.toString("yyyy-MM-dd");
    qDebug() << "date " << dateStr;
    bool success = true;

    qDebug() << "sender " << sender();
    if(sender() == ui->calendarWidget)
        dateStr = lastDateStr;

    // Добавляем или обновляем заметки
    for(int i = 1; i < number_click; i++)
    {
        queryStr = QString(
                    "INSERT OR REPLACE INTO notes (user_id, note_num, notedate, note, done) "
                    "VALUES (%1, %2, '%3', '%4', %5)"
                    )
                .arg(user_id)
                .arg(i)
                .arg(dateStr)
                .arg(lineEdits.at(i-1)->text())
                .arg(checkBoxes.at(i-1)->isChecked() ? 1 : 0)
                ;

        QSqlQuery query(queryStr, db);
        if(!query.exec())
        {
            qDebug() << "Error to save notes " << query.lastError() << " query " << queryStr;
            success = false;
        }
    }

    // Удаляем лишние заметки
    for(int i = number_click; i <= 10; i++)
    {
        queryStr = QString(
                    "DELETE FROM notes "
                    "WHERE notedate = '%1' "
                    "  AND user_id = %2 "
                    "  AND note_num = %3;")
                .arg(dateStr)
                .arg(user_id)
                .arg(i)
                ;

        QSqlQuery query(queryStr, db);
        if(!query.exec())
        {
            qDebug() << "Error to delete notes " << query.lastError() << " query " << queryStr;
            success = false;
        }
    }

    if(success)
    {
        QMessageBox::information(this, "Сохранено", "Записи успешно сохранены в базе данных.");
        changed = false;
    }
    else
        QMessageBox::critical(this, "Ошибка", "Произошла ошибка при сохранении записей в базе данных.");

}



void MainWindow::on_btnDeleteFromDB_clicked()
{
    QDate date = ui->calendarWidget->selectedDate();
    QString dateStr = date.toString("yyyy-MM-dd");
    qDebug() << "date " << dateStr;

    QMessageBox::StandardButton ans = QMessageBox::question(this, "Удаление", QString("Вы уверены, что хотите удалить все записи за %1?\nЗаписи невозможно будет восстановить.").arg(dateStr));
    if(ans != QMessageBox::Yes)
        return;

    // Удаляем все записи за текущую дату
    queryStr = QString(
                "DELETE FROM notes "
                "WHERE notedate = '%1' "
                "  AND user_id = %2;")
            .arg(dateStr)
            .arg(user_id)
            ;

    QSqlQuery query(queryStr, db);
    if(!query.exec())
    {
        qDebug() << "Error to delete notes " << query.lastError() << " query " << queryStr;
        QMessageBox::critical(this, "Ошибка", "Произошла ошибка при удалении записей из базы данных.");
        return;
    }

    for(int i = 1; i < 10; i++)
    {
        clearLine(i);
    }
    number_click = 1;

    QMessageBox::information(this, "Успешно", "Записи успешно удалены.");
    changed = false;
}

void MainWindow::on_calendarWidget_selectionChanged()
{
    QDate date = ui->calendarWidget->selectedDate();
    QString dateStr = date.toString("yyyy-MM-dd");
    qDebug() << "date " << dateStr;

    if(changed)
    {
        QMessageBox::StandardButton ans = QMessageBox::question(this, "Переключение", QString("У Вас есть несохраненные данные. Все несохраненные данные будут потеряны. Сохранить данные перед переключением?"));
        if(ans == QMessageBox::Yes)
            on_btnSaveInDB_clicked();
    }

    for(int i = 1; i < 10; i++)
    {
        clearLine(i);
    }
    number_click = 1;

    queryStr = QString(
                "SELECT * FROM notes "
                "WHERE notedate = '%1' "
                "  AND user_id = %2 "
                "  ORDER BY note_num ASC;")
            .arg(dateStr)
            .arg(user_id)
            ;

    QSqlQuery query(queryStr, db);
    if(!query.exec())
    {
        qDebug() << "Error to select notes " << query.lastError() << " query " << queryStr;
        return;
    }

    qDebug() << "query " << queryStr;

    int line = 0;
    while(query.next())
    {
        qDebug() << "note " << line+1;
        on_btnAddNote_clicked();
        QString text = query.value("note").toString();
        lineEdits.at(line)->setText(text);
        bool done = query.value("done").toBool();
        if(done)
            checkBoxes.at(line)->click();

        line++;
    }
    changed = false;
    lastDateStr = dateStr;
    ui->tabWidget->setCurrentIndex(0);
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    if(index != 1) return;
    QDate date = ui->calendarWidget->selectedDate();
    QString dateStr = date.toString("MM");
    qDebug() << "date " << dateStr;

    queryStr = QString(
                "SELECT strftime('%d', notedate) AS day, "
                "COUNT(*) AS cnt, notedate "
                "FROM notes "
                "WHERE strftime('%m', notedate) = '%1' "
                " AND  user_id = %2 "
                "GROUP BY strftime('%d', notedate)")
            .arg(dateStr)
            .arg(user_id)
            ;

    QSqlQuery query(queryStr, db);
    if(!query.exec())
    {
        qDebug() << "Error to select notes " << query.lastError() <<
                    " query " << queryStr;
        return;
    }

    QDate cur(date.year(), date.month(), 1);
    int curMonth = date.month();

    // Обнуляем формат
    while(cur.month() == curMonth)
    {
        QTextCharFormat format = ui->calendarWidget->dateTextFormat(cur);
        format.setForeground(QBrush(Qt::black, Qt::SolidPattern));
        ui->calendarWidget->setDateTextFormat(cur, format);
        cur = cur.addDays(1);
    }

    // Устнаваливаем формат для дат, где есть записи
    while(query.next())
    {
        int day = query.value("day").toInt();
        int cnt = query.value("cnt").toInt();
        QString notedate = query.value("notedate").toString();
        qDebug() << "day " << day << " cnt " << cnt;
        QDate date = QDate::fromString(notedate, "yyyy-MM-dd");
        QTextCharFormat format = ui->calendarWidget->dateTextFormat(date);
        format.setForeground(QBrush(Qt::green, Qt::SolidPattern));
        ui->calendarWidget->setDateTextFormat(date, format);
    }

}


