#include "targettable.h"
#include <QVBoxLayout>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QHeaderView>
#include <QAction>
#include <QMenu>
#include <QTableWidgetItem>

//---------------------------------------------------------------
TargetTable::TargetTable(QWidget *parent)
    : QWidget(parent)

{
    QVBoxLayout* mainLayout = new QVBoxLayout;
    setLayout( mainLayout );

    m_widget = new QTableWidget;
    m_widget->horizontalHeader()->setStretchLastSection(true);
    m_widget->resizeColumnsToContents();
    m_widget->setColumnCount(8); // задействуем 7 колонок
    QStringList headers; // формирование заголовка табл.

        headers.append("Курс (K)");
        headers.append("Дистанция (D)");
        headers.append("Скорость (V)");

        headers.append("Курс корабля");
        headers.append("Скорость корабля");

        headers.append("Курс (K`)");
        headers.append("Дистанция (D`)");
        headers.append("Пелинг (P`)");


    m_widget->setHorizontalHeaderLabels(headers); // установление заголовка в табл.
    m_widget->setEditTriggers(QTableWidget::NoEditTriggers); // возможность редактирования табл.(пока отключаем)
    mainLayout->addWidget(m_widget );

    m_widget->setRowCount(1);
    makeViewRow(0);

    contextMenuCreate();

    m_timer = new QTimer(this); // таймер ожидания ответа (подтверждения) от БД
    m_timer->setTimerType(Qt::PreciseTimer);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(slotTimerAlarm()));

}
//-------------------------------------------------
void TargetTable::makeViewRow(int row)
{
    QDoubleSpinBox * dsb = new QDoubleSpinBox;
    dsb->setAutoFillBackground(true);
    QPalette pl;
    pl.setBrush(QPalette::Active, QPalette::Base, Qt::lightGray);
    dsb->setPalette(pl);
    dsb->setMaximum(360);
    m_widget->setCellWidget ( row, 0,  dsb ); //курс

    QDoubleSpinBox * dsb2 = new QDoubleSpinBox;
    dsb2->setAutoFillBackground(true);
    pl.setBrush(QPalette::Active, QPalette::Base, Qt::lightGray);
    dsb2->setPalette(pl);
    dsb2->setMaximum(10000.0);
    m_widget->setCellWidget ( row, 1,  dsb2 ); // дист

    QDoubleSpinBox * dsb3 = new QDoubleSpinBox;
    dsb3->setAutoFillBackground(true);
    pl.setBrush(QPalette::Active, QPalette::Base, Qt::lightGray);
    dsb3->setPalette(pl);
    dsb3->setMaximum(100.0);
    m_widget->setCellWidget ( row, 2,  dsb3 ); // скор

    if(row ==0){

        QDoubleSpinBox * dsb6 = new QDoubleSpinBox;
        dsb6->setAutoFillBackground(true);
        pl.setBrush(QPalette::Active, QPalette::Base, Qt::cyan);
        dsb6->setPalette(pl);
        dsb6->setMaximum(360.0);
        m_widget->setCellWidget ( row, 3,  dsb6 );  // курс корабля

        QDoubleSpinBox * dsb7 = new QDoubleSpinBox;
        dsb7->setAutoFillBackground(true);
        pl.setBrush(QPalette::Active, QPalette::Base, Qt::cyan);
        dsb7->setPalette(pl);
        dsb7->setMaximum(100.0);
        m_widget->setCellWidget ( row, 4,  dsb7 );  // скор. корабля
    }

    QLineEdit * le4 = new QLineEdit;
    le4->setReadOnly(true);
    le4->setAutoFillBackground(true);
    pl.setBrush(QPalette::Active, QPalette::Base, Qt::lightGray);
    le4->setPalette(pl);
    m_widget->setCellWidget ( row, 5,  le4 ); // курс`

    QLineEdit * le5 = new QLineEdit;
    le5->setReadOnly(true);
    le5->setAutoFillBackground(true);
    pl.setBrush(QPalette::Active, QPalette::Base, Qt::lightGray);
    le5->setPalette(pl);
    m_widget->setCellWidget ( row, 6,  le5 );  //дист`

    QLineEdit * le8 = new QLineEdit;
    le8->setReadOnly(true);
    le8->setAutoFillBackground(true);
    pl.setBrush(QPalette::Active, QPalette::Base, Qt::lightGray);
    le8->setPalette(pl);
    m_widget->setCellWidget ( row, 7,  le8 ); // пеленг`

}
//-------------------------------------------------
TargetTable::~TargetTable()
{

}
//------------------------------------------------
void TargetTable::contextMenuCreate()
{
    m_widget->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(m_widget, SIGNAL(customContextMenuRequested(QPoint)),
                this, SLOT(slotEdTarg(QPoint)));
}
//-------------------------------------------------
void TargetTable::slotEdTarg(QPoint pos_)
{
    QAction *add, *del, *change, *calc;
    add = new QAction("&Добавить", m_widget);
    connect (add, SIGNAL(triggered()), this, SLOT(slotAdd()));

    del = new QAction("&Удалить", m_widget);
    connect (del, SIGNAL(triggered()), this, SLOT(slotDel()));

    change = new QAction("&Редактировать", m_widget);
    connect(change, SIGNAL(triggered()), this, SLOT(slotChange()));

    calc = new QAction("&Расчет", m_widget);
    connect(calc, SIGNAL(triggered()), this, SLOT(slotCalc()));

    QMenu *menu;
    menu = new QMenu(this);
    menu->addAction(add);
    menu->addAction(del);
    menu->addAction(change);
    menu->addAction(calc);
    menu->popup(m_widget->viewport()->mapToGlobal(pos_));
}
//-----------------------------------------------
void TargetTable::slotAdd()
{
   int r =  m_widget->rowCount();
   m_widget->setRowCount(r+1);
   makeViewRow(r);

}
//-----------------------------------------------
void TargetTable::slotDel()
{
   int r = m_widget->currentRow();
   m_widget->removeRow(r);
}
//-----------------------------------------------
void TargetTable::slotChange()
{

}
//-------------------------------------------------
void TargetTable::slotCalc()
{
    calcFlightPoints();
    m_timer->start(1000);
}
//-------------------------------------------------
void TargetTable::slotTimerAlarm()
{
    InitShipParams ship_params = getParamsFromForm();

    for(int i = 0; i< m_vec_targ.count(); ++i){
        InitParams init_param = getParamsFromTable(i);
        m_vec_targ.at(i)->setInitParam(init_param);
        m_vec_targ.at(i)->setShipParam(ship_params);
        m_vec_targ.at(i)->calculateNextFlightPoint();
        putParamsToTable(*m_vec_targ.at(i),i);

    }
}
//-------------------------------------------------
void TargetTable::calcFlightPoints(){

    InitShipParams ship_params = getParamsFromForm();

    for(int i = 0; i< m_widget->rowCount(); ++i){
        Target *tar= new Target;
        InitParams init_param = getParamsFromTable(i);
        tar->setInitParam(init_param);
        tar->setShipParam(ship_params);
        tar->calculateFlightPoint();
        putParamsToTable(*tar,i);
        m_vec_targ.append(tar);
    }
}
//------------------------------------------------
void TargetTable::calcNextFlightPoints(){

    InitShipParams ship_params = getParamsFromForm();

    for(int i = 0; i< m_vec_targ.count(); ++i){
        InitParams init_param = getParamsFromTable(i);
        m_vec_targ.at(i)->setInitParam(init_param);
        m_vec_targ.at(i)->setShipParam(ship_params);
        m_vec_targ.at(i)->calculateNextFlightPoint();
        putParamsToTable(*m_vec_targ.at(i),i);

    }
}

//-------------------------------------------------
InitShipParams TargetTable::getParamsFromForm(){
    InitShipParams ship_param_;

    QDoubleSpinBox * dsb_a;
    dsb_a = static_cast<QDoubleSpinBox *>(m_widget->cellWidget( 0, 3 )) ;
    ship_param_.A = dsb_a->value();

    QDoubleSpinBox * dsb_v;
    dsb_v = static_cast<QDoubleSpinBox *>(m_widget->cellWidget( 0, 4 )) ;
    ship_param_.V = dsb_v->value();

   return ship_param_;

}
//-------------------------------------------------
InitParams TargetTable::getParamsFromTable( int row/*, int col*/){

   InitParams init_;

   QDoubleSpinBox * dsb_a;
   dsb_a = static_cast<QDoubleSpinBox *>(m_widget->cellWidget( row, 0 )) ;
   init_.A = dsb_a->value();

   QDoubleSpinBox * dsb_d;
   dsb_d = static_cast<QDoubleSpinBox *>(m_widget->cellWidget( row, 1 )) ;
   init_.D = dsb_d->value();

   QDoubleSpinBox * dsb_v;
   dsb_v = static_cast<QDoubleSpinBox *>(m_widget->cellWidget( row, 2 )) ;
   init_.V = dsb_v->value();

   return init_;
}
//-------------------------------------------------
void TargetTable::putParamsToTable(Target &tar, int row){

    FlightPointTarg flight_point_= tar.getFlightPoint();

    QLineEdit * le_a;
    le_a = static_cast<QLineEdit*>(m_widget->cellWidget( row, 5 )) ;
    le_a->setText(QString::number(flight_point_.A,'g',6));

    QLineEdit * le_d;
    le_d = static_cast<QLineEdit *>(m_widget->cellWidget( row, 6 )) ;
    le_d->setText(QString::number(flight_point_.D));

    QLineEdit * le_p;
    le_p = static_cast<QLineEdit *>(m_widget->cellWidget( row, 7 )) ;
    le_p->setText(QString::number(flight_point_.P));

    emit sigPutDataToGraph(flight_point_.P, flight_point_.A, flight_point_.D);
    // emit sig with data to CustomPlot

}
//--------------------------------------------------
