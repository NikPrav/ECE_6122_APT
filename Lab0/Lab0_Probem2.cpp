/*
Author: Nikhil Praveen ((nikhil.pravin@gmail.com))
Lab0_Probem2.cpp (c) 2023
Class: ECE6122: Advanced Programming Techniques
Modified:  2023-08-31
Description: 

    Program to calculate the sum of all the multiples of 3 and 5 of a number

*/
#include<iostream>

using namespace std;

/* Function to calculate the sum of multiples of a number
    Inputs: 
        max(int): The number whose sum of multiples has to be calculated
        cur(int): Current iteration value, starts at 0
        incr(int): The number whose multiple has to be summed up
    Outputs:
        (int) Sum of multiples until value cur. 
*/ 

int sumOfMultiples(int max, int cur, int incr)
{
    int sum;

    // Stop if current value exceeds max value
    if (cur < max)
    {
        // sum = sum + cur;
        // Check if its the first call of the function for formatting purposes
        if (cur > incr)
        {
            cout<<", "<<cur;
        } else 
        {
            cout<<" "<<cur;
        }

        // Recursively calling the function with the next value in the series   
        return cur + sumOfMultiples(max, cur + incr, incr);     
    }
    else 
    {
        // Recursive stack done
        return 0;
    }

}


// Main Function
int main()
{
    int n, multOf3, multOf5;
    string input;
    do
    {
        n = 0;

        // Check if any of the input characters are flagged as not int
        int flag = 1;

        // Taking Input
        cout<<"Please enter a natural number (0 to quit): ";
        getline(cin, input);

        // Checking if the input is valid character by character
        for (int i=0; i<input.size(); ++i )
        {
            if(input[i] == '-'){

                // Negative number
                cout<<"Invalid input, only positive numbers allowed \n";

                // Flagging input as invalid
                flag = 0;
            } else if (!isdigit(input[i]) && flag){

                // Non-integer Value
                cout<<"Invalid Input, it is not a number\n";

                // Flagging input as invalid
                flag = 0;
            } else {

                // Converting char to int
                n = 10*n + input[i] - '0';
            }
        }

        if(n==0) 
        {
            // Exit Case
            break;
        }

        if (flag)
        {
            // Printing when input is valid
            cout<<"The multiples of 3 below "<<n<<" are:";
            multOf3 = sumOfMultiples(n,3,3);
            cout<<".\n";
            cout<<"The multiples of 5 below "<<n<<" are:";
            multOf5 = sumOfMultiples(n,5,5);
            cout<<".\n";
            cout<<"The sum of all multiples is: "<<multOf3+multOf5<<"\n";


        }
        

    } while(true);

    cout<<"Program Terminated.\nHave a nice day!\n";
    



}