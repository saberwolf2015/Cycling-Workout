// Copyright (c) 2016 Tchernopyatov Alexey. Contacts: alexey@tchernopyatov.com
// Under MIT license, view LICENSE.txt

#include "createexercisedialog.h"
#include <QWidget>
#include <QLabel>
#include <QBoxLayout>
#include <QSpinBox>
#include <QPushButton>
#include <QFileDialog>
#include <QApplication>
#include <QPixmap>
#include <QDialog>
#include <QDebug>
#include <QLineEdit>
#include "exercisewidget.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QVariantMap>


CreateExerciseDialog::CreateExerciseDialog(QWidget *parent) : QDialog(parent)
{
    QVBoxLayout* mainLout = new QVBoxLayout(this);
    createBaseSettings(mainLout);
    createExerciseField(mainLout);
    createConfirmButton(mainLout);
    mainLout->addStretch(1);
}

CreateExerciseDialog::~CreateExerciseDialog()
{

}

/**
 * set language for dialog
 * @param lang - words for set
 * @author Чернопятов А.В.
 * @date 2015.02.07
 */
void CreateExerciseDialog::setLanguage(const LanguageStruct &lang) {
    currentLanguage = lang;
    nameLbl->setText(currentLanguage.words["CREATE_EXERCISE_DLG_CYCLE_NAME"]);
    setCountSpinBox->setPrefix(currentLanguage.words["CREATE_EXERCISE_DLG_SET_COUNT"]);
    addExerciseBtn->setText(currentLanguage.words["CREATE_EXERCISE_DLG_ADD_EXERCISE"]);
    this->btnOk->setText(currentLanguage.words["BTN_OK"]);
    this->btnCancel->setText(currentLanguage.words["BTN_CANCEL"]);
    for(int i = 0; i < exersiseVector.size(); i++) {
        exersiseVector[i]->setLanguage(lang);
    }
}

void CreateExerciseDialog::createBaseSettings(QBoxLayout*lout) {
    {
        QHBoxLayout* hlout = new QHBoxLayout();
        nameLbl = new QLabel(this);
        nameLbl->setText(currentLanguage.words["CREATE_EXERCISE_DLG_CYCLE_NAME"]);
        hlout->addWidget(nameLbl);
        nameEdit = new QLineEdit(this);
        hlout->addWidget(nameEdit);
        lout->addLayout(hlout);
    }
    {
        QHBoxLayout* hlout = new QHBoxLayout();
        setCountSpinBox = new QSpinBox(this);
        setCountSpinBox->setPrefix(currentLanguage.words["CREATE_EXERCISE_DLG_SET_COUNT"]);
        setCountSpinBox->setMinimum(1);
        setCountSpinBox->setMaximum(32);
        hlout->addWidget(setCountSpinBox);

        addExerciseBtn = new QPushButton(this);
        addExerciseBtn->setText(currentLanguage.words["CREATE_EXERCISE_DLG_ADD_EXERCISE"]);
        connect(addExerciseBtn,SIGNAL(clicked()),this,SLOT(pushButtonClicked()));
        hlout->addWidget(addExerciseBtn);

        lout->addLayout(hlout);
    }
}

void CreateExerciseDialog::createExerciseField(QBoxLayout *lout) {
    exerciseLout = new QGridLayout();
    lout->addLayout(exerciseLout);

}

ExerciseWidget* CreateExerciseDialog::createExersise() {
    QString title = currentLanguage.words["CREATE_EXERCISE_DLG_EXERCISE"]+ QString::number(exersiseVector.size()+1);
    ExerciseWidget* wdg = new ExerciseWidget(this);
    connect(wdg,SIGNAL(removeMe()),this,SLOT(removeExercise()));
    wdg->setBoxTitle(title);
    exersiseVector.push_back(wdg);
    return wdg;
}

void CreateExerciseDialog::pushButtonClicked() {
    QPushButton* btn =  (qobject_cast<QPushButton*>(sender()));
    if(btn == addExerciseBtn) {
        this->createExersise();
        updateGrid();
    }
}

void CreateExerciseDialog::createConfirmButton(QBoxLayout *lout) {
    btnOk = new QPushButton(currentLanguage.words["BTN_OK"],this);
    btnCancel = new QPushButton(currentLanguage.words["BTN_CANCEL"],this);
    QHBoxLayout *hlout = new QHBoxLayout();
    hlout->addWidget(btnOk);
    hlout->addWidget(btnCancel);
    connect(btnOk,SIGNAL(clicked()),SLOT(accept()));
    connect(btnCancel,SIGNAL(clicked()),SLOT(reject()));

    lout->addLayout(hlout);
}

void CreateExerciseDialog::removeExercise() {
       ExerciseWidget* wdg =  (qobject_cast<ExerciseWidget*>(sender()));
       disconnect(wdg,SIGNAL(removeMe()),this,SLOT(removeExercise()));
       exerciseLout->removeWidget(wdg);
       exersiseVector.removeOne(wdg);
       wdg->deleteLater();

       updateGrid();

}

void CreateExerciseDialog::updateGrid() {
    for(int i = 0; i < exersiseVector.size(); i++) {
         QString title = currentLanguage.words["CREATE_EXERCISE_DLG_EXERCISE"] + QString::number(i+1);
         exersiseVector[i]->setBoxTitle(title);
         exerciseLout->removeWidget(exersiseVector[i]);
         int maxR = 3;
         int maxC = 3;
         int sz = i;
         int row = sz/maxR;
         int col = sz - row*maxC;
         exerciseLout->addWidget(exersiseVector[i],row,col);
    }
}

QByteArray CreateExerciseDialog::getExerciseData() {
    QJsonDocument doc;
    QVariantMap map;
    map.insert("name", nameEdit->text());
    map.insert("set_count",setCountSpinBox->value());
    QVariantMap step;
    QVariantList lst;
    ExerciseStruct es;
    for(int i = 0; i< exersiseVector.size(); i++) {
        es= exersiseVector.at(i)->getData();
        step["name"] = es.name;
        step["time"] = es.time;
        lst.push_back(step);
    }
    map.insert("exercises",lst);


    QJsonObject json = QJsonObject::fromVariantMap(map);

    //obj["name"]=nameEdit->text();
    doc.setObject(json);
    qDebug() << " " << doc.toJson();
    return doc.toJson();
}

void CreateExerciseDialog::loadExerciseData(const QByteArray &data) {
    qDebug() << " data = " << data;
    QJsonDocument doc;
    doc = QJsonDocument::fromJson(data);
    qDebug() << "readed " << doc.toJson();
    SetStruct ss(doc);
    this->loadExerciseData(ss);
    updateGrid();
}

void CreateExerciseDialog::loadExerciseData(const SetStruct &data) {
    nameEdit->setText(data.name);
    setCountSpinBox->setValue(data.count);
    for(int i = 0; i < data.exercise.size(); i++) {
        qDebug() << "[" << i << "]" << data.exercise[i].name << " time = " << data.exercise[i].time;
        ExerciseWidget* wdg = this->createExersise();
        ExerciseStruct es;
        es.name = data.exercise[i].name;
        es.time = data.exercise[i].time;
        wdg->setData(es);
    }
}
