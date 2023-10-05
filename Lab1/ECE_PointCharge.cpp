/*
Author: Nikhil Praveen ((nikhil.pravin@gmail.com))
ECE_PointCharge.cpp (c) 2023
Modified:  2023-09-15
Organisation: ECE 6122 Lab 1
Description:

Point Charge object implementation

*/

#include "ECE_PointCharge.h"
#include <iostream>

// Constructor to initialise values

ECE_PointCharge::ECE_PointCharge() : x(0),
                                     y(0),
                                     z(0),
                                     q(0)
{
}

// Sets seperation

void ECE_PointCharge::setLocation(double x_in, double y_in, double z_in)
{
    x = x_in;
    y = y_in;
    z = z_in;
}
