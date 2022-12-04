#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QTime>
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include <QInputDialog>

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
        if(ui->tabWidget->count())
        (static_cast<MyWidget *>(ui->tabWidget->currentWidget()))->myUpdate();
    });
    // 任务显示
    connect(ui->taskBox, &QComboBox::currentTextChanged, this, &MainWindow::initTask);

    connect(ui->volBox, QOverload<int>::of(&QSpinBox::valueChanged), ui->volSlider, &QSlider::setValue);
    connect(ui->volSlider, &QSlider::valueChanged, ui->volBox, &QSpinBox::setValue);
    // connect tabwidget关闭
    connect(ui->tabWidget, &QTabWidget::tabCloseRequested, [&](int index){
        QWidget *widget = ui->tabWidget->widget(index);
        ui->tabWidget->removeTab(index);
        delete(widget);
    });


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
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) perror("randomInit wrong");
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
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) perror("initList wrong");
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
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) perror("initTask wrong");
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
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) perror("initTask wrong");
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
    // connect 音乐模式变化
    ui->musicModel->setCurrentIndex(-1);
    connect(ui->musicModel, QOverload<int>::of(&QComboBox::currentIndexChanged), [&](int index){
        switch(index)
        {
            case 0: playList.setPlaybackMode(QMediaPlaylist::Random); break;
            case 1: playList.setPlaybackMode(QMediaPlaylist::Sequential); break;
            case 2: playList.setPlaybackMode(QMediaPlaylist::CurrentItemInLoop); break;
            case 3: playList.setPlaybackMode(QMediaPlaylist::Loop); break;
            default: perror("musicModel Wrong");
        }
    });

    QFile file("./users/setting.txt");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) perror("loadSetting wrong");
    if(QString(file.readLine()).trimmed() == "true") ui->musicSwitch->setChecked(true);
    else ui->musicSwitch->setChecked(false);
    int vol = file.readLine().toInt();
    ui->volBox->setValue(vol);
    ui->volSlider->setValue(vol);
    ui->musicPath->setText(QString(file.readLine()).trimmed());
    ui->musicModel->setCurrentIndex(QString(file.readLine()).toInt());
    if(QString(file.readLine()).trimmed() == "true") ui->timeSwitch->setChecked(true);
    else ui->timeSwitch->setChecked(false);
    ui->timeBox->setCurrentText(QString(file.readLine()).trimmed());

    // 设置音乐与时间
    timer.setInterval(1000);
    // connect 时间显示
    connect(&timer, &QTimer::timeout, [&](){
        ui->timeLabel->setText(QDateTime::currentDateTime().toString(ui->timeBox->currentText()));
    });
    // connect box 和 slider player
    connect(ui->volSlider, &QSlider::valueChanged, [&](int vol){
        player.setVolume(vol);
    });
    // connect 开关
    connect(ui->timeSwitch, &QCheckBox::stateChanged, this, &MainWindow::setTime);
    connect(ui->musicSwitch, &QCheckBox::stateChanged, this, &MainWindow::setMusic);
    // connect 音乐暂停
    connect(ui->musicBtn, &QPushButton::clicked, [&](){
        if(player.state() == QMediaPlayer::PausedState) player.play();
        else player.pause();
    });
    // connecy 音乐名称显示
    connect(&playList, &QMediaPlaylist::currentIndexChanged, [&](int index){
       if(index != -1) ui->musicBtn->setText(list.at(index));
    });
    // connect 路径变化
    connect(ui->musicPath, &QLineEdit::editingFinished, [&](){
       this->setMusic();
    });

    // 初始化
    this->setTime();
    this->setMusic();
}

void MainWindow::setList(int num)
{
    QFile file("./questions/" + QString::number(num) + "/info.txt");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) perror("setList wrong");
    QListWidgetItem *item = new QListWidgetItem(QString::number(num) + ". " + QString(file.readLine()).trimmed(), ui->listWidget);
    item->setData(Qt::UserRole, num);
    ui->listWidget->addItem(item);
    file.close();
}

void MainWindow::setMusic()
{
    if(ui->musicSwitch->checkState() == Qt::Checked)
    {
        playList.clear();
        musicPath = ui->musicPath->text();
        QDir dir(musicPath);
        QStringList nameFilters;
        nameFilters << "*.mp3";
        list = dir.entryList(nameFilters, QDir::Files | QDir::Readable, QDir::Name);
        for(int i = 0; i < list.count(); ++i)
            playList.addMedia(QUrl::fromLocalFile(musicPath + "/" + list.at(i)));
        player.setPlaylist(&playList);
        player.play();
        if(list.count())
        {
            ui->musicBtn->setText(list.at(0));
        }
        else ui->musicBtn->setText("该目录下未检测到音乐");
        ui->musicBtn->show();
    }
    else player.stop(), ui->musicBtn->hide();

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
    if(!file.open(QIODevice::ReadOnly  | QIODevice::Text)) perror("getNumber Wrong");
    int id;
    QTextStream stream(&file);
    stream.readLine();
    for(int i = 0; i< num; ++i) stream >> id;
    file.close();
    return id;
}

void MainWindow::saveSetting()
{
    QFile file("./users/setting.txt");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) perror("saveSettging wrong");
    QString str;
    if(ui->musicSwitch->checkState() == Qt::Checked) str.append("true\n");
    else str.append("false\n");
    str.append(QString::number(ui->volBox->value()) + "\n");
    str.append(ui->musicPath->text() + "\n");
    str.append(QString::number(ui->musicModel->currentIndex()) + "\n");
    if(ui->timeSwitch->checkState() == Qt::Checked) str.append("true\n");
    else str.append("false\n");
    str.append(ui->timeBox->currentText());
    file.close();
    if(!file.open(QIODevice::WriteOnly |QIODevice::Truncate| QIODevice::Text)) perror("saveSetting wrong");
    file.write(str.toUtf8().data()); // 可以存 utf8 的中文
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

void MainWindow::closeEvent(QCloseEvent *event)
{
    this->saveSetting();
    return QWidget::closeEvent(event);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    qDebug() << event->key();
    switch(event->key())
    {
        case Qt::Key_Alt:
            alt = true;
            break;
        case Qt::Key_Space:
            space = true;
            break;
        case Qt::Key_Left:
            left = true;
            break;
        case Qt::Key_Right:
            right = true;
            break;
        case Qt::Key_F1:
            ui->menu->setCurrentRow(0);
            break;
        case Qt::Key_F2:
            ui->menu->setCurrentRow(1);
            break;
        case Qt::Key_F3:
            ui->menu->setCurrentRow(2);
            break;
        case Qt::Key_F4:
            ui->menu->setCurrentRow(3);
            break;
    }
    if(alt && space)
    {
        if(player.state() == QMediaPlayer::PausedState) player.play();
        else if(player.state() == QMediaPlayer::PlayingState) player.pause();
    }
    if(alt && left) playList.previous();
    if(alt && right) playList.next();
    return QWidget::keyPressEvent(event);
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    switch(event->key())
    {
        case Qt::Key_Alt:
            alt = false;
            break;
        case Qt::Key_Space:
            space = false;
            break;
        case Qt::Key_Left:
            left = false;
            break;
        case Qt::Key_Right:
            right = false;
            break;
    }
    return QWidget::keyReleaseEvent(event);
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
    if(musicPath != "")
    {
        ui->musicPath->setText(musicPath);
        this->setMusic();
    }
}

void MainWindow::on_musicCheck_clicked()
{
    QMessageBox dialog(QMessageBox::NoIcon, "音乐列表", list.join("\n"), QMessageBox::Ok, this);
    dialog.exec();
}


void MainWindow::on_passwdModify_clicked()
{
    QMessageBox::StandardButton btn =  QMessageBox::warning(this, "警告", "您将修改您的密码", QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if(btn == QMessageBox::No) return;

    bool ok;
    QString str;
    str = QInputDialog::getText(this, "您将修改您的密码", "请输入您的旧密码", QLineEdit::Password, "", &ok);
    if(ok)
    {
        QFile file("./users/" + username + ".txt");
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) perror("on_userDelete_clicked wrong");
        QString passwd(file.readLine().trimmed());
        qDebug() << passwd << str;
        if(passwd == str)
        {
            str = QInputDialog::getText(this, "您将修改您的密码", "请输入您的新密码", QLineEdit::Password, "", &ok);
            str = str + "\n";
            str += QString(file.readAll());
            file.close();
            if(!file.open(QIODevice::WriteOnly |QIODevice::Truncate| QIODevice::Text)) perror("on_passwdModify_clicked");
            file.write(str.toLatin1());
        }
        else
        {
            file.close();
            QMessageBox::information(this, "错误", "密码错误", QMessageBox::Ok);
        }
    }
}

void MainWindow::on_userDelete_clicked()
{
    QMessageBox::StandardButton btn =  QMessageBox::warning(this, "警告", "确定删除该用户？", QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if(btn == QMessageBox::No) return;

    bool ok;
    QString str;
    str = QInputDialog::getText(this, "您将删除您的账户", "请输入您的密码", QLineEdit::Password, "", &ok, Qt::WindowFlags(), \
                         Qt::ImhPreferUppercase | Qt::ImhPreferLowercase | Qt::ImhPreferNumbers);
    if(ok)
    {
        QFile file("./users/" + username + ".txt");
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) perror("on_userDelete_clicked wrong");
        QString passwd(file.readLine().trimmed());
        if(passwd == str)
        {
            file.close();
            file.remove();
            this->close();
        }
        else
        {
            file.close();
            QMessageBox::information(this, "删除账户失败", "密码错误", QMessageBox::Ok);
        }
    }
}

void MyFrame::mousePressEvent(QMouseEvent *event)
{
    MyWidget *widget = new MyWidget(this->id, mainwindow->ui->tabWidget);
    mainwindow->ui->tabWidget->addTab(widget, QString::number(this->id));
    mainwindow->ui->tabWidget->setCurrentWidget(widget);
    mainwindow->ui->menu->setCurrentRow(1);
    return QFrame::mousePressEvent(event);
}
