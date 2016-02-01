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

    groupBox->setTitle(QString::fromUtf8("Упражнение"));
    {
        QHBoxLayout*hlout = new QHBoxLayout();
        QLabel*lbl = new QLabel(groupBox);
        lbl->setText(QString::fromUtf8("Название упраженения"));
        nameEdit = new QLineEdit(groupBox);
        nameEdit->setText(QString::fromUtf8("упраженение"));
        hlout->addWidget(lbl);
        hlout->addWidget(nameEdit);

        groupLout->addLayout(hlout);
    }
    {
        timeSpinBox = new QSpinBox(groupBox);
        timeSpinBox->setPrefix(QString::fromUtf8("Время упражнения: "));
        timeSpinBox->setSuffix(QString::fromUtf8(" c"));
        timeSpinBox->setMinimum(1);
        timeSpinBox->setMaximum(3600);
        groupLout->addWidget(timeSpinBox);
    }
    {
        removeBtn = new QPushButton(groupBox);
        removeBtn->setText(QString::fromUtf8("Удалить"));
        groupLout->addWidget(removeBtn);
        connect(removeBtn,SIGNAL(clicked()),SIGNAL(removeMe()));
    }
    //groupBox->setLayout(groupLout);
    mainLout->addWidget(groupBox);

}

ExerciseWidget::~ExerciseWidget()
{

}

ExerciseStruct ExerciseWidget::getData() {
    ExerciseStruct data;
    data.name = nameEdit->text();
    data.time = timeSpinBox->value();
    return data;
}

void ExerciseWidget::setBoxTitle(const QString &data) {
    groupBox->setTitle(data);
}
