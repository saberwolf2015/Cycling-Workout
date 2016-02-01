#ifndef CREATEEXERCISEDIALOG_H
#define CREATEEXERCISEDIALOG_H

/**
 * Виджет для создания сетов из упраженений
 * позволяет добавлять в сет упраженения, их длительность и число сетов
 * @author Чернопятов А.В.
 * @date 2015.02.02
 */

#include <QDialog>
class QSpinBox;
class QLabel;
class QBoxLayout;
class QPushButton;
class QLineEdit;
class ExerciseWidget;
class QGridLayout;


class CreateExerciseDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CreateExerciseDialog(QWidget *parent = 0);
    ~CreateExerciseDialog();
    QString getTexturePath();
    QString getExerciseData();
    void loadExerciseData(const QString& data);
signals:
private:
    QGridLayout* exerciseLout;
    QVector<ExerciseWidget*> exersiseVector;
    void createBaseSettings(QBoxLayout*lout);
    void createExerciseField(QBoxLayout*lout);
    QSpinBox* setCountSpinBox;
    QPushButton* addExerciseBtn;
    QLineEdit* nameEdit;
    void updateGrid();

    void createConfirmButton(QBoxLayout*lout);
public slots:
    void pushButtonClicked();
    void removeExercise();
};

#endif // CREATEEXERCISEDIALOG_H
