#include "mainwindow.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <Qtimer>
#include <QFont>
#include <QMenuBar>
#include <QDebug>
#include <QTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    timePerEx = 30;//30;
    setCount = 4;
    setPos = 0;
    elapsedTime = 0;
    remainedTime = 0;
    exPos = 0;
    exercizes.clear();
    exercizes.push_back(QString::fromUtf8("Отжимание и выпригивание"));
    exercizes.push_back(QString::fromUtf8("Пресс"));
    exercizes.push_back(QString::fromUtf8("Бег с подниманием колен"));

    QWidget *centerWidget = new QWidget(this);
    QVBoxLayout* mainLout = new QVBoxLayout(centerWidget);
    timerLabel = new QLabel(centerWidget);
    QFont serifFont("Times", 32);

    remainLabel = new QLabel(centerWidget);
    setLabel = new QLabel(centerWidget);

    timerLabel->setFont(serifFont);
    timerLabel->setAlignment(Qt::AlignLeft);
    remainLabel->setFont(serifFont);
    remainLabel->setAlignment(Qt::AlignRight);

    setLabel->setFont(serifFont);
    setLabel->setAlignment(Qt::AlignCenter);

    QHBoxLayout* timeLout = new QHBoxLayout();
    timeLout->addWidget(timerLabel);
    timeLout->addWidget(setLabel);
    timeLout->addWidget(remainLabel);


    exersizeLabel = new QLabel(centerWidget);
    mainLout->addLayout(timeLout);
    mainLout->addWidget(exersizeLabel);
    mainLout->addStretch();
    QFont bigFont("Times", 38, QFont::Bold);
    exersizeLabel->setFont(bigFont);
    exersizeLabel->setAlignment(Qt::AlignCenter);

    updateSet();
    updateTime();
    exersizeLabel->setText(QString::fromUtf8("Ожидание"));

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerTick()));

    this->setCentralWidget(centerWidget);
    QMenuBar* menuBar = createMainMenu(this);
    this->setMenuBar(menuBar);
    this->setWindowTitle(QString::fromUtf8("Training"));
    this->resize(640,480);



}

MainWindow::~MainWindow()
{

}
void MainWindow::updateSet() {
    setLabel->setText(QString::fromUtf8("Сет ")+QString::number(setPos) + QString::fromUtf8(" из ") + QString::number(setCount));
}

QMenuBar* MainWindow::createMainMenu(QWidget* wdg) {
    QMenuBar *mnuBar = new QMenuBar(wdg);//creating menu bar

    QMenu *pMenu = new QMenu(QString::fromUtf8("Файл"));//create menu file
    pMenu->addAction(QString::fromUtf8("Запуск"),this,SLOT(startPressed()));
    QMenu *pMenu3 = new QMenu(QString::fromUtf8("Справка"));
    pMenu3->addAction(QString::fromUtf8("Вызов справки"));
    pMenu3->addSeparator();
    pMenu3->addAction(QString::fromUtf8("&О программе"));
    mnuBar->addMenu(pMenu);
    mnuBar->addMenu(pMenu3);
    mnuBar->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    return mnuBar;
}

void MainWindow::startPressed() {
    step = Step::READY;
    timer->start(1000);
}

void MainWindow::timerTick() {
    if(step == Step::READY) {
        step = Step::STEADY;
        exersizeLabel->setText(QString::fromUtf8("Настарт"));
    } else if (step == Step::STEADY) {
        step = Step::GO;
        exersizeLabel->setText(QString::fromUtf8("Внимание"));
    }else if (step == Step::GO) {
        step = Step::WORK;
        setPos = 1;
        remainedTime = timePerEx;
        elapsedTime = 0;
        setPos = 1;
        updateSet();
        exersizeLabel->setText(QString::fromUtf8("Марш"));
    } else {
        //6 min
        if(elapsedTime == setCount*timePerEx*exercizes.size()) {
             exersizeLabel->setText(QString::fromUtf8("ГОТОВО"));
             updateTime();
             timer->stop();
             return;
        }
        if(remainedTime == 0) {
            exPos++;
            if(exPos>=exercizes.size()) {
                exPos = 0;
                setPos++;
            }
            qDebug() << " remained is zero";
            remainedTime = timePerEx;
            updateSet();
        }

        if(remainedTime == timePerEx) {
            qDebug() << " remained is timePerEx";
            exersizeLabel->setText(exercizes.at(exPos));
            qDebug() << " set " << exercizes.at(exPos);
        }
        updateTime();
        elapsedTime++;
        remainedTime--;
        qDebug() << "remain " << remainedTime;
    }
}

void MainWindow::updateTime() {
    QTime tm;
    tm.setHMS(0,0,0);
    tm = tm.addSecs(elapsedTime);
    qDebug() << "elapsed = " << tm;
    timerLabel->setText(tm.toString("hh:mm:ss"));
    tm.setHMS(0,0,0);
    tm = tm.addSecs(remainedTime);
    qDebug() << "remained = " << tm;
    remainLabel->setText(tm.toString("hh:mm:ss"));
}
