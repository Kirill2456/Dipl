#ifndef TARGETTABLE_H
#define TARGETTABLE_H

#include <QWidget>
#include <QTableWidget>
#include <QTimer>
#include "target.h"

//QT_BEGIN_NAMESPACE
//namespace Ui { class TargetTable; }
//QT_END_NAMESPACE

class TargetTable : public QWidget
{
    Q_OBJECT

public:
    TargetTable(QWidget *parent = nullptr);
    ~TargetTable();

    void  calcFlightPoints();
    void  calcNextFlightPoints();

private slots:
    void slotEdTarg(QPoint pos_);
    void slotAdd();
    void slotDel();
    void slotChange();
    void slotCalc();
    void slotTimerAlarm();
private:

     QTableWidget*    m_widget;
     QVector<Target*> m_vec_targ;
     QTimer *         m_timer;

     void contextMenuCreate();
     void makeViewRow(int row);
     InitShipParams getParamsFromForm();
     InitParams getParamsFromTable( int row/*, int col*/);
     void putParamsToTable(Target &tar, int row);

signals:
    void sigPutDataToGraph(double, double, double);
};
#endif // TARGETTABLE_H
