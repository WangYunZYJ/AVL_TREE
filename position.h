#ifndef POSITION_H
#define POSITION_H

class position
{
public:
    int x, y, d;
    //int center_x, center_y, radius;
    position();
    position(int cen_x, int cen_y, int rad = 20);
    ~position();
    position& operator = (position& p);
};

#endif // POSITION_H
