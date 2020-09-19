#include "widget.h"

#include <QApplication>
#include <QProcess>
#include <QFile>
#include <QDebug>
#include <QDir>

Widget::Widget(QWidget *parent) : QWidget(parent)
{
    this->resize(350, 250);
    this->setWindowTitle("录屏工具");

    label_1 = new QLabel(this);
    label_1->setText("录制尚未开始");
    label_1->setGeometry(175, 100, 200, 50);
    label_1->setParent(this);
    label_1->setAlignment(Qt::AlignCenter);

    button_1.setText("屏幕录制");
    button_1.setGeometry(0, 0, 175, 50);
    button_1.setParent(this);

    button_2.setText("停止录制");
    button_2.setGeometry(0, 50, 175, 50);
    button_2.setParent(this);

    button_3.setText("截屏");
    button_3.setGeometry(0, 100, 175, 50);
    button_3.setParent(this);

    button_4.setText("打开文件夹");
    button_4.setGeometry(0, 150, 175, 50);
    button_4.setParent(this);


    comboBox_1 = new QComboBox(this);
    comboBox_1->setGeometry(0, 200, 175, 50);
    comboBox_1->setParent(this);
    comboBox_1->addItem("屏幕");
    comboBox_1->addItem("摄像头");

    connect(&button_1, &QPushButton::pressed, this, &Widget::screenStart);
    connect(&button_2, &QPushButton::pressed, this, &Widget::screenStop);
    connect(&button_3, &QPushButton::pressed, this, &Widget::screenShot);
    connect(&button_4, &QPushButton::pressed, this, &Widget::openDir);

    void (QComboBox::*comboBox_1_f)(int) = &QComboBox::currentIndexChanged;
    connect(comboBox_1, comboBox_1_f, this, &Widget::comboBox_1_event);
}

void Widget::comboBox_1_event(int index)
{
    switch (index) {
    case 0: cmd_1 = 0; break;
    case 1: cmd_1 = 1; break;
    default: cmd_1 = 0; break;
    }
}

void Widget::timeInit()
{
    QDateTime dt;
    QTime time;
    QDate date;
    dt.setTime(time.currentTime());
    dt.setDate(date.currentDate());
    currentDate = dt.toString("yyyy-MM-dd-hh-mm-ss");
    tempName = savedDir + currentDate;
}

void Widget::screenStart()
{
    button_1.setEnabled(false);
    timeInit();
    label_1->setText("录制中");
    QStringList cmd_a, cmd_b;
    switch (cmd_1) {
    case 0: cmd_a << "-f" << "x11grab" << "-s" << screenResolution << "-i" << ":0.0" << tempName + ".mp4"; break;
    case 1: cmd_a << "-i" << "/dev/video0" << tempName + ".mp4"; break;
    default: cmd_a << "-f" << "x11grab" << "-s" << screenResolution << "-i" << ":0.0" << tempName + ".mp4"; break;
    }
    cmd_b << "-f" << "alsa" << "-i" << "default" << tempName + ".mp3";
    process_1 = new QProcess();
    process_2 = new QProcess();
    process_1->start(FFmpeg, cmd_a);
    process_2->start(FFmpeg, cmd_b);
    saveName = tempName;
}

void Widget::screenStop()
{
    QFile currentVideo(saveName + "屏幕录制.mp4");
    QStringList cmd_1, cmd_2;
    cmd_1 << "-i" << saveName + ".mp4" << "-i" << saveName + ".mp3" << "-t" << "25" << "-y" << saveName + "屏幕录制.mp4";
    cmd_2 << saveName + ".mp4" << saveName + ".mp3";
    //../bin/ffmpeg -i $1.mp4 -i $1.mp3 -t 25 -y $1屏幕录制.mp4
    process_1->write("q");
    process_1->waitForFinished();
    process_2->write("q");
    process_2->waitForFinished();

    process_4 = new QProcess();
    process_4->startDetached(FFmpeg, cmd_1);
    process_4->waitForFinished();

    while (!currentVideo.exists()) { }

    process_4->startDetached(rm, cmd_2);
    label_1->setText("上一个录制已完成\n新录制尚未开始");
    button_1.setEnabled(true);
}

void Widget::screenShot()
{
    QStringList cmd;
    timeInit();
    cmd << "-f" << "x11grab" << "-s" << screenResolution << "-i" << ":0.0" << "-frames:v" << "1" << tempName + "屏幕截图.jpg";
    process_3 = new QProcess();
    process_3->start(FFmpeg, cmd);
}

void Widget::openDir()
{
    QString currentPath = QDir::currentPath();
    QString url;
//    QString url = "/home/tintin/Workspaces/QtCreator/screen_record/captures/";
//    qDebug() << currentPath + "/" + savedDir << Qt::endl;
    url = currentPath + "/" + savedDir;
    QDesktopServices::openUrl(QUrl(url, QUrl::TolerantMode));
}

Widget::~Widget() {}
