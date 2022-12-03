#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QMouseEvent>


#include "myframe.h"
#include "mywidget.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void init();
    void initRandom();
    void initList();
    void initTaskBox();
    void initTask(QString str);
    void setList(int num);
    int getNumber(int num);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    QPoint point;
    Ui::MainWindow *ui;

private slots:
    void on_minBtn_clicked();
    void on_closeBtn_clicked();



};

#endif // MAINWINDOW_H
