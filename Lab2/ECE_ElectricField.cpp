/*
Author: Nikhil Praveen ((nikhil.pravin@gmail.com))
ECE_ElectricField.cpp (c) 2023
Modified:  2023-09-15
Organisation: ECE 6122 Lab 1
Description:

Implementation of a class that calculates E at a given point

*/

#include "ECE_ElectricField.h"
#include "ECE_PointCharge.h"
#include <iostream>
#include <math.h>
#include <atomic>

using namespace std;

ECE_ElectricField::ECE_ElectricField() : Ex(0),
                                         Ey(0),
                                         Ez(0)
{
}

// Calculates Field at given coordinates
void ECE_ElectricField::computeFieldAt(double x_in, double y_in, double z_in)
{
    double k = 9000;
    double dist;

    // Calculating Euler's Distance
    dist = std::sqrt((x_in - x) * (x_in - x) + (y_in - y) * (y_in - y) + (z_in - z) * (z_in - z));
    k = k * q / (dist * dist * dist);

    // Using Euler's formula for EField calculation for vectors
    Ex = (k) * ((x_in - x));
    Ey = (k) * ((y_in - y));
    Ez = (k) * ((z_in - z));
    // }
}

// Returns ElectricField Values
void ECE_ElectricField::getElectricField(double &Ex_in, double &Ey_in, double &Ez_in)
{
    Ex_in = Ex;
    Ey_in = Ey;
    Ez_in = Ez;
}

// int main(){

//     double Ex, Ey, Ez;

//     ECE_ElectricField test;

//     test.setLocation(1,1,1);
//     test.setCharge(1);

//     test.computeFieldAt(2,5,5);

//     test.getElectricField(Ex, Ey, Ez);

//     std::cout << Ex << Ey << Ez;

// }