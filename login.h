#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QMouseEvent>
#include "mainwindow.h"
#include "users.h"

extern QString username;
extern MainWindow *mainwindow;

QT_BEGIN_NAMESPACE
namespace Ui { class Login; }
QT_END_NAMESPACE

class Login : public QWidget
{
    Q_OBJECT

public:
    Login(QWidget *parent = nullptr);
    ~Login();
    QPoint winP;
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
private slots:
    void on_closeBtn_clicked();
    int on_loginBtn_clicked();
private:
    Ui::Login *ui;
};


void add(int a,int b);
#endif // LOGIN_H

