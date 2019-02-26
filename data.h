#ifndef DATA_H
#define DATA_H

#include<string>
class my_data
{
public:
    std::string id;
    std::string passward;
    my_data();
    my_data(std::string str);
    my_data(std::string na, std::string pass);
    ~my_data();
    bool operator < (const my_data& data1) const;
    bool operator == (const my_data& data1) const;
    my_data& operator = (const my_data& data1);
};

#endif // DATA_H
