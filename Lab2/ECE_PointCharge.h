/*
Author: Nikhil Praveen ((nikhil.pravin@gmail.com))
ECE_PointCharge.h (c) 2023
Modified:  2023-09-15
Organisation: ECE 6122 Lab 1
Description:

Point Charge Header File

*/

// Ensure file is included only once
#pragma once

using namespace std;

class ECE_PointCharge
{
public:
    // Constructor to initialise the object
    ECE_PointCharge();
    // Setter function to set location of charges
    void setLocation(double x_in, double y_in, double z_in);
    // Setter funtion to set point charge magnitudes
    void setCharge(double q_in) { q = q_in; };

protected:
    // Store spacing of charges in 3D
    double x, y, z;
    // Store charge magnitude
    double q;
};