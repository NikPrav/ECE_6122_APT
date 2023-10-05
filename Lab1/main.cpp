/*
Author: Nikhil Praveen ((nikhil.pravin@gmail.com))
main.cpp (c) 2023
Modified:  2023-09-21
Organisation: ECE 6122 Lab 01
Description:

Main entry point for the program
Calculated electric field with multithreading

*/

#include <string>
#include <iostream>
#include <vector>
#include <thread>
#include <omp.h>
#include <chrono>
#include <cmath>
#include <atomic>
#include <sstream>
#include <regex>

#include "ECE_ElectricField.h"
#include "ECE_PointCharge.h"

using namespace std;

enum enStatus
{
    eWaiting,
    eRun,
    eFinished,
    eExit
};

enum varType
{
    intType,
    doubleType,
    naturalType
};

// Stores current status of threads
atomic<int> threadsStatus[100];

double xLoc, yLoc, zLoc;

// FUnction to csplit input strings and check validity
bool inputSanitation(string inputString, vector<string> &tokens)
{
    // Check if empty
    if (inputString.empty())
    {
        return false;
    }
    else
    {
        string token;
        istringstream tokenStream(inputString);

        // iterate through all words seperated by space and add to vector
        while (getline(tokenStream, token, ' '))
        {
            tokens.push_back(token);
        }
        return true;
    }
}

bool stringCheck(string inputString, int varType)
{
    // Expression for valid integer
    regex eInt("[\\-\\+]?([0-9]*)");

    regex eNatural("[\\+]?([0-9]*)");

    // Expression for valid double
    regex eDouble("[\\-\\+]?([0-9]*\\.[0-9]+|[0-9]+)");

    // Checking if regex matches
    switch (varType)
    {

    case intType:
        if (regex_match(inputString, eInt))
            return true;
        break;

    case doubleType:
        if (regex_match(inputString, eDouble))
            return true;
        break;
    case naturalType:
        if (regex_match(inputString,eNatural))
            return true;
        break;
        // default:
    }

    return false;
}

// Function for threading loop
void workerThread(int id, int iStart, int jStart, int iEnd, int jEnd, int cols, vector<vector<ECE_ElectricField>> &chargeMatrix)
{

    while (true)
    {
        int calls = 0;

        // wait for excecute signal
        while (threadsStatus[id] == eWaiting)
        {
            this_thread::yield();
        }

        for (int i = iStart; i <= iEnd; ++i)
        {
            for (int j = i == iStart ? jStart : 0; j < cols; ++j)
            {
                // break if required number of loops are excecuted
                if (i == iEnd && j == jEnd)
                    break;

                // Calculating Field
                // chargeMatrix[i][j].setLocation(j*xSep + originX, i*ySep + originY, 0);
                chargeMatrix[i][j].computeFieldAt(xLoc, yLoc, zLoc);
                calls++;
            }
        }

        // Signal excecution completion
        threadsStatus[id] = eFinished;

        // Wait until signaled what to do next
        while (threadsStatus[id] == eFinished)
        {
            std::this_thread::yield();
        }

        // Exits Thread
        if (threadsStatus[id] == eExit)
        {
            break;
        }
    }
}

// Function to find the exponent of a number
int expFinder(double x)
{
    int p = 0;

    // Counts power until the number is less than 10
    while (x >= 10)
    {
        p++;
        x = (double)x / 10;
    }
    return p;
}

int main()
{
    bool execDone;

    bool inputSanitary = true;
    // Generating 2D Array
    int rows, cols;

    int iStart, iEnd, jStart, jEnd;

    string inputString;

    double xSep, ySep, q, originX, originY;
    time_t start, end;

    // Getting max allowed threads
    unsigned int nThreads = std::thread::hardware_concurrency();
    cout << "Your computer supports " << nThreads << " concurrent threads \n";

    // Creating Threads stack
    vector<thread> threads;

    do
    {
        // Assuming input is valid
        inputSanitary = true;

        // Defining dimensions of charge matrix
        std::cout << "Please enter the number of rows and columns in the N x M array: ";

        getline(cin, inputString);
        vector<string> inputTokens;

        // CHhecking validity of input
        if (inputSanitation(inputString, inputTokens))
        {
            if (inputTokens.size() == 2 && stringCheck(inputTokens[0], naturalType) && stringCheck(inputTokens[1], naturalType))
            {
                rows = stoi(inputTokens[0]);
                cols = stoi(inputTokens[1]);
            }
            else
            {
                cout << "Sorry, invalid input, please try again \n";
                // inputSanitary = false;
                continue;
            }
        }

        // Generating Matrix
        vector<vector<ECE_ElectricField>> chargeMatrix(rows, vector<ECE_ElectricField>(cols));

        cout << "Please enter the x and y separation distances in meters: ";
        getline(cin, inputString);
        inputTokens.clear();

        // Checking validity of input
        if (inputSanitation(inputString, inputTokens))
        {
            if (inputTokens.size() == 2 && stringCheck(inputTokens[0], doubleType) && stringCheck(inputTokens[1], doubleType))
            {
                xSep = stod(inputTokens[0]);
                ySep = stod(inputTokens[1]);
            }
            else
            {
                cout << "Sorry, invalid input, please try again \n";
                // inputSanitary = false;
                continue;
            }
        }

        cout << "Please enter the common charge on the points in micro C: ";
        getline(cin, inputString);
        inputTokens.clear();

        // Checking validity of input
        if (inputSanitation(inputString, inputTokens))
        {
            if (inputTokens.size() == 1 && stringCheck(inputTokens[0], doubleType))
            {
                q = stod(inputTokens[0]);
            }
            else
            {
                cout << "Sorry, invalid input, please try again \n";
                continue;
            }
        }

        // Calculating number of loops per thread
        int loopsPerThread = ceil((double)rows * cols / (nThreads));

        // Launch worker threads
        // Initialising all threads to wait
        for (int i = 0; i < nThreads; ++i)
        {
            threadsStatus[i] = eWaiting;
            ;
        }

        // Assigning loops to threads
        // by giving the starting and ending indices
        // and then adding to stack
        for (int i = 0; i < nThreads; ++i)
        {
            iStart = (int)loopsPerThread * i / cols;
            iEnd = (int)loopsPerThread * (i + 1) / cols;

            jStart = (int)loopsPerThread * i % cols;
            jEnd = (int)loopsPerThread * (i + 1) % cols;

            // Preventing the loop from going out of bounds
            jEnd = iEnd >= rows ? cols : jEnd;
            iEnd = iEnd >= rows ? rows - 1 : iEnd;

            threads.push_back(thread(workerThread, i, iStart, jStart, iEnd, jEnd, cols, ref(chargeMatrix)));
        }

        do
        {
            inputSanitary = true;
            cout << "Please enter the location in space to determine the electric field (x y z) in meters:  ";

            getline(cin, inputString);
            inputTokens.clear();

            // Checking if input is valid or not
            if (inputSanitation(inputString, inputTokens))
            {
                // Converting to double if inputs are valid
                if (inputTokens.size() == 3 && stringCheck(inputTokens[0], doubleType) && stringCheck(inputTokens[1], doubleType) && stringCheck(inputTokens[2], doubleType))
                {
                    xLoc = stod(inputTokens[0]);
                    yLoc = stod(inputTokens[1]);
                    zLoc = stod(inputTokens[2]);
                }
                else
                {
                    cout << "Sorry, invalid input, please try again \n";
                    continue;
                }
            }

            // Initialising all threads to wait
            for (int i = 0; i < nThreads; ++i)
            {
                threadsStatus[i] = eWaiting;
                ;
            }

            // Setting origin offset
            // Ensuring that origin is centered in the matrix of charges
            if (rows % 2)
            {
                originY = -1 * ySep * rows / 2;
            }
            else
            {
                originY = -1 * ySep * (rows - 1) / 2;
            }

            if (cols % 2)
            {
                originX = -1 * xSep * cols / 2;
            }
            else
            {
                originX = -1 * xSep * (cols - 1) / 2;
            }

            // initialsing array wwith charges and locations
            for (int i = 0; i < rows; ++i)
            {
                for (int j = 0; j < cols; ++j)
                {
                    // Checking for Overlaps
                    if (zLoc == 0)
                    {
                        if ((xLoc == (i * xSep + originX)) && (yLoc == (j * ySep + originY)))
                        {
                            cout << "Location overlaps with charge, infinite electric field \n";
                            inputSanitary = false;
                        }
                    }
                    chargeMatrix[i][j].setLocation(i * xSep + originX, j * ySep + originY, 0);
                    chargeMatrix[i][j].setCharge(q);
                }
            }

            // Calculating EField
            // Beginning parellel code

            // Break if position overlaps with charges
            if (!inputSanitary)
            {
                continue;
            }
            auto start1 = chrono::high_resolution_clock::now();

            // Tell threads to start calcualtions
            for (int j = 0; j < nThreads; ++j)
            {
                threadsStatus[j] = eRun;
            }

            // Wait for the calculations to finish
            do
            {
                execDone = true;
                for (int j = 0; j < nThreads; ++j)
                {
                    if (threadsStatus[j] != eFinished)
                    {
                        execDone = false;
                        break;
                    }
                }
                std::this_thread::yield();
            } while (!execDone);

            // Adding the EField calculated by the threads
            double totalEx = 0, totalEy = 0, totalEz = 0, Ex, Ey, Ez;

            for (int i = 0; i < rows; ++i)
            {
                for (int j = 0; j < cols; ++j)
                {
                    // chargeMatrix[i][j].setLocation(j*xSep + originX, i*ySep + originY, 0);
                    chargeMatrix[i][j].getElectricField(Ex, Ey, Ez);
                    totalEx += Ex;
                    totalEy += Ey;
                    totalEz += Ez;
                }
            }

            // Stop the clock when execution is finished
            auto stop1 = chrono::high_resolution_clock::now();

            cout << "The electric field at (" << xLoc << "," << yLoc
                 << "," << zLoc << ") in V/m is \n";

            // Calculating total EField
            double Ef = sqrt(totalEx * totalEx + totalEy * totalEy + totalEz * totalEz);

            cout << "Ex = " << round(totalEx / pow(10, expFinder(totalEx)) * 10000) / 10000 << "x10^" << expFinder(totalEx) << "\n";
            cout << "Ey = " << round(totalEy / pow(10, expFinder(totalEy)) * 10000) / 10000 << "x10^" << expFinder(totalEy) << "\n";
            cout << "Ez = " << round(totalEz / pow(10, expFinder(totalEz)) * 10000) / 10000 << "x10^" << expFinder(totalEz) << "\n";
            cout << "|E| = " << round(Ef / pow(10, expFinder(Ef)) * 10000) / 10000 << "x10^" << expFinder(Ef) << "\n";

            auto duration = chrono::duration_cast<chrono::microseconds>(stop1 - start1);
            cout << "The calculation took : " << (double)(duration.count()) << " microsec!\n";

            char input;
            cout << "Do you want to enter a new location (Y?N)?";
            getline(cin, inputString);

            // Exit if user inputs N
            if (!inputString.compare("N"))
            {
                for (int j = 0; j < nThreads; ++j)
                {
                    threadsStatus[j] = eExit;
                }
                break;
            }

        } while (true);

        if (!inputString.compare("N"))
        {
            break;
        }

    } while (true);

    // Joining Threads after excecution
    for (auto &thread : threads)
    {
        thread.join();
    }
}