// Copyright (c) 2016 Tchernopyatov Alexey. Contacts: alexey@tchernopyatov.com
// Under MIT license, view LICENSE.txt

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
    setCount = 0;
    setPos = 0;
    elapsedTime = 0;
    remainedTime = 0;
    exPos = 0;

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
    setNameLabel->setText(currentLanguage.words["MW_NAME_LABEL"]);
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
    exersiseLabel->setText(currentLanguage.words["MW_EXERCISE_LABEL"]);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerTick()));

    this->setCentralWidget(centerWidget);
    QMenuBar* menuBar = createMainMenu(this);
    scanDataAndUpdateMenu();
    this->setMenuBar(menuBar);
    this->setWindowTitle(currentLanguage.words["MW_TITLE"]);
    this->resize(640,480);
    loadConfig();
    loadLanguages();
    for(int i = 0; i < languages.size(); i++) {
        if(configStruct.lang.compare(languages.at(i).lang.shorEnName) == 0) {
            currentLanguage = languages.at(i).lang;
            languages[i].action->setChecked(true);
            updateLanguage();
            break;
        }
    }
}

MainWindow::~MainWindow()
{

}
void MainWindow::updateSet() {
    setLabel->setText(currentLanguage.words["MW_SET_LABEL_SET"] +QString::number(setPos) + currentLanguage.words["MW_SET_LABEL_FROM"] + QString::number(setCount));
}

QMenuBar* MainWindow::createMainMenu(QWidget* wdg) {
    QMenuBar *mnuBar = new QMenuBar(wdg);//creating menu bar

    fileMenu = new QMenu(currentLanguage.words["MENU_FILE"]);//create menu file
    createEx = fileMenu->addAction(currentLanguage.words["MENU_CREATE_EXERCISE"],this,SLOT(createExercise()));
    loadEx = fileMenu->addAction(currentLanguage.words["MENU_LOAD_EXERCISE"],this,SLOT(loadExercise()));
    runEx = fileMenu->addAction(currentLanguage.words["MENU_RUN_EXERCISE"],this,SLOT(startPressed()));
    exMenu = new QMenu(currentLanguage.words["MENU_EXERCISE"]);//create menu file
    langMenu = new QMenu(currentLanguage.words["MENU_LANGUAGE"]);

    helpMenu = new QMenu(currentLanguage.words["MENU_HELP"]);
    helpTrigger = helpMenu->addAction(currentLanguage.words["MENU_HELP_TRIGGER"]);
    helpMenu->addSeparator();
    helpAbout = helpMenu->addAction(currentLanguage.words["MENU_HELP_ABOUT"], this,SLOT(actionTriggered()));
    mnuBar->addMenu(fileMenu);
    mnuBar->addMenu(exMenu);
    mnuBar->addMenu(langMenu);
    mnuBar->addMenu(helpMenu);
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
        exersiseLabel->setText(currentLanguage.words["STEP_READY"]);
    } else if (step == Step::STEADY) {
        step = Step::GO;
        exersiseLabel->setText(currentLanguage.words["STEP_STEADY"]);
    }else if (step == Step::GO) {
        step = Step::WORK;
        setPos = 1;
        remainedTime = selectedSet.exercise.at(0).time;
        elapsedTime = 0;
        setPos = 1;
        updateSet();
        exersiseLabel->setText(currentLanguage.words["STEP_GO"]);
    } else {
        if(elapsedTime == setCount*timePerEx) {
             exersiseLabel->setText(currentLanguage.words["STEP_FINISH"] );
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
            qDebug() << " remained is zero";
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
    QString path = QFileDialog::getOpenFileName(this,currentLanguage.words["DLG_OPEN_EXERCISE"],dir.absolutePath());
    qDebug() << " path = " << path;
    QFile jsonFile(path);
    jsonFile.open(QFile::ReadOnly);
    createExerciseDialog->loadExerciseData(jsonFile.readAll());
    createExercise();
}

void MainWindow::createExercise() {
    if(createExerciseDialog->exec()) {
        qDebug() << createExerciseDialog->getExerciseData();
        QDir dir(qApp->applicationDirPath());
        dir.cd("data");
        QString path = QFileDialog::getSaveFileName(this,currentLanguage.words["DLG_SAVE_EXERCISE"],dir.absolutePath());
        this->writeDataToFile(path,createExerciseDialog->getExerciseData());

        scanDataAndUpdateMenu();
    }
}
void MainWindow::writeDataToFile(const QString &file, const QByteArray &data) {
    QFile jsonFile(file);
    jsonFile.open(QFile::WriteOnly);
    jsonFile.write(data);
    jsonFile.flush();
}

SetStruct MainWindow::loadExerciseData(const QString &file) {
    QFile jsonFile(file);
    jsonFile.open(QFile::ReadOnly);
    QJsonDocument doc;
    doc = QJsonDocument::fromJson(jsonFile.readAll());
    SetStruct ss(doc);
    return ss;
}

LanguageStruct MainWindow::loadLanguageData(const QString &file) {
    QFile jsonFile(file);
    jsonFile.open(QFile::ReadOnly);
    QJsonDocument doc;
    doc = QJsonDocument::fromJson(jsonFile.readAll());
    LanguageStruct data(doc);
    return data;
}

/**
 *
 * @date 2015.02.07
 */
void MainWindow::loadConfig() {
    QDir dir(qApp->applicationDirPath());
    //dir.cd("data");
    QFile jsonFile(dir.filePath("config.json"));
    jsonFile.open(QFile::ReadOnly);
    QJsonDocument doc;
    doc = QJsonDocument::fromJson(jsonFile.readAll());
    qDebug() << "readed " << doc.toJson();
    configStruct = ConfigStruct(doc);
}
/**
 * Загружаем набор языков
 * @author Чернопятов А.В.
 * @date 2015.02.07
 */
void MainWindow::loadLanguages() {
    for(int i = 0; i < languages.size(); i++) {
        langMenu->removeAction(languages[i].action);
        disconnect(languages[i].action,SIGNAL(triggered()),this,SLOT(actionTriggered()));
        ass[i].action->deleteLater();
    }
    languages.clear();

    QDir dir(qApp->applicationDirPath());
    dir.cd("lang");
    QFileInfoList list = dir.entryInfoList();
    for(int i = 0; i < list.size(); i++) {
        qDebug() << "[" << i << "]" << list.at(i).absoluteFilePath();
        if(list.at(i).isFile() && list.at(i).suffix().compare("json") == 0) {
            LanguageStruct data = this->loadLanguageData(list.at(i).absoluteFilePath());
            if(data.fullName.length() > 0) {
                QAction *action = langMenu->addAction(data.fullName);
                action->setCheckable(true);
                connect(action,SIGNAL(triggered()),this,SLOT(actionTriggered()));
                ActionLanguageStruct tmp;
                tmp.action = action;
                tmp.lang = data;
                languages.push_back(tmp);
            }
        }
    }
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
    bool found = false;
    for(int i = 0; i < ass.size(); i++) {
        if(ass.at(i).action == act) {
            selectedSet = ass.at(i).set;
            found  = true;
            break;
        }
    }
    if(found) {
    for(int i = 0; i < selectedSet.exercise.size(); i++) {
        qDebug() << "[" << i << "] = " << selectedSet.exercise.at(i).name;
    }
    setNameLabel->setText(selectedSet.name);
    setCount = selectedSet.count;
    exPos = 0;
    timePerEx = 0;
    for(int i = 0; i < selectedSet.exercise.size(); i++) {
        timePerEx+=selectedSet.exercise.at(i).time;
    }
    qDebug() << "selectedSet = " << selectedSet.name;
    return;
    }
    found = false;
    for(int i = 0; i < languages.size(); i++) {
        languages[i].action->setChecked(false);
        if(languages.at(i).action == act) {
            qDebug() << " found lang " << languages.at(i).lang.shorEnName;
            currentLanguage = languages.at(i).lang;
            found  = true;
            languages[i].action->setChecked(true);
            //break;
        }
    }
    if(found) {
        updateLanguage();
        configStruct.lang = currentLanguage.shorEnName;
        QDir dir(qApp->applicationDirPath());
        writeDataToFile(dir.filePath("config.json"), configStruct.toJson());
    }
    if(act == helpAbout) {
        qDebug() << "about";
        QApplication::aboutQt();
    }
}

void MainWindow::updateLanguage() {
    qDebug() << __func__;
    if(selectedSet.name.length() == 0) {
        setNameLabel->setText(currentLanguage.words["MW_NAME_LABEL"]);
    }
    updateSet();
    exersiseLabel->setText(currentLanguage.words["MW_EXERCISE_LABEL"]);
    this->setWindowTitle(currentLanguage.words["MW_TITLE"]);
    fileMenu->setTitle(currentLanguage.words["MENU_FILE"]);
    createEx->setText(currentLanguage.words["MENU_CREATE_EXERCISE"]);
    loadEx->setText(currentLanguage.words["MENU_LOAD_EXERCISE"]);
    runEx->setText(currentLanguage.words["MENU_RUN_EXERCISE"]);

    exMenu->setTitle(currentLanguage.words["MENU_EXERCISE"]);

    langMenu->setTitle(currentLanguage.words["MENU_LANGUAGE"]);

    helpMenu->setTitle(currentLanguage.words["MENU_HELP"]);
    helpTrigger->setText(currentLanguage.words["MENU_HELP_TRIGGER"]);
    helpAbout->setText(currentLanguage.words["MENU_HELP_ABOUT"]);
    createExerciseDialog->setLanguage(currentLanguage);
}
