#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QSize>
#include <QGraphicsLineItem>
#include <QGraphicsRectItem>
#include <cpuinfo.h>
#include <gpuinfo.h>
#include <QPainter>
#include <QPaintEvent>
#include <QPixMap>
//Класс, описывающий главное окно прокраммы
class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    //объекты для получения информации о GPU и CPU
    CPUInfo objectForCPU;
    GPUInfo objectForGPU;
    //переменные, отвечающие за размещения объектов
    QVBoxLayout *CPULayout;
    QVBoxLayout *GPULayout;
    QTimer m_timer;         //таймер для обновления состояния
    QLabel *CPUInfoLabel;
    QLabel *GPUInfoLabel;
    //векторы, используемые для построения графиков
    QVector<int> vectXCPU;
    QVector<int> vectYCPU;
    QVector<int> vectXGPU;
    QVector<int> vectYGPU;

protected:
    void paintEvent(QPaintEvent *event);    //метод отрисовки графиков
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void drawGraphicCPUUsage();             //слот для отрисовки графиков
};

#endif // MAINWINDOW_H
