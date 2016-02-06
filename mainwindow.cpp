#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Stand up Alarm");

    timer = new QTimer(this);
    timer->setTimerType(Qt::PreciseTimer);
    timer->setSingleShot(false);
    timer->setInterval(1000);
    timer->start(); // Every sec signal to change seconds

    connect(timer, SIGNAL(timeout()), this, SLOT(updateSeconds()));

    QSystemTrayIcon *icon = new QSystemTrayIcon(QIcon(":/resources/favicon.ico"),this);
    icon->setVisible(true);
    icon->setToolTip("StandUp Alarm");
    icon->show();

    connect(icon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(showTop()) );

    QMenu *menu = new QMenu(this);

    QAction *quitAction = new QAction("Quit",this);
    connect(quitAction,SIGNAL(triggered()),this,SLOT(close()));
    menu->addAction(quitAction);
    icon->setContextMenu(menu);

    QAction *restore = new QAction("Restore",this);
    restore->setEnabled(true);
    connect(restore,SIGNAL(triggered()),this,SLOT(show()));
    menu->addAction(restore);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::updateSeconds()
{
    this->seconds += 1;
    displayTime(seconds);
    checkTime(seconds);
}

void MainWindow::on_resetBtn_clicked()
{
    timer->start();
    this->seconds = 0;
    if(mp3play->state() != QMediaPlayer::StoppedState)
    {
        mp3play->stop();
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, "Closing App",
                                                                tr("Are you sure?\n"),
                                                                QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                                QMessageBox::Yes);
    if (resBtn != QMessageBox::Yes)
    {
        event->ignore();
    }
    else
    {
        event->accept();
    }
}

void MainWindow::displayTime(int seconds)
{
    // Counter values
    int hours = qFloor(seconds/3600);
    int mins = qFloor(seconds - (hours*3600)) / 60;
    int secs = qFloor(seconds % 60);

    // Add leading zeros if needed
    ui->clockDisplay->setText(TimeToString(hours, mins, secs));

    // Remaining Time
    int remTimeInSecs = ui->hours->text().toInt()*3600 + ui->minutes->text().toInt()*60;
    int remTime = remTimeInSecs - seconds;

    hours = qFloor(remTime/3600);
    mins = qFloor(remTime - (hours*3600)) / 60;
    secs = 60 - secs;
    ui->clockDisplayRem->setText(TimeToString(hours, mins, secs));

}

QString MainWindow::TimeToString(int hours, int mins, int secs)
{
    QString StrHours = QString("%1").arg(hours, 2, 10, QChar('0'));
    QString StrMins = QString("%1").arg(mins, 2, 10, QChar('0'));
    QString StrSecs = QString("%1").arg(secs, 2, 10, QChar('0'));
    return StrHours + ":" + StrMins + ":" + StrSecs;
}

void MainWindow::checkTime(int seconds)
{
    int hours = qFloor(seconds/3600);
    int mins = qFloor(seconds - (hours*3600)) / 60;

    int uiHours = ui->hours->text().toInt();
    int uiMins = ui->minutes->text().toInt();

    if(hours == uiHours && (uiMins <= mins) && (mp3play->state() == QMediaPlayer::StoppedState)) // Check if the time is passed
    {
        this->showTop();
        QFile file(soundFile);
        if(file.open(QFile::ReadOnly))
        {
            mp3play->setMedia(QUrl::fromLocalFile(soundFile));
            mp3play->setVolume(100);
            mp3play->play();
        }
    }
}

void MainWindow::showTop()
{
    this->showNormal();
    this->activateWindow();
    this->raise();
}


void MainWindow::changeEvent(QEvent *event)
{
QMainWindow::changeEvent(event);
    if(event->type() == QEvent::WindowStateChange)
        if(isMinimized())
            this->hide();
}
