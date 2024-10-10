#pragma once
#include "Shape.h"
#include "circle.h"
#include "rect.h"
#include "arch.h"
#include "polygon.h"
#include "point2d.h"
#include "UnknownType.h"
#include <fstream>
#include <string>
#include <sstream>


#define THROW_ERR 23000000


using namespace geometry; // to use the operators

// I wanna make it a Singleton
class DataProvider
{
public:

    virtual ~DataProvider() {
        if (gData != nullptr) {
            delete[] gData;
            gData = nullptr;
        }
    }

    virtual int rdInt(); // read int
    virtual double rdDouble(); // read double
    void writeToFile(Shape* obj, std::string fileToWrite);
    void readFromFile(std::string fileName);

    double* getData() {
        if (gData != nullptr)
            return gData;
    }

    // Delete copy constructor and assignment operator
    DataProvider(DataProvider& other) = delete;
    void operator=(const DataProvider&) = delete;
    static DataProvider* GetInstance(long maxC);

    static void DestroyInstance() {
        delete dataProvider;
        dataProvider = nullptr;
    }

    void setArray(double* arr, long size) {
        // The array has been initialized in the constructor
        delete[] gData;
        this->gData = arr;
        this->c = 0;
        this->maxC = size;
    }
    size_t get_size() {
        return maxC;
    }
    long getC(){
        return c;
    }
    void setC(long c) {
        this->c = c;
    }

private:
    // Private constructor for Singleton
    DataProvider(long maxC) : maxC(maxC), c(0) {
        gData = new double[maxC];
    }
    static DataProvider* dataProvider;

    void checkC();

    long c = 0; // curPos
    long maxC = 0; 

    double *gData = nullptr;
};
