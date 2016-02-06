#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QtCore/qmath.h>
#include <QtMultimedia>
#include <QUrl>
#include <QSystemTrayIcon>
#include <QIcon>
#include <QMessageBox>
#include <QFile>

#include <QDebug>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void showTop();

private slots:
    void changeEvent(QEvent *event);
    void updateSeconds();
    void on_resetBtn_clicked();


protected:
    void closeEvent(QCloseEvent *event);

private:
    QMediaPlayer *mp3play = new QMediaPlayer(this);
    int seconds = 0;

    void displayTime(int seconds); // Display time passed
    QString TimeToString(int hours, int mins, int secs);
    void checkTime(int seconds);

    QString soundFile = "resources/music.mp3";

    QTimer *timer;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
