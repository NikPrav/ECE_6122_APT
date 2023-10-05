/*
Author: Nikhil Praveen ((nikhil.pravin@gmail.com))
Lab0_Probem1.cpp (c) 2023
Class: ECE6122A: Advanced Programming Techniques
Modified:  2023-08-31
Description: 
    
    Demonstrating Insertion Stream Operators and Escape Sequences

*/

#include <iostream>
#include <iomanip>

int main()
{
    
    std::cout<<"My name is: Nikhil Praveen\n";

    // Escape Characters
    std::cout<<"This (\") is a double quote\n";
    std::cout<<"This (\') is a single quote\n";
    std::cout<<"This (\\) is a backslash\n";

    // No Escape character necessary here
    std::cout<<"This (/) is a forward slash\n";

}

