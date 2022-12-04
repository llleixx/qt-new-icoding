#include "login.h"
#include "ui_login.h"
#include <QDebug>
#include <unistd.h>
#include <QDir>
#include <QRegExpValidator>
#include <QMessageBox>
#include <QInputDialog>

Login::Login(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Login)
{
    //设置相关属性
    ui->setupUi(this);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setWindowFlags(Qt::FramelessWindowHint);
    //关闭

    QRegExp regExp("[A-Za-z0-9]{1,16}");
    ui->usernameLine->setValidator(new QRegExpValidator(regExp, this));
    ui->passwdLine->setValidator(new QRegExpValidator(regExp, this));
}

Login::~Login()
{
    delete ui;
}

void Login::mousePressEvent(QMouseEvent *event)//鼠标点击事件
{
    winP = event->pos();
    return QWidget::mousePressEvent(event);//由基类继续传递
}

void Login::mouseMoveEvent(QMouseEvent *event)
{
    this->move(event->globalPos() - winP);//设置窗口相对于桌面的坐标（x），实现窗口移动
    return QWidget::mouseMoveEvent(event);//由基类继续传递;
}

void Login::myRegister(QString username, QString passwd)
{
    QFile file("./users/" + username + ".txt");
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) perror("myRegister wrong");
    QString str;
    str.append(passwd + "\n");
    str.append("0\n");
    file.write(str.toUtf8().data());
    file.close();
}

void Login::on_closeBtn_clicked()
{
    this->close();
}

void Login::on_loginBtn_clicked()
{
    username = ui->usernameLine->text();
    QString passwd = ui->passwdLine->text();
    QString path = "users/" + username + ".txt";
    QFile user(path);
    if(!user.open(QIODevice::ReadOnly))
    {
        QMessageBox::information(this, "登录错误", "该用户名不存在");
        return;
    }
    QString passwd1 = QString(user.readLine().trimmed());
    if(passwd1 == passwd)
    {
        this->close();
        mainwindow = new MainWindow();
        mainwindow->show();
        return user.close();
    }
    else
    {
        QMessageBox::information(this, "登录错误", "密码错误");
        return user.close();
    }
}

void Login::on_minBtn_clicked()
{
    this->showMinimized();
}

void Login::on_aboutBtn_clicked()
{
    QMessageBox::information(this, "提示", "用户名和密码只能由 1 到 16 位的大小写字母和数字组成");
}

void Login::on_registerBtn_clicked()
{
    bool ok;
    QRegExp rC("[0-9a-zA-Z]{1,16}");
    QRegExpValidator regs(rC, this);
    QString registerName = QInputDialog::getText(this, "注册", "请输入您的用户名", QLineEdit::Normal, "", &ok);
    if(!ok) return;
    int pos = 0;
    if(regs.validate(registerName, pos) != QValidator::Acceptable)
    {
        QMessageBox::information(this, "用户名格式错误", "用户名只能由 1 到 16 位的大小写字母和数字组成");
        return;
    }
    QFile file("./users/" + registerName + ".txt");
    if(file.exists())
    {
        QMessageBox::information(this, "用户名错误", "该用户名已经存在");
        return;
    }
    QString registerPasswd = QInputDialog::getText(this, "注册", "请输入您的密码", QLineEdit::Normal, "", &ok);
    if(regs.validate(registerPasswd, pos) != QValidator::Acceptable)
    {
        QMessageBox::information(this, "密码格式错误", "密码只能由 1 到 16 位的大小写字母和数字组成");
        return;
    }
    myRegister(registerName, registerPasswd);
}

