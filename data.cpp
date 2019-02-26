#include "data.h"
#include <exception>
#include <cmath>
#include <string>

my_data::my_data()
{
    id = "";
    passward = "";
}

my_data::my_data(std::string str)
{
    id = str;
    passward = "";
}

my_data::my_data(std::string na, std::string pass)
{
    id = na, passward = pass;
}

my_data::~my_data()
{

}

bool my_data::operator < (const my_data& data2) const
{
    int len1 = this->id.length();
    int len2 = data2.id.length();
    if(len1 != len2)
        return len1 < len2;
    return this->id < data2.id;
}

bool my_data::operator == (const my_data& data1) const
{
    if(this->id == data1.id)
        return true;
    return false;
}

my_data& my_data::operator = (const my_data& data1)
{
    this->id = data1.id;
    this->passward = data1.passward;
    return *this;
}//重载=
