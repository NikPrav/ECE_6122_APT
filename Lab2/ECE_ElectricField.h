/*
Author: Nikhil Praveen ((nikhil.pravin@gmail.com))
ECE_ElectricField.h (c) 2023
Modified:  2023-09-15
Organisation: ECE 6122 Lab 1
Description:

Inherited class that models the electric field

*/

// include only once per compilation
#pragma once

#include "ECE_PointCharge.h"

// Class inheriting from ECE_PointCharge
class ECE_ElectricField : public ECE_PointCharge
{
public:
    // Constructor to initialise oblject
    ECE_ElectricField();
    // Cmputes Field at a point
    void computeFieldAt(double x, double y, double z);
    // Getter Function to get value of Electric Field
    void getElectricField(double &Ex, double &Ey, double &Ez);

private:
    // Stores value of Electric Field
    double Ex, Ey, Ez;
};