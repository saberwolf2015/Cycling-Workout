#ifndef CREATEEXERCISEDIALOG_H
#define CREATEEXERCISEDIALOG_H

/**
 * Виджет для создания сетов из упраженений
 * позволяет добавлять в сет упраженения, их длительность и число сетов
 * @author Чернопятов А.В.
 * @date 2015.02.02
 */

#include <QDialog>
#include "struct.h"
class QSpinBox;
class QLabel;
class QBoxLayout;
class QPushButton;
class QLineEdit;
class ExerciseWidget;
class QGridLayout;
//struct SetStruct;
//struct LanguageStruct;


class CreateExerciseDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CreateExerciseDialog(QWidget *parent = 0);
    ~CreateExerciseDialog();
    QByteArray getExerciseData();
    void loadExerciseData(const QByteArray& data);
    void loadExerciseData(const SetStruct& data);
    void setLanguage(const LanguageStruct& lang);
signals:
private:
    LanguageStruct currentLanguage;/*! widget translation language */
    QGridLayout* exerciseLout;/*! layout for exercises */
    QVector<ExerciseWidget*> exersiseVector; /*! vector with widgets of exercises */
    /**
     * Базовые поля для создания упражнения
     * @author Чернопятов А.В.
     * @date 2015.02.02
     */
    void createBaseSettings(QBoxLayout*lout);
    /**
     * Создаёт лэйаут для упражнений
     * @author Чернопятов А.В.
     * @date 2015.02.02
     */
    void createExerciseField(QBoxLayout*lout);
    QSpinBox* setCountSpinBox; /*! number of cycles*/
    QPushButton* addExerciseBtn;
    QLineEdit* nameEdit; /*! line edit with name of exercise */
    QLabel* nameLbl;/*! label for name */
    QPushButton *btnOk;
    QPushButton *btnCancel;
    /**
     * Заново расставляем виджеты в лэйауте упражнений
     * и переименовываем их номера
     * @author Чернопятов А.В.
     * @date 2015.02.02
     */
    void updateGrid();
    /**
     * Добавляем виджет-упражнение на лэйаут
     * @author Чернопятов А.В.
     * @date 2015.02.02
     */
    ExerciseWidget* createExersise();
    /**
     * Создаём кнопки для подтверждения
     * @author Чернопятов А.В.
     * @date 2015.02.02
     */
    void createConfirmButton(QBoxLayout*lout);
public slots:
    /**
     * Была нажата кнопка
     * @author Чернопятов А.В.
     * @date 2015.02.02
     */
    void pushButtonClicked();
    /**
     * Был получен сигнал на удаление упражнения
     * @author Чернопятов А.В.
     * @date 2015.02.02
     */
    void removeExercise();
};

#endif // CREATEEXERCISEDIALOG_H
