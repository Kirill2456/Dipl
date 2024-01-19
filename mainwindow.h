#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setupPolarPlotDemo(QCustomPlot *customPlot);
public slots:
    void slotDataToPlot(double peling, double kurs, double dist);

private:
    Ui::MainWindow *ui;
    QCPPolarAxisAngular *m_angularAxis;
    int m{0};
};
#endif // MAINWINDOW_H
