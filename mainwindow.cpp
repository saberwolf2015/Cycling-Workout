#include "mainwindow.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <Qtimer>
#include <QFont>
#include <QMenuBar>
#include <QDebug>
#include <QTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    timePerEx = 1;//30;//30;
    setCount = 4;
    setPos = 0;
    elapsedTime = 0;
    remainedTime = 0;

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


    exersizeLabel = new QLabel(centerWidget);
    mainLout->addLayout(timeLout);
    mainLout->addWidget(exersizeLabel);
    mainLout->addStretch();
    QFont bigFont("Times", 38, QFont::Bold);
    exersizeLabel->setFont(bigFont);
    exersizeLabel->setAlignment(Qt::AlignCenter);

    updateSet();
    updateTime();
    exersizeLabel->setText(QString::fromUtf8("Ожидание"));

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerTick()));

    this->setCentralWidget(centerWidget);
    QMenuBar* menuBar = createMainMenu(this);
    this->setMenuBar(menuBar);
    this->setWindowTitle(QString::fromUtf8("Training"));
    this->resize(640,480);



}

MainWindow::~MainWindow()
{

}
void MainWindow::updateSet() {
    setLabel->setText(QString::fromUtf8("Сет ")+QString::number(setPos) + QString::fromUtf8(" из ") + QString::number(setCount));
}

QMenuBar* MainWindow::createMainMenu(QWidget* wdg) {
    QMenuBar *mnuBar = new QMenuBar(wdg);//creating menu bar

    QMenu *pMenu = new QMenu(QString::fromUtf8("Файл"));//create menu file
    pMenu->addAction(QString::fromUtf8("Запуск"),this,SLOT(startPressed()));
    //pMenu->addAction(QString::fromUtf8("Добавить кандзи..."),this,SLOT(addKanjiClicked()));
    /*
    QAction *openFile = new QAction(QString::fromUtf8("Открыть..."),pMenu);
    connect(openFile, SIGNAL(triggered()),this,SLOT(openFilePressed()));
    pMenu->addAction(openFile);
    pMenu->addAction(QString::fromUtf8("Открыть номер..."),this,SLOT(openNumberPressed()));
    pMenu->addAction(QString::fromUtf8("Открыть бинаризованный номер..."),this,SLOT(openTrasholdedNumberPressed()));
    pMenu->addAction(QString::fromUtf8("Открыть сеть..."),this,SLOT(openNet()));
    pMenu->addAction(QString::fromUtf8("Открыть даные для обучения..."),this,SLOT(openLearningData()));
    pMenu->addAction(QString::fromUtf8("Создать шаблоны..."),this,SLOT(createTemplates()));
    pMenu->addAction(QString::fromUtf8("Открыть шаблоны..."),this,SLOT(loadTemplates()));
    QMenu* pMenu2 = new QMenu(QString::fromUtf8("Обработка"));
    pMenu2->addAction(QString::fromUtf8("Обработать картинки..."),this,SLOT(obrabotatImages()));

    pMenu2->addAction(QString::fromUtf8("Увеличить резкость"),this,SLOT(sharpenImage()));
    pMenu2->addAction(QString::fromUtf8("Найти номер"),this,SLOT(findNumber()));
    pMenu2->addAction(QString::fromUtf8("Найти номер по блокам"),this,SLOT(findNumberByBlock()));
    pMenu2->addAction(QString::fromUtf8("Бинаризация по Нобуюки Отсу"),this,SLOT(otsuTrashold()));
    pMenu2->addAction(QString::fromUtf8("Убрать черноту"),this,SLOT(deleteBlack()));
    pMenu2->addAction(QString::fromUtf8("Разбить по символам"),this,SLOT(splitNumber()));
    pMenu2->addAction(QString::fromUtf8("Добить нулями"),this,SLOT(zeroPadding()));
    pMenu2->addAction(QString::fromUtf8("Перевести в область частот"),this,SLOT(FFT()));
    pMenu2->addAction(QString::fromUtf8("Подготовить картинки"),this,SLOT(prepareImage()));
    pMenu2->addAction(QString::fromUtf8("Просканировать директорию"),this,SLOT(scanFolder()));
    pMenu2->addAction(QString::fromUtf8("Начать обучение"),this,SLOT(startLearning()));
    pMenu2->addAction(QString::fromUtf8("Начать обучение по моему"),this,SLOT(startMyLearning()));
    pMenu2->addAction(QString::fromUtf8("Остановить обучение"),this,SLOT(stopLearn()));
    pMenu2->addAction(QString::fromUtf8("Остановить обучение по моему"),this,SLOT(stopLearnMy()));
    pMenu2->addAction(QString::fromUtf8("Возобновить обучение"),this,SLOT(continueLearning()));
    pMenu2->addAction(QString::fromUtf8("Опознать..."),this,SLOT(recognize()));
    pMenu2->addAction(QString::fromUtf8("Опознать по шаблону..."),this,SLOT(templateRecognize()));
    pMenu2->addAction(QString::fromUtf8("Опознать номер по шаблону..."),this,SLOT(templateNumberRecognize()));
    pMenu2->addAction(QString::fromUtf8("Опознать номер через ИНС..."),this,SLOT(annNumberRecognize()));
    pMenu2->addAction(QString::fromUtf8("Проверка обучения ИНС и шаблонов..."),this,SLOT(testRecognition()));
    QMenu* pMenuOtchet = new QMenu(QString::fromUtf8("Для отчетности"));
    pMenuOtchet->addAction(QString::fromUtf8("Создать горизонтальную гистограмму"),this,SLOT(drawHist()));
    pMenuOtchet->addAction(QString::fromUtf8("Сохранить рисунок с разбивкой по квадратам"),this,SLOT(saveWorkSquares()));
    pMenuOtchet->addAction(QString::fromUtf8("Сохранить рисунок с выделенными квадратами"),this,SLOT(detectedSquares()));
    pMenuOtchet->addAction(QString::fromUtf8("Сохранить рисунок с распухшими выделенными квадратами"),this,SLOT(detectedSquaresBloat()));
    pMenuOtchet->addAction(QString::fromUtf8("Сохранить рисунок с обводкой"),this,SLOT(obvodkaSquares()));
    pMenuOtchet->addAction(QString::fromUtf8("Сохранить рисунок с объединенными квадратами"),this,SLOT(unitedSquares()));
    pMenuOtchet->addAction(QString::fromUtf8("Сохранить рисунок после удаления маленьких блоков"),this,SLOT(dropSmallSquares()));
    pMenuOtchet->addAction(QString::fromUtf8("Сохранить рисунок после удаления маленьких блоков и увеличения сомнительных"),this,SLOT(obvodka()));
*/
    QMenu *pMenu3 = new QMenu(QString::fromUtf8("Справка"));
    pMenu3->addAction(QString::fromUtf8("Вызов справки"));
    pMenu3->addSeparator();
    pMenu3->addAction(QString::fromUtf8("&О программе"));
    mnuBar->addMenu(pMenu);
    //mnuBar->addMenu(pMenu2);
    //mnuBar->addMenu(pMenuOtchet);
    mnuBar->addMenu(pMenu3);
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
        exersizeLabel->setText(QString::fromUtf8("Настарт"));
    } else if (step == Step::STEADY) {
        step = Step::GO;
        exersizeLabel->setText(QString::fromUtf8("Внимание"));
    }else if (step == Step::GO) {
        step = Step::PUSHJUMP;
        setPos = 1;
        remainedTime = timePerEx;
        elapsedTime = 0;
        exersizeLabel->setText(QString::fromUtf8("Марш"));
    } else {
        //6 min
        if(elapsedTime == setCount*timePerEx*3) {
             exersizeLabel->setText(QString::fromUtf8("ГОТОВО"));
             timer->stop();
             return;
        }
        if(remainedTime == 0) {
            qDebug() << " remained is zero";
            if(step == Step::PUSHJUMP) {
                qDebug() << "***press";
                step = Step::PRESS;
            }else if(step == Step::PRESS) {
                step = Step::HIGHLEGS;
                qDebug() << "***highlegs";
            } else if(step == Step::HIGHLEGS) {
                step = Step::PUSHJUMP;
                qDebug() << "***pushjump";
                setPos++;
            }
            remainedTime = timePerEx;
            updateSet();
        }

        if(remainedTime == timePerEx) {
            qDebug() << " remained is timePerEx";
            if(step == Step::PUSHJUMP) {
                qDebug() << " set pushjup";
                exersizeLabel->setText(QString::fromUtf8("Отжимание и выпригивание"));
            } else if(step == Step::PRESS) {
                qDebug() << " set press";
                exersizeLabel->setText(QString::fromUtf8("Пресс"));
            } else if(step == Step::HIGHLEGS) {
                qDebug() << " set highlegs";
                exersizeLabel->setText(QString::fromUtf8("Бег с подниманием колен"));
            }
        }
        updateTime();
        elapsedTime++;
        remainedTime--;
        qDebug() << "remain " << remainedTime;
    }
}

void MainWindow::updateTime() {
    QTime tm;
    tm.setHMS(0,0,elapsedTime);
    timerLabel->setText(tm.toString("hh:mm:ss"));
    tm.setHMS(0,0,remainedTime);
    remainLabel->setText(tm.toString("hh:mm:ss"));
}
