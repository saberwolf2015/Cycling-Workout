#include "mainwindow.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <Qtimer>
#include <QFont>
#include <QMenuBar>
#include <QDebug>
#include <QTime>
#include <QJsonObject>
#include <QFileDialog>
#include <QApplication> //qApp
#include <QFile>
#include <QFileInfoList>
#include "createexercisedialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    timePerEx = 30;//30;
    setCount = 4;
    setPos = 0;
    elapsedTime = 0;
    remainedTime = 0;
    exPos = 0;
    exercises.clear();
    exercises.push_back(QString::fromUtf8("Отжимание и выпригивание"));
    exercises.push_back(QString::fromUtf8("Пресс"));
    exercises.push_back(QString::fromUtf8("Бег с подниманием колен"));

    createExerciseDialog = new CreateExerciseDialog(this);

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

    setNameLabel = new QLabel(this);
    setNameLabel->setText(QString::fromUtf8("Выберите упражнение"));
    setNameLabel->setFont(serifFont);
    setNameLabel->setAlignment(Qt::AlignCenter);


    exersiseLabel = new QLabel(centerWidget);
    mainLout->addLayout(timeLout);
    mainLout->addWidget(setNameLabel);
    mainLout->addWidget(exersiseLabel);
    mainLout->addStretch();
    QFont bigFont("Times", 38, QFont::Bold);
    exersiseLabel->setFont(bigFont);
    exersiseLabel->setAlignment(Qt::AlignCenter);

    updateSet();
    updateTime();
    exersiseLabel->setText(QString::fromUtf8("Ожидание"));

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerTick()));

    this->setCentralWidget(centerWidget);
    QMenuBar* menuBar = createMainMenu(this);
    scanDataAndUpdateMenu();
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
    pMenu->addAction(QString::fromUtf8("Создать упраженение"),this,SLOT(createExercise()));
    pMenu->addAction(QString::fromUtf8("Загрузка упраженения"),this,SLOT(loadExercise()));
    pMenu->addAction(QString::fromUtf8("Запуск"),this,SLOT(startPressed()));
    exMenu = new QMenu(QString::fromUtf8("Упражнения"));//create menu file

    QMenu *pMenu3 = new QMenu(QString::fromUtf8("Справка"));
    pMenu3->addAction(QString::fromUtf8("Вызов справки"));
    pMenu3->addSeparator();
    pMenu3->addAction(QString::fromUtf8("&О программе"));
    mnuBar->addMenu(pMenu);
    mnuBar->addMenu(exMenu);
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
        exersiseLabel->setText(QString::fromUtf8("Настарт"));
    } else if (step == Step::STEADY) {
        step = Step::GO;
        exersiseLabel->setText(QString::fromUtf8("Внимание"));
    }else if (step == Step::GO) {
        step = Step::WORK;
        setPos = 1;
        remainedTime = selectedSet.exercise.at(0).time;
        elapsedTime = 0;
        setPos = 1;
        updateSet();
        exersiseLabel->setText(QString::fromUtf8("Марш"));
    } else {
        //6 min
        if(elapsedTime == setCount*timePerEx) {
             exersiseLabel->setText(QString::fromUtf8("ГОТОВО"));
             updateTime();
             timer->stop();
             return;
        }
        if(remainedTime == 0) {
            exPos++;
            if(exPos>=selectedSet.exercise.size()) {
                exPos = 0;
                setPos++;
            }
            qDebug() << " remained is sero";
            remainedTime = selectedSet.exercise.at(exPos).time;
            updateSet();
        }

        if(remainedTime == selectedSet.exercise.at(exPos).time) {
            qDebug() << " remained is timePerEx";
            exersiseLabel->setText(selectedSet.exercise.at(exPos).name);
            qDebug() << " set " << selectedSet.exercise.at(exPos).name;
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

void MainWindow::loadExercise() {
    QJsonObject data;
    QDir dir(qApp->applicationDirPath());
    dir.cd("data");
    QString path = QFileDialog::getOpenFileName(this,QString::fromUtf8("Открыть упражнение"),dir.absolutePath());
    qDebug() << " path = " << path;
    QFile jsonFile(path);
    jsonFile.open(QFile::ReadOnly);
    createExerciseDialog->loadExerciseData(jsonFile.readAll());
    createExercise();
}

void MainWindow::createExercise() {
    if(createExerciseDialog->exec()) {
        qDebug() << "accept";
        qDebug() << createExerciseDialog->getExerciseData();
        QDir dir(qApp->applicationDirPath());
        dir.cd("data");
        QString path = QFileDialog::getSaveFileName(this,QString::fromUtf8("Сохранить упражнение"),dir.absolutePath());
        QFile jsonFile(path);
        jsonFile.open(QFile::WriteOnly);
        jsonFile.write(createExerciseDialog->getExerciseData());
        jsonFile.flush();
        scanDataAndUpdateMenu();
    }
}

SetStruct MainWindow::loadExerciseData(const QString &file) {
    QFile jsonFile(file);
    jsonFile.open(QFile::ReadOnly);
    QJsonDocument doc;
    doc = QJsonDocument::fromJson(jsonFile.readAll());
    qDebug() << "readed " << doc.toJson();
    SetStruct ss(doc);
    return ss;
}

void MainWindow::scanDataAndUpdateMenu() {
    for(int i = 0; i < ass.size(); i++) {
        exMenu->removeAction(ass[i].action);
        disconnect(ass[i].action,SIGNAL(triggered()),this,SLOT(actionTriggered()));
        ass[i].action->deleteLater();
    }
    ass.clear();

    QDir dir(qApp->applicationDirPath());
    dir.cd("data");
    QFileInfoList list = dir.entryInfoList();
    for(int i = 0; i < list.size(); i++) {
        qDebug() << "[" << i << "]" << list.at(i).absoluteFilePath();
        if(list.at(i).isFile() && list.at(i).suffix().compare("json") == 0) {
            SetStruct ss = this->loadExerciseData(list.at(i).absoluteFilePath());
            if(ss.name.length() > 0) {
                QAction *action = exMenu->addAction(ss.name);
                connect(action,SIGNAL(triggered()),this,SLOT(actionTriggered()));
                ActionSetStruct tmp;
                tmp.action = action;
                tmp.set = ss;
                ass.push_back(tmp);

            }
        }
    }

}

void MainWindow::actionTriggered() {
    qDebug() << __PRETTY_FUNCTION__;
    QAction* act =  (qobject_cast<QAction*>(sender()));
    for(int i = 0; i < ass.size(); i++) {
        if(ass.at(i).action == act) {
            selectedSet = ass.at(i).set;
            break;
        }
    }
    setNameLabel->setText(selectedSet.name);
    setCount = selectedSet.count;
    timePerEx = 0;
    for(int i = 0; i < selectedSet.exercise.size(); i++) {
        timePerEx+=selectedSet.exercise.at(i).time;
    }
    qDebug() << "selectedSet = " << selectedSet.name;
}
