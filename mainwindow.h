#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QMouseEvent>
#include <QMediaPlayer>
#include <QTimer>
#include <QMediaPlaylist>
#include <QCloseEvent>


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
    void loadSetting();
    void setList(int num);
    void setMusic();
    void setTime();
    int getNumber(int num);
    void saveSetting();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void closeEvent(QCloseEvent *event);

    QPoint point;
    Ui::MainWindow *ui;
    QMediaPlayer player;
    QMediaPlaylist playList;
    QStringList list;
    QString musicPath;
    QTimer timer;

private slots:
    void on_minBtn_clicked();
    void on_closeBtn_clicked();
    void on_pathModify_clicked();



};

#endif // MAINWINDOW_H
