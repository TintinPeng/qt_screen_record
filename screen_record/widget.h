#ifndef WIDGET_H
#define WIDGET_H

#pragma once
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QProcess>
#include <QComboBox>
#include <QUrl>
#include <QDesktopServices>
#include <QDate>
#include <QTime>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void screenShot();
    void openDir();
    void screenStart();
    void screenStop();
    void timeInit();
    void comboBox_1_event(int index);

private:
    QLabel *label_1;
    QPushButton button_1;
    QPushButton button_2;
    QPushButton button_3;
    QPushButton button_4;
    QProcess *process_1;
    QProcess *process_2;
    QProcess *process_3;
    QProcess *process_4;

    QString tempName;
    QString saveName;
    QComboBox *comboBox_1;
    int cmd_1 = 0;

    QString FFmpeg = "../bin/ffmpeg";
    QString rm = "../bin/rm";
    QString savedDir = "../captures/";

    QString currentDate;
    QString screenResolution = "1920x1080";
};
#endif
