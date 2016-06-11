#include "mainwindow.h"
#include "cpuinfo.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    //инициализацию начальных значений векторов
    vectXCPU.append(10);
    vectYCPU.append(265);
    vectXGPU.append(280);
    vectYGPU.append(265);

    setWindowTitle(tr("Monitoring CPU & GPU"));
    QWidget * wgt = new QWidget(this);
    setCentralWidget(wgt);
    //инициализция строк для отображения информации
    QString nameProcessor = "Model СPU: ";
    nameProcessor +=  objectForCPU.getNameProcessor();
    QString numbOfCores = "Numb Cores of CPU: ";
    numbOfCores += QString::number(objectForCPU.getNumbCores());
    QString totalSizeRam = "Total RAM size: ";
    totalSizeRam += QString::number(objectForCPU.getTotalSizeRAM()) + " Mb";
    //создание надписей
    QLabel *nameProc = new QLabel(nameProcessor);
    QLabel *numbCores = new QLabel(numbOfCores);
    QLabel *sizeRAM = new QLabel(totalSizeRam);
    //размещение надписей
    CPULayout  = new QVBoxLayout;
    CPULayout->addWidget(nameProc);
    CPULayout->addWidget(numbCores);
    CPULayout->addWidget(sizeRAM);

    GPULayout = new QVBoxLayout();
    QString nameGPU = "Model GPU: ";
    nameGPU +=  objectForGPU.getNameGPU();
    QLabel *nameGPULabel = new QLabel(nameGPU);

    GPULayout->addWidget(nameGPULabel);
    QWidget *tempGPUWidget = new QWidget();
    tempGPUWidget->setMinimumHeight(240);
    tempGPUWidget->setMinimumWidth(260);
    GPULayout->addWidget(tempGPUWidget);
    QWidget *tempCPUwgt = new QWidget();
    tempCPUwgt->setMinimumHeight(200);
    tempCPUwgt->setMinimumWidth(250);
    CPULayout->addWidget(tempCPUwgt);
    CPUInfoLabel = new QLabel();
    GPUInfoLabel = new QLabel();
    //запуск таймера
    connect( &m_timer, SIGNAL( timeout() ), SLOT( drawGraphicCPUUsage() ) );
    m_timer.start( 1 ); // Таймер будет срабатывать каждые 1000 миллисекунд, т.е. каждую секунду
    CPULayout->addWidget(CPUInfoLabel);
     GPULayout->addWidget(GPUInfoLabel);
    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->addLayout(CPULayout);
    mainLayout->addLayout(GPULayout);
    wgt->setLayout ( mainLayout );
}

MainWindow::~MainWindow()
{

}

void MainWindow::drawGraphicCPUUsage()
{
    repaint();
}

void MainWindow::paintEvent(QPaintEvent *event)
{

    Q_UNUSED(event);
    QPainter painter(this);     //переменная для рисования
    //установка пера
    painter.setPen(QPen(Qt::black, 3, Qt::SolidLine));
    //орисовка пустых прямоугольников
    painter.fillRect(10, 65, 260, 200, Qt::white);
    painter.fillRect(270, 65, 540, 200, Qt::white);
    //установка пера
    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine));
    //рисование сетки
    for(int x=10;x<=270;x+=20)
        painter.drawLine(x, 65, x,265);
    for(int y=65;y<=265;y+=20)
        painter.drawLine(10, y, 270,y);
    for(int x=280;x<=540;x+=20)
        painter.drawLine(x, 65, x,265);
    for(int y=65;y<=265;y+=20)
        painter.drawLine(280, y, 540,y);
    if(vectXCPU[vectXCPU.size()-1]==270)
    {
        vectXCPU.pop_front();
        vectYCPU.pop_front();
        for(int i=0;i<vectXCPU.length();i++)
            vectXCPU[i] -=20;
    }
    if(vectXGPU[vectXGPU.size()-1]==540)
    {
        vectXGPU.pop_front();
        vectYGPU.pop_front();
        for(int i=0;i<vectXGPU.length();i++)
            vectXGPU[i] -=20;
    }

    objectForGPU.UpdateState();     //обновление информации о GPU
    vectXCPU.append(vectXCPU[vectXCPU.size()-1]+20);
    vectYCPU.append(265-2*objectForCPU.getCPUUsage());
    vectXGPU.append(vectXGPU[vectXGPU.size()-1]+20);
    vectYGPU.append(265-2*objectForGPU.getGPUUsage());
    painter.setPen(QPen(Qt::green, 3, Qt::SolidLine));
    //отрисовка графиков
    for(int i=0;i<vectXCPU.size()-1;i++)
    {
        painter.drawLine(vectXCPU[i], vectYCPU[i], vectXCPU[i+1],vectYCPU[i+1]);
    }
    for(int i=0;i<vectXGPU.size()-1;i++)
    {
        painter.drawLine(vectXGPU[i], vectYGPU[i], vectXGPU[i+1],vectYGPU[i+1]);
    }
    //обновление надписей
    QString CPUInfo = "СPU Usage: ";
    CPUInfo += QString::number(objectForCPU.getCPUUsage()) + "%" + "\t" + "Current RAM usage: " +
            QString::number(objectForCPU.getCurrentRamUsage()) + "%";
    CPUInfoLabel->setText(CPUInfo);

    QString GPUInfo = "GPU Usage: ";
    GPUInfo +=  QString::number(objectForGPU.getGPUUsage()) + "%"+ "\t" + "GPU temperature: "
            + QString::number(objectForGPU.getTemperature()) + "°C";
    GPUInfoLabel->setText(GPUInfo);
}

