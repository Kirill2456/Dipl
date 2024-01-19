#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->customPlot->setGeometry(400, 250, 542, 390);
    setupPolarPlotDemo(ui->customPlot);
    ui->customPlot->replot();
    connect(ui->tableWidget, &TargetTable::sigPutDataToGraph, this, &MainWindow::slotDataToPlot);

}
//---------------------------------------------------------------------------
MainWindow::~MainWindow()
{
    delete ui;
}
//-------------------------------------------------------------------------
void MainWindow::setupPolarPlotDemo(QCustomPlot *customPlot)
{
  // Warning: Polar plots are a still a tech preview

  customPlot->plotLayout()->clear();
  m_angularAxis = new QCPPolarAxisAngular(customPlot);
  customPlot->plotLayout()->addElement(0, 0, m_angularAxis);

  /* This is how we could set the angular axis to show pi symbols instead of degree numbers:
  QSharedPointer<QCPAxisTickerPi> ticker(new QCPAxisTickerPi);
  ticker->setPiValue(180);
  ticker->setTickCount(8);
  polarAxis->setTicker(ticker);
  */

  customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
 // customPlot->setBackground(QBrush(QColor(30,30,30)));
  m_angularAxis->setRangeDrag(false);
  m_angularAxis->setTickLabelMode(QCPPolarAxisAngular::lmUpright);

  m_angularAxis->radialAxis()->setTickLabelMode(QCPPolarAxisRadial::lmUpright);
  m_angularAxis->radialAxis()->setTickLabelRotation(0);
  m_angularAxis->radialAxis()->setAngle(45);

  m_angularAxis->grid()->setAngularPen(QPen(QColor(400, 400, 400), 10, Qt::SolidLine));
  m_angularAxis->grid()->setSubGridType(QCPPolarGrid::gtAll);



 // QCPPolarGraph *g1 = new QCPPolarGraph(m_angularAxis, m_angularAxis->radialAxis());
 // QCPPolarGraph *g2 = new QCPPolarGraph(angularAxis, angularAxis->radialAxis());
//  g2->setPen(QPen(QColor(255, 150, 20)));
 // g2->setBrush(QColor(255, 150, 20, 50));
 // g1->setScatterStyle(QCPScatterStyle::ssDisc);
  //for (int i=0; i < 100; ++i)
 // {
 //   g1->addData(i/100.0*360.0, qSin(i/100.0*M_PI*8)*8+1);
   // g2->addData(i/100.0*360.0, qSin(i/100.0*M_PI*6)*2);
 // }
 // m_angularAxis->setRange(0, 360);
 // m_angularAxis->radialAxis()->setRange(0, 10);
  m_angularAxis->radialAxis()->setRange(0, 500);
  m_angularAxis->setRange(0, 360);
}
//-----------------------------------------------------------------------------------
void MainWindow::slotDataToPlot(double peling, double kurs, double dist)
{
    QCPPolarGraph *g1 = new QCPPolarGraph(m_angularAxis, m_angularAxis->radialAxis());
    ++m;
    //dist = 200;
    double x = dist * sin(kurs);
    double y = dist * cos(kurs);

      // g1->setBrush(Qt::red);
   /* for (int i=0; i < 100; ++i)
      {
           //  g1->addData(270+i, (200+i)*m);
        double kurs =i/100.0*360.0;
        double dist = 200.7;  //qSin(i/100.0*M_PI*8)*8+1;
        g1->addData(kurs, dist);
     }*/

   //  g1->addData(5/100.0*360.0, qSin(5/100.0*M_PI*8)*8+1*m);

    g1->addData(x, y);
    g1->setLineStyle(QCPPolarGraph ::lsNone);
    g1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle,QPen(Qt::blue),QBrush(Qt::green),8));

    ui->customPlot->replot();

}
//------------------------------------------------------------------------------------
