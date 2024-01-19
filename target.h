#ifndef TARGET_H
#define TARGET_H

#include <QObject>

struct InitParams{
    double V; // скорость
    double A; // азимут(курс)
    double D; // дистанция
};

struct InitShipParams{ // нач. пар. корабля
    double V;
    double A;
    double D;
};


struct PointTarg{ //пролетная точка цели
    double x;
    double y;
};
struct PointShip{ //пролетная точка цели
    double x;
    double y;
};
struct FlightPointTarg{ //пролетная точка цели
    double A;
    double D;
    double P;
};

class Target
{
    InitParams           m_init_p;
    InitShipParams       m_ship_p;
    QVector<PointTarg>   m_vec_coord_targ;
    QVector<FlightPointTarg> m_vec_flight_point;
    PointShip            m_coord_ship;

public:
    Target();

void setInitParam(InitParams &);
void setShipParam(InitShipParams &);
//void setCoordTarg();


void setPoint(PointTarg &);
InitParams & getInitParams();
InitShipParams & getShipParams();
PointTarg & getPoint();
FlightPointTarg & getFlightPoint();

void calculateFlightPoint();
void calculateNextFlightPoint();


private:
void  calc_flight ();
void  calc_carrier_ship();
void  calculate();
float calcelation_distance_ship_to_target();
float degrees_peleng();
float ku_degrees(float peling);
void  change_carrier_ship();
void  change_tar();

};

#endif // TARGET_H
