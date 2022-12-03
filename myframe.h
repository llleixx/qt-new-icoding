#ifndef MYFRAME_H
#define MYFRAME_H

#include <QFrame>
#include "mywidget.h"
#include "mainwindow.h"
#include "ui_myframe.h"

namespace Ui {
class MyFrame;
}

class MyFrame : public QFrame
{
    Q_OBJECT
public:
    explicit MyFrame(QWidget *parent = nullptr, int quesnum = 0);
    ~MyFrame();
    int id;
    void mousePressEvent(QMouseEvent *event);
    void setQuestion(const int &quesnum);
    Ui::MyFrame *ui;
};

#endif // MYFRAME_H
