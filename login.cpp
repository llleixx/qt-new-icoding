#include "login.h"
#include "ui_login.h"
#include <QDebug>
#include <unistd.h>
#include <QDir>

Login::Login(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Login)
{
    // 测试
    mainwindow = new MainWindow();
    mainwindow->show();
    //设置相关属性
    ui->setupUi(this);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setWindowFlags(Qt::FramelessWindowHint);
    //关闭
//    connect(ui->cancelBtn, &QPushButton::clicked, [=](){
//        this->close();
//    });


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

void Login::on_closeBtn_clicked()
{
    this->close();
}

/*
0 成功
1 用户名不存在
2 密码错误
*/

int Login::on_loginBtn_clicked()
{
    username = ui->usernameLine->text();
    QString passwd = ui->passwdLine->text();
    QString path = "users/" + username + ".txt";
    QFile user(path);
    if(!user.open(QIODevice::ReadOnly)) return 1;
    QString passwd1 = QString(user.readLine());
    if(passwd1 == passwd)
    {
        this->close();
        mainwindow = new MainWindow();
        mainwindow->show();
        return user.close(), 0;
    }
    else return user.close(), 2;
}

