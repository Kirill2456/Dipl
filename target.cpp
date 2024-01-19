#include "target.h"
#include <math.h>
const double PI = 3.1415;
//-----------------------------------
Target::Target()
{

}
//----------------------------------
void Target::setPoint(PointTarg & param){

   PointTarg point;
   point.x = param.x;
   point.y = param.y;
   m_vec_coord_targ.append(point);

}
//----------------------------------
void Target::setInitParam(InitParams & params){
    m_init_p.A = params.A;
    m_init_p.V = params.V;
    m_init_p.D = params.D;
}
//----------------------------------
void Target::setShipParam(InitShipParams & params){
    m_ship_p.A = params.A;
    m_ship_p.V = params.V;
}
//----------------------------------
PointTarg & Target::getPoint(){
    return m_vec_coord_targ.last();
}
//----------------------------------
InitParams & Target::getInitParams(){
    return m_init_p;
}
//----------------------------------
InitShipParams & Target::getShipParams(){
    return m_ship_p;
}
//----------------------------------
void Target::calculateFlightPoint()
{
    calc_carrier_ship();
    calc_flight ();
    calculate();
}
//-------------------------------------
void Target::calculateNextFlightPoint()
{
    change_carrier_ship();
    change_tar ();
    calculate();
}
//--------------------------------------
FlightPointTarg & Target::getFlightPoint()
{
   return m_vec_flight_point.last();/*at(m_vec_flight_point.count()-1);//*/
}

//----------------------------------
void Target::calc_flight ()
{
 float temp,x,y;
 PointTarg point_;
 temp = (m_init_p.A)* PI / 180;
 x = m_init_p.D * sin(temp);
 y = m_init_p.D * cos(temp);

 point_.x = x;
 point_.y = y;

 m_vec_coord_targ.append(point_);
}
//-------------------------------------
void Target :: calc_carrier_ship()
{
    float rad, x, y;
    rad = (180 - m_ship_p.A)* PI / 180;
    x = m_ship_p.D * sin(rad);
    y = m_ship_p.D * cos(rad);
    m_coord_ship.x = x;
    m_coord_ship.y = y;
};

//-------------------------------------
void Target::calculate()
{
    FlightPointTarg flightPoint_{0.0,0.0,0.0};

    flightPoint_.D = calcelation_distance_ship_to_target();

    flightPoint_.P = degrees_peleng();

    flightPoint_.A = ku_degrees(flightPoint_.P);

   m_vec_flight_point.append(flightPoint_);
}
//-----------------------------------------------
float Target::calcelation_distance_ship_to_target(/*struct main_target* ptar, struct  carrier_ship* pship, int var, int j*/)
{
 float d = sqrt (((m_vec_coord_targ.last().x - m_coord_ship.x) * (m_vec_coord_targ.last().x - m_coord_ship.x)) +
                 ((m_vec_coord_targ.last().y - m_coord_ship.y) * (m_vec_coord_targ.last().y - m_coord_ship.x)));
 return d;
}
//------------------------------------------
float Target::degrees_peleng(/*struct main_target* ptar, struct  carrier_ship* pship*/)
{
 float dx1 = m_vec_coord_targ.last().x - m_coord_ship.x;
 float dy1 = m_vec_coord_targ.last().y - m_coord_ship.y;
 float arctan = (atan(dx1 / dy1) * 180 / PI);
 if (arctan < 0)
 {
  if (dx1 > 0 && dy1 < 0)
   return (180 + arctan);
  if (dx1 < 0 && dy1 < 0)
   return (270 + arctan);
  if (dx1 < 0 && dy1>0)
   return (360 + arctan);
  else
   return (90 + arctan);
 }
 if (dx1 < 0 && dy1 < 0)
  return (180 + arctan);
 return (arctan);
}

//-------------------------------------------------
float Target :: ku_degrees(float peling)
{
 float ku = peling - m_ship_p.A;
 if (ku >= 180)
 {
  ku = peling - m_ship_p.A;
  return ku;
 }
 if (ku <= -180)
 {
  return ku + 360;
 }
 else
  return ku;
}
//------------------------------------------------------
void Target :: change_carrier_ship(/*struct  carrier_ship* pship*/)
{
 float change_x;
 float change_y;
 float rad = (90 - m_ship_p.A) * PI / 180;
 change_x = m_ship_p.V * cos(rad) * 1;
 change_y = m_ship_p.V * sin(rad) * 1;

 m_coord_ship.x += change_x;
 m_coord_ship.y += change_y;
}
//--------------------------------------------------------
void Target :: change_tar(/*struct main_target* ptar, int var, int j*/)
{
 float change_x;
 float change_y;
 float rad = (90 - m_init_p.A) * PI / 180;
 change_x = m_init_p.V * cos(rad) * 1;
 change_y = m_init_p.V * sin(rad) * 1;

 PointTarg point_{0,0};
 point_.x += change_x;
 point_.y += change_y;

 if(m_vec_coord_targ.count()>0){
     point_.x += m_vec_coord_targ.last().x +change_x;
     point_.y += m_vec_coord_targ.last().y +change_y;
 }
 else{
     point_.x += change_x;
     point_.y += change_y;
 }
 m_vec_coord_targ.append(point_);

}
//-----------------------------------------------------
