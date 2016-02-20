// Copyright (c) 2016 Tchernopyatov Alexey. Contacts: alexey@tchernopyatov.com
// Under MIT license, view LICENSE.txt

#include "exercisewidget.h"
#include <QBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QGroupBox>
#include <QPushButton>

ExerciseWidget::ExerciseWidget(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout* mainLout = new QVBoxLayout(this);
    groupBox = new QGroupBox(this);

    QVBoxLayout* groupLout = new QVBoxLayout(groupBox);

    groupBox->setTitle(currentLanguage.words["EXERCISE_WDG_EXERCISE"]);
    {
        QHBoxLayout*hlout = new QHBoxLayout();
        nameLabel = new QLabel(groupBox);
        nameLabel->setText(currentLanguage.words["EXERCISE_WDG_EXERCISE_NAME"] );
        nameEdit = new QLineEdit(groupBox);
        nameEdit->setText("");
        hlout->addWidget(nameLabel);
        hlout->addWidget(nameEdit);

        groupLout->addLayout(hlout);
    }
    {
        timeSpinBox = new QSpinBox(groupBox);
        timeSpinBox->setPrefix(currentLanguage.words["EXERCISE_WDG_EXERCISE_TIME_PREFIX"]);
        timeSpinBox->setSuffix(currentLanguage.words["EXERCISE_WDG_EXERCISE_TIME_SUFFIX"]);
        timeSpinBox->setMinimum(1);
        timeSpinBox->setValue(30);
        timeSpinBox->setMaximum(3600);
        groupLout->addWidget(timeSpinBox);
    }
    {
        removeBtn = new QPushButton(groupBox);
        removeBtn->setText(currentLanguage.words["BTN_DELETE"]);
        groupLout->addWidget(removeBtn);
        connect(removeBtn,SIGNAL(clicked()),SIGNAL(removeMe()));
    }
    //groupBox->setLayout(groupLout);
    mainLout->addWidget(groupBox);

}

ExerciseWidget::~ExerciseWidget()
{
}

/**
 * set language for widget
 * @param lang - words for set
 * @author Чернопятов А.В.
 * @date 2015.02.07
 */
void ExerciseWidget::setLanguage(const LanguageStruct &lang) {
    currentLanguage = lang;
    groupBox->setTitle(currentLanguage.words["EXERCISE_WDG_EXERCISE"]);
    nameLabel->setText(currentLanguage.words["EXERCISE_WDG_EXERCISE_NAME"] );
    if(nameEdit->text().length() == 0) {
        nameEdit->setText(currentLanguage.words["EXERCISE_WDG_DEFAULT_EXERCISE_NAME"]);
    }
    timeSpinBox->setPrefix(currentLanguage.words["EXERCISE_WDG_EXERCISE_TIME_PREFIX"]);
    timeSpinBox->setSuffix(currentLanguage.words["EXERCISE_WDG_EXERCISE_TIME_SUFFIX"]);
    removeBtn->setText(currentLanguage.words["BTN_DELETE"]);
}

ExerciseStruct ExerciseWidget::getData() {
    ExerciseStruct data;
    data.name = nameEdit->text();
    data.time = timeSpinBox->value();
    return data;
}

void ExerciseWidget::setData(const ExerciseStruct &data) {
    nameEdit->setText(data.name);
    timeSpinBox->setValue(data.time);
}

void ExerciseWidget::setBoxTitle(const QString &data) {
    groupBox->setTitle(data);
}
