#include "position.h"

position::position():
    x(0),
    y(0),
    d(40)
{

}

position::~position()
{

}

position::position(int cen_x, int cen_y, int rad)
{
    x = cen_x - rad;
    y = cen_y - rad;
    d = 2 * rad;
}

position& position::operator = (position& p)
{
    this->x = p.x;
    this->y = p.y;
    this->d = p.d;
    return *this;
}
