#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include <QTextBrowser>
#include "mywidget.h"
#include <QFile>
#include <QMessageBox>


extern QString username;

namespace Ui {
class MyWidget;
}

class MyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MyWidget(int id = 0, QWidget *parent = nullptr);
    int id;
    ~MyWidget();
    void myWrite(QFile &file, QTextBrowser *text);
    void myUpdate();

private slots:
    void on_tagBtn_clicked();
    void on_submitBtn_clicked();
    void on_answerBtn_clicked();
    void on_addBtn_clicked();

private:
    Ui::MyWidget *ui;
};

#endif // MYWIDGET_H
