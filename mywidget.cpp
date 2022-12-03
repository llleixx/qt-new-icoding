#include "mywidget.h"
#include "ui_mywidget.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QFileDialog>

MyWidget::MyWidget(int id, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyWidget)
{
    ui->setupUi(this);
    this->id = id;
    //问题名称，输入格式，输出格式，时间限制，空间限制
    QString path = "./questions/" + QString::number(id) + "/info.txt";
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly)) perror("openfile wrong mywidget");
    ui->quesLabel->setText(QString(file.readLine()));
    myWrite(file, ui->descriptionText);
    myWrite(file, ui->inForm);
    myWrite(file, ui->outForm);
    myWrite(file, ui->range);
    ui->timeLimit->setText("时间限制: " + QString(file.readLine()));
    ui->memoryLimit->setText("空间限制: " + QString(file.readLine()));
    while(!file.atEnd())
    {
        QListWidgetItem *item = new QListWidgetItem(QString(file.readLine()).trimmed(),ui->tagList);
        item->setTextAlignment(Qt::AlignCenter);
        ui->tagList->addItem(item);
    }
    file.close();
    //输入样例
    path = "./questions/" + QString::number(id) + "/0.in";
    file.setFileName(path);
    if(!file.open(QIODevice::ReadOnly)) perror("openfile wrong mywidget");
    ui->inText->setText(QString(file.readAll()));
    file.close();
    //输出样例
    path = "./questions/" + QString::number(id) + "/0.out";
    file.setFileName(path);
    if(!file.open(QIODevice::ReadOnly)) perror("openfile wrong mywidget");
    ui->outText->setText(QString(file.readAll()));
    file.close();

    //先隐藏 tag
    ui->tagList->hide();


    //改变框大小
    ui->descriptionText->document()->adjustSize();
    ui->descriptionText->setFixedHeight(ui->descriptionText->document()->size().rheight());
    ui->inText->document()->adjustSize();
    ui->inText->setFixedHeight(ui->inText->document()->size().rheight());
    ui->outText->document()->adjustSize();
    ui->outText->setFixedHeight(ui->outText->document()->size().rheight());
    ui->inForm->document()->adjustSize();
    ui->inForm->setFixedHeight(ui->inForm->document()->size().rheight());
    ui->outForm->document()->adjustSize();
    ui->outForm->setFixedHeight(ui->outForm->document()->size().rheight());
    ui->range->document()->adjustSize();
    ui->range->setFixedHeight(ui->range->document()->size().rheight());
    ui->inText->document()->setTextWidth(ui->inText->width());
    ui->inText->setFixedHeight(ui->inText->document()->size().height() + 10);
    ui->outText->document()->setTextWidth(ui->outText->width());
    ui->outText->setFixedHeight(ui->outText->document()->size().height() + 10);
}

MyWidget::~MyWidget()
{
    delete ui;
}

void MyWidget::myWrite(QFile &file, QTextBrowser *text)
{
    text->clear();
    QString str(file.readLine());
    while(str.trimmed()!="$")
    {
        text->append(str);
        str = QString(file.readLine());
    }
}

void MyWidget::myUpdate()
{
    QFile file("./users/" + username + ".txt");
    if(!file.open(QIODevice::ReadOnly)) return perror("openfile wrong myUpdate");
    QTextStream text(&file);
    text.readLine();
    int tot, num;
    bool f1 = 0, f2 = 0;
    //题单
    text >> tot;
    for(int i = 0; i < tot; ++i)
    {
        text >> num;
        if(num == this->id)
        {
            f1 = 1;
            ui->addBtn->setText("移除题单");
            break;
        }
    }
    if(!f1) ui->addBtn->setText("加入题单");
    text.readLine();
    //分数
    while(!text.atEnd())
    {
        text >> num;
        if(num == this->id)
        {
            text >> num;
            f2 = 1;
            ui->scoreLabel->setText(QString::number(num));
            if(num == 100) ui->scoreLabel->setStyleSheet("QLabel{color: green;}");
            else ui->scoreLabel->setStyleSheet("QLabel{color: red;}");
        }
    }
    if(!f2) ui->scoreLabel->setText("0");
    file.close();
}

void MyWidget::on_tagBtn_clicked()
{
    if(ui->tagList->isHidden()) ui->tagList->show();
    else ui->tagList->hide();
}

void MyWidget::on_submitBtn_clicked()
{
    QString path = QFileDialog::getExistingDirectory();
}

void MyWidget::on_answerBtn_clicked()
{
    QFile file("./questions/" + QString::number(this->id) + "/answer.txt");
    if(!file.open(QIODevice::ReadOnly)) perror("on_answerBtn_clicked error");
    QMessageBox dialog(QMessageBox::NoIcon, "题解", QString(file.readAll()), QMessageBox::Ok, this);
    dialog.exec();
}

void MyWidget::on_addBtn_clicked()
{
    if(ui->addBtn->text() == "加入题单")
    {
        ui->addBtn->setText("移除题单");
        int tot, num;
        QFile file("./users/" + username + ".txt");
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) perror("on_addBtn_clicked wrong");
        QTextStream text(&file);
        QString content(text.readLine());
        text >> tot;
        content.append("\n" + QString::number(tot + 1));
        for (int i = 0; i < tot; ++i)
        {
            text >> num;
            content += " " + QString::number(num);
        }
        content += " " + QString::number(this->id);
        content.append(text.readAll());
        file.close();
        file.open(QIODevice::WriteOnly |QIODevice::Truncate| QIODevice::Text);
        file.write(content.toLatin1());
    }
    else
    {
        ui->addBtn->setText("加入题单");
        int tot, num;
        QFile file("./users/" + username + ".txt");
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) perror("on_addBtn_clicked wrong");
        QTextStream text(&file);
        QString content(text.readLine());
        text >> tot;
        content.append("\n" + QString::number(tot - 1));
        for (int i = 0; i < tot; ++i)
        {
            text >> num;
            if (num != this->id) content += " " + QString::number(num);
        }
        content.append(text.readAll());
        file.close();
        file.open(QIODevice::WriteOnly |QIODevice::Truncate| QIODevice::Text);
        file.write(content.toLatin1());
    }
}


