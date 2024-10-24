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
#include "Exceptions.h"
#include <functional>

using namespace geometry;

// I wanna make it a Singleton
class DataProvider
{
public:

    ~DataProvider() {
        if (m_gData != nullptr) {
            delete[] m_gData;
            m_gData = nullptr;
        }
    }

    int rdInt(); // read int
    double rdDouble(); // read double
    void writeToFile(Shape* obj, std::string fileToWrite);
    void readFromFile(std::string fileName);

    double* getData() {
        if (m_gData != nullptr)
            return m_gData;
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
        delete[] m_gData;
        m_gData = arr;
        m_c = 0;
        m_maxC = size;
    }
    size_t get_size() {
        return m_maxC;
    }
    long getC(){
        return m_c;
    }
    void setC(long c) {
        m_c = c;
    }

    void cleanShapes(std::vector<Shape*>& shapes);
    void readShapesFromFile(std::vector<Shape*>& shapes, std::string fileName);

    void createShape(std::vector<double> data, std::vector<Shape*>& shapes, int type);

    // create universal methods to read data
    double readNextNumFromFile(std::string fileName);
    void readData(std::vector<Shape*>& shapes, std::string fileName = "", 
        std::function<double(std::string)> getNextNum = nullptr);

private:
    // Private constructor for Singleton
    DataProvider(long maxC) : m_maxC(maxC), m_c(0) {
        m_gData = new double[maxC];
    }
    static DataProvider* dataProvider;

    void checkC();

    long m_c = 0; // curPos in the array
    long m_maxC = 0; // array size

    double * m_gData = nullptr;

    // Read any file
    std::string readNthLine(std::ifstream& file, int n) const;
    int countLinesInFile(std::ifstream& file) const;
    std::vector<double> stringToNumbers(std::string s) const;
    Point2d getPoint(std::string point) const;
    void getRect(std::ifstream& file, std::vector<Shape*>& shapes, std::vector<Point2d>& points, int& i) const;
    void getCircle(std::ifstream& file, std::vector<Shape*>& shapes, std::vector<Point2d>& points, int& i) const;
    void getArch(std::ifstream& file, std::vector<Shape*>& shapes, std::vector<Point2d>& points, int& i) const;
    void getPolyline(std::ifstream& file, int numberNumbersToRead, std::vector<Shape*>& shapes, std::vector<Point2d>& points, int& i) const;
    void getPolygon(std::ifstream& file, int numberNumbersToRead, std::vector<Shape*>& shapes, std::vector<Point2d>& points, int& i) const;
    void getUnknown(std::ifstream& file, std::vector<Shape*>& shapes, int& i) const;

    // the struct to store data for the createObjectsFromNumbers function
    struct ReceivedObj {
        int numberObjs;
        int type;
        int numberNumsToExpect;
        int numberNumsToExpectMem;
        std::vector<double> obj;
    };

    void createObjectsFromNumbers(std::vector<Shape*>& shapes, double n, int iteration, ReceivedObj& obj);

};
