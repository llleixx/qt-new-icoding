#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QTime>
#include <QFileDialog>
#include <QDir>

extern QString username;
extern MainWindow *mainwindow;


MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_TranslucentBackground);//设置背景透明
    setWindowFlags(Qt::FramelessWindowHint);//无边框
    //初始化
    this->init();
    // 连接 menu 跳转 与 List 更新
    connect(ui->menu, &QListWidget::currentRowChanged, [&](int index){
        if(index == 0) this->initList();
        else if(index == 2) this->initTaskBox();
        ui->stackedWidget->setCurrentIndex(index);
    });
    ui->menu->setCurrentRow(0);
    // 连接随机
    connect(ui->randomBtn, &QPushButton::clicked, this, &MainWindow::initRandom);

    connect(ui->listWidget, &QListWidget::itemClicked, [&](QListWidgetItem *item){
        int id = item->data(Qt::UserRole).toInt();
        MyWidget *widget = new MyWidget(id, mainwindow->ui->tabWidget);
        mainwindow->ui->tabWidget->addTab(widget, QString::number(id));
        mainwindow->ui->tabWidget->setCurrentWidget(widget);
        mainwindow->ui->menu->setCurrentRow(1);
    });
    // 页面变化，刷新 题单按钮和分数
    connect(ui->tabWidget, &QTabWidget::currentChanged, [&](){
        (static_cast<MyWidget *>(ui->tabWidget->currentWidget()))->myUpdate();
    });
    // 任务显示
    connect(ui->taskBox, &QComboBox::currentTextChanged, this, &MainWindow::initTask);
    // connect box 和 slider
    connect(ui->volBox, QOverload<int>::of(&QSpinBox::valueChanged), ui->volSlider, &QSlider::setValue);
    connect(ui->volSlider, &QSlider::valueChanged, ui->volBox, &QSpinBox::setValue);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    this->loadSetting();
    srand(time(0));
    this->initRandom();
    // 清空默认页面
    ui->tabWidget->clear();
}

void MainWindow::initRandom()
{
    QFile file("./questions/data.txt");
    if(!file.open(QIODevice::ReadOnly)) perror("randomInit wrong");
    int tot = file.readLine().toInt(), num;
    MyFrame* frame[9] = {ui->question1, ui->question2, ui->question3, ui->question4, ui->question5, \
                       ui->question6, ui->question7, ui->question8, ui->question9};
    for(int i = 0; i < 9; ++i)
    {
        num = rand() % tot;
        int id = this->getNumber(num);
        frame[i]->setQuestion(id);
    }

    file.close();
}

void MainWindow::initList()
{
    ui->listWidget->clear();
    QFile file("./users/" + username + ".txt");
    if(!file.open(QIODevice::ReadOnly)) perror("initList wrong");
    file.readLine();
    int num, tot;
    QTextStream stream(&file);
    stream >> tot;
    for(int i = 0; i < tot; ++i)
    {
        stream >> num;
        this->setList(num);
    }
    file.close();
}

void MainWindow::initTaskBox()
{
    QFile file("./questions/data.txt");
    if(!file.open(QIODevice::ReadOnly)) perror("initTask wrong");
    file.readLine(); file.readLine();
    while(!file.atEnd())
    {
        ui->taskBox->addItem(QString(file.readLine()).trimmed());
        while(QString(file.readLine()).trimmed() != "$");
        file.readLine();
    }
    file.close();
}

void MainWindow::initTask(QString str)
{
    QLayoutItem *child;
    while ((child = ui->quesLayout->takeAt(0)) != 0)
    {
        if(child->widget())
        {
             child->widget()->setParent(NULL);
             delete child->widget();//释放
         }
        delete child;
    }
    QFile file("./questions/data.txt");
    if(!file.open(QIODevice::ReadOnly)) perror("initTask wrong");
    file.readLine(); file.readLine();
    while(!file.atEnd())
    {
        if(QString(file.readLine()).trimmed() == str) break;
        while(QString(file.readLine()).trimmed() != "$");
        file.readLine();
    }
    QString des = ""; str = QString(file.readLine());
    while(str.trimmed() != "$") des.append(str), str = QString(file.readLine());
    ui->taskDescrip->setText(des);

    // 自适应
    ui->taskDescrip->document()->adjustSize();
    ui->taskDescrip->setFixedHeight(ui->taskDescrip->document()->size().rheight());

    str = QString(file.readLine());
    QStringList list = str.split(" ");
    int tot = list.count();
    for(int i = 0; i < tot; ++i)
    {
        int row = i / 3, col = i % 3;
        ui->quesLayout->addWidget(new MyFrame(ui->taskArea, list.at(i).toInt()), row, col, 1, 1, Qt::AlignCenter);
    }
    file.close();
}

void MainWindow::loadSetting()
{
    QFile file("./users/setting.txt");
    if(!file.open(QIODevice::ReadOnly)) perror("loadSetting wrong");
    if(QString(file.readLine()).trimmed() == "true") ui->musicSwitch->setChecked(true);
    else ui->musicSwitch->setChecked(false);
    int vol = file.readLine().toInt();
    ui->volBox->setValue(vol);
    ui->volSlider->setValue(vol);
    ui->musicPath->setText(QString(file.readLine()).trimmed());
    if(QString(file.readLine()).trimmed() == "true") ui->musicSwitch->setChecked(true);
    else ui->musicSwitch->setChecked(false);
    ui->timeBox->setCurrentText(QString(file.readLine()).trimmed());

    // 设置音乐 时间
    timer.setInterval(1000);
    // connect
    connect(&timer, &QTimer::timeout, [&](){
        ui->timeLabel->setText(QDateTime::currentDateTime().toString(ui->timeBox->currentText()));
    });
    connect(ui->timeSwitch, &QCheckBox::stateChanged, this, &MainWindow::setTime);
    connect(ui->musicSwitch, &QCheckBox::stateChanged, this, &MainWindow::setMusic);
    this->setTime();
    this->setMusic();
}

void MainWindow::setList(int num)
{
    QFile file("./questions/" + QString::number(num) + "/info.txt");
    if(!file.open(QIODevice::ReadOnly)) perror("setList wrong");
    QListWidgetItem *item = new QListWidgetItem(QString::number(num) + ". " + QString(file.readLine()).trimmed(), ui->listWidget);
    item->setData(Qt::UserRole, num);
    ui->listWidget->addItem(item);
    file.close();
}

void MainWindow::setMusic()
{
    if(ui->musicSwitch->checkState() == Qt::Checked)
    {
        musicPath = ui->musicPath->text();
        QDir dir(musicPath);
        QStringList nameFilters;
        nameFilters << "*.mp3" << "*.m4a";
        list = dir.entryList(nameFilters, QDir::Files | QDir::Readable, QDir::Name);
        qDebug() << list.count() << musicPath << list.at(0);
        qDebug() << player.state();
        this->currentIndex = 0;
        player.setMedia(QUrl::fromLocalFile(musicPath + "/" + list.at(0)));
        player.play();
    }
    else player.stop();

}

void MainWindow::setTime()
{
    if(ui->timeSwitch->checkState() == Qt::Checked)
    {
        timer.start();
        ui->timeLabel->show();
    }
    else timer.stop(), ui->timeLabel->hide();
}

int MainWindow::getNumber(int num)
{
    QFile file("./questions/data.txt");
    if(!file.open(QIODevice::ReadOnly)) perror("getNumber Wrong");
    int id;
    QTextStream stream(&file);
    stream.readLine();
    for(int i = 0; i< num; ++i) stream >> id;
    file.close();
    return id;
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->y() < ui->topBar->height())
    {
        this->point = event->pos();
    }
    return QWidget::mousePressEvent(event);
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(event->y() < ui->topBar->height())
    {
        this->move(event->globalPos() - this->point);
    }
    return QWidget::mouseMoveEvent(event);
}

void MainWindow::on_minBtn_clicked()
{
    this->showMinimized();
}

void MainWindow::on_closeBtn_clicked()
{
    this->close();
}

void MainWindow::on_pathModify_clicked()
{
    musicPath = QFileDialog::getExistingDirectory(this, "选择音乐目录", ui->musicPath->text());
    ui->musicPath->setText(musicPath);
}

void MyFrame::mousePressEvent(QMouseEvent *event)
{
    MyWidget *widget = new MyWidget(this->id, mainwindow->ui->tabWidget);
    mainwindow->ui->tabWidget->addTab(widget, QString::number(this->id));
    mainwindow->ui->tabWidget->setCurrentWidget(widget);
    mainwindow->ui->menu->setCurrentRow(1);
    return QFrame::mousePressEvent(event);
}
