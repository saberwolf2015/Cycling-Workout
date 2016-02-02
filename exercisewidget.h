#ifndef EXERCISEWIDGET_H
#define EXERCISEWIDGET_H

#include <QWidget>
#include "struct.h"
class QLineEdit;
class QSpinBox;
class QGroupBox;
class QPushButton;

/**
 * Виджет для упраженения, хранит имя упраженения и время выполнения
 * @author Чернопятов А.В.
 * @date 2015.02.02
 */
class ExerciseWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ExerciseWidget(QWidget *parent = 0);
    ~ExerciseWidget();
    ExerciseStruct getData();
    void setBoxTitle(const QString &data);
    void setData(const ExerciseStruct &data);
signals:
void removeMe();
public slots:
private:
    QLineEdit* nameEdit;
    QSpinBox* timeSpinBox;
    QGroupBox* groupBox;
    QPushButton* removeBtn;

};

#endif // EXERCISEWIDGET_H
