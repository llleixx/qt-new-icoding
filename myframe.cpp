#include "myframe.h"
#include "ui_myframe.h"
#include "mainwindow.h"
#include <QDebug>

extern MainWindow *mainwindow;


MyFrame::MyFrame(QWidget *parent, int quesnum)
    : QFrame{parent},
    id(quesnum),
    ui(new Ui::MyFrame)
{
    ui->setupUi(this);
    // 防止变样
    this->setMaximumSize(350, 300);
    this->setMinimumSize(250, 200);
    setQuestion(id);
}

MyFrame::~MyFrame()
{
    delete ui;
}

void MyFrame::setQuestion(const int &quesnum)
{
    QFile file("./questions/" + QString::number(quesnum) + "/info.txt");
    if(!file.open(QIODevice::ReadOnly)) return perror("openfile wrong2");
    QString quesname = QString(file.readLine());
    this->ui->name->setText(quesname);
    this->ui->id->setText(QString::number(quesnum));
    this->id = quesnum;
    file.close();
}

