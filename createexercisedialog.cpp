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
//#include <QVBoxLayout>
//#include <QHBoxLayout>

CreateExerciseDialog::CreateExerciseDialog(QWidget *parent) : QDialog(parent)
{
    qDebug() << __func__;
    QVBoxLayout* mainLout = new QVBoxLayout(this);
    createBaseSettings(mainLout);
    createExerciseField(mainLout);
    createConfirmButton(mainLout);
    mainLout->addStretch(1);
}

CreateExerciseDialog::~CreateExerciseDialog()
{

}

void CreateExerciseDialog::createBaseSettings(QBoxLayout*lout) {
    {
        QHBoxLayout* hlout = new QHBoxLayout();
        QLabel* nameLbl = new QLabel(this);
        nameLbl->setText(QString::fromUtf8("Название цикла"));
        hlout->addWidget(nameLbl);
        nameEdit = new QLineEdit(this);
        hlout->addWidget(nameEdit);
        lout->addLayout(hlout);
    }
    {
        QHBoxLayout* hlout = new QHBoxLayout();
        setCountSpinBox = new QSpinBox(this);
        setCountSpinBox->setPrefix(QString::fromUtf8("Число сетов: "));
        setCountSpinBox->setMinimum(1);
        setCountSpinBox->setMaximum(32);
        hlout->addWidget(setCountSpinBox);

        addExerciseBtn = new QPushButton(this);
        addExerciseBtn->setText(QString::fromUtf8("Добавить упражнение"));
        connect(addExerciseBtn,SIGNAL(clicked()),this,SLOT(pushButtonClicked()));
        hlout->addWidget(addExerciseBtn);

        lout->addLayout(hlout);
    }
}

void CreateExerciseDialog::createExerciseField(QBoxLayout *lout) {
    exerciseLout = new QGridLayout(this);
    lout->addLayout(exerciseLout);

}

void CreateExerciseDialog::pushButtonClicked() {
    QPushButton* btn =  (qobject_cast<QPushButton*>(sender()));
    if(btn == addExerciseBtn) {
        QString title = QString::fromUtf8("Упраженение") + QString::number(exersiseVector.size()+1);
        ExerciseWidget* wdg = new ExerciseWidget(this);
        connect(wdg,SIGNAL(removeMe()),this,SLOT(removeExercise()));
        wdg->setBoxTitle(title);
        exersiseVector.push_back(wdg);
        updateGrid();
    }
}

void CreateExerciseDialog::createConfirmButton(QBoxLayout *lout) {
    QPushButton *btnOk = new QPushButton(QString::fromUtf8("OK"),this);
    QPushButton *btnCancel = new QPushButton(QString::fromUtf8("Отмена"),this);
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
       qDebug() << "pre remove was" << exersiseVector.size();
       exersiseVector.removeOne(wdg);
       qDebug() << "after remove was" << exersiseVector.size();
       wdg->deleteLater();

       updateGrid();

}

void CreateExerciseDialog::updateGrid() {
    for(int i = 0; i < exersiseVector.size(); i++) {
         QString title = QString::fromUtf8("Упраженение") + QString::number(i+1);
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

QString CreateExerciseDialog::getExerciseData() {
    QJsonDocument doc;
    QVariantMap map;
    map.insert("name", nameEdit->text());
    QVariantMap step;
    //QJsonArray arr;
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
    return QString(doc.toJson());
}

void CreateExerciseDialog::loadExerciseData(const QString &data) {

}
