#ifndef EXERCISEWIDGET_H
#define EXERCISEWIDGET_H

#include <QWidget>
#include "struct.h"
class QLineEdit;
class QSpinBox;
class QGroupBox;
class QPushButton;
class QLabel;

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
    void setLanguage(const LanguageStruct& lang);
signals:
/**
 * У виджета нажали кнопку удалить
 * @author Чернопятов А.В.
 * @date 2015.02.02
 */
void removeMe();
public slots:
private:
    LanguageStruct currentLanguage;/*! widget translation language */
    QLabel *nameLabel; /*! label with exercise name */
    QLineEdit* nameEdit;/*! line edit for exercise name */
    QSpinBox* timeSpinBox; /*! spinbox for time per exercise */
    QGroupBox* groupBox; /*! group box around elements */
    QPushButton* removeBtn; /*! button for remove widget */
};

#endif // EXERCISEWIDGET_H
