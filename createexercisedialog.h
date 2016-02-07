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
    LanguageStruct currentLanguage;
    QGridLayout* exerciseLout;
    QVector<ExerciseWidget*> exersiseVector;
    void createBaseSettings(QBoxLayout*lout);
    void createExerciseField(QBoxLayout*lout);
    QSpinBox* setCountSpinBox;
    QPushButton* addExerciseBtn;
    QLineEdit* nameEdit;
    QLabel* nameLbl;
    QPushButton *btnOk;
    QPushButton *btnCancel;
    void updateGrid();
    ExerciseWidget* createExersise();

    void createConfirmButton(QBoxLayout*lout);
public slots:
    void pushButtonClicked();
    void removeExercise();
};

#endif // CREATEEXERCISEDIALOG_H
