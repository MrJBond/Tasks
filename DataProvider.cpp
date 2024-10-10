#include "DataProvider.h"
#include "Exceptions.h"


/*
double gData[] = {
    8, // -- ����� �������� � �����



    1, // 1 -- ��� "�������" �� ���������, ������������� ���� ���������
    4, // 4 -- ���������� ����� double
    0.0, 0.0, // ���������� - ����� ������ ����
    10.0, 10.0, // ���������� - ������ ������� ����



    2, // 2 -- ��� "����������"
    3, // 3 -- ���������� �����
    0.0, 0.0, // ���������� - ����� ����������
    25.0, // ������ ����������



    3, // 3 -- ���� ����������� ���
    5, // 4 -- ���������� �����
    1., 2., 3., 4., 5., // ������



    4, // 4 - ���� ����������
    5, // 5 -- ���������� �����
    150., 150.,  // ���������� -- ����� ����������
    44.,         // ������ ����������
    PI/4., PI/2., // ����� -- ������ � ����� ����


    2, // 2 -- ��� "����������"
    3,
    10.0, THROW_ERR, // ��� ������ ������� ���������� ����������
    25.0,



    5, // 5 -- ��� �������������
    10,
    10., 20., // ���������� �����
    20., 30., // ���������� �����
    35., 25., // ���������� �����
    0., 50., // ���������� �����
    0., 0.,   // ���������� �����



    6, // -- ��� "�������"
    10,
    0,0, // ���������� �����
    1,1, // ���������� �����
    0,1, // ���������� �����
    0,2, // ���������� �����
    5,5, // ���������� �����


    28, // ����������� ������
    10,
    90,91,92,93,94,95,96,97,98,99
};

DataProvider::DataProvider(void) : c(0), maxC(sizeof(gData) / sizeof(gData[0]))
{
}

DataProvider::~DataProvider(void)
{
}*/

// Static methods should be defined outside the class
DataProvider* DataProvider::dataProvider = nullptr;


DataProvider* DataProvider::GetInstance(long maxC) {
    if (dataProvider == nullptr) {
        dataProvider = new DataProvider(maxC);
    }
    return dataProvider;
}

int DataProvider::rdInt() {
    checkC();
    int i = (int)gData[c++];
    if (i == THROW_ERR) {
        if (c == 1) { // the number of objects is wrong
            throw ReadError("The number of objects is wrong");
        }else
            throw ReadError("Reading error!");
    }
    return i;
}

double DataProvider::rdDouble() {
    checkC();
    double d = gData[c++];
    if (d == THROW_ERR) {
        throw ReadError("Reading error!");
    }
    return d;
}

void DataProvider::checkC() {
    if (c == maxC) {
        throw EndOfFile();
    }
}


void DataProvider::writeToFile(Shape* obj, std::string fileToWrite) {

    if (obj == nullptr) {
        std::cerr << "Can not write null to a file!" << std::endl;
        return;
    }

    // Open the file in append mode
    std::ofstream f;
    f.open(fileToWrite, std::ios::app);

    if (Arch* a = dynamic_cast<Arch*>(obj); a != nullptr) { // Arch before Circle otherwise it would cast Arch obj to Circle
        Point2d center = a->getCenter();
        double radius = a->getRadius();
        f << Objects::ARCH << '\n';
        f << 7 << '\n'; // ���������� �����
        f << center;
        f << radius << '\n';
        f << a->getAngles();
        f << a->getControlPoint();
    }
    else if (Circle* c = dynamic_cast<Circle*>(obj); c != nullptr) {
        Point2d center = c->getCenter();
        double radius = c->getRadius();
        f << Objects::CIRCLE << '\n';
        f << 3 << '\n'; // ���������� �����
        f << center;
        f << radius << '\n';
    }
    else if (UnknownType* ut = dynamic_cast<UnknownType*>(obj); ut != nullptr) {
        std::vector<double> data = ut->getData();
        f << Objects::UNKNOWN << '\n';
        f << data.size() << '\n'; // ���������� �����
        for (const auto& e : data) {
            f << e << " ";
        }
        f << '\n';
    }
    else {

        std::vector<Point2d> points = obj->getPoints();

        int type = INT_MAX; // default
        if (Rect* rect = dynamic_cast<Rect*>(obj); rect != nullptr) {
            type = Objects::RECT;
        }
        else if (Polygon* polygon = dynamic_cast<Polygon*>(obj); polygon != nullptr) { // Polygon before Polyline otherwise it would cast Polygon obj to Polyline
            type = Objects::POLYGON;
        }
        else if (Polyline* polyline = dynamic_cast<Polyline*>(obj); polyline != nullptr) { 
            type = Objects::POLYLINE;
        }
        
        f << type << '\n';
        f << points.size()*2 << '\n'; // ���������� �����

        for (const auto& p : points) {
            f << p;
        }
    }
    f << '\n';
    f.close();
}


// write to gData
void DataProvider::readFromFile(std::string fileName) {

    // Check if file name is valid (ends with .txt)
    if (fileName.length() < 5 || fileName.substr(fileName.length() - 4) != ".txt") {
        std::cerr << "Invalid file name: " << fileName << std::endl;
        return;
    }

    // Open file
    std::ifstream f(fileName);
    if (!f.is_open()) {
        std::cerr << "Error opening file: " << fileName << std::endl;
        return;
    }

    std::string line;
    std::vector<double> v; // using vector to check the size then 

    auto stringToNumbers = [&](std::string s) -> std::vector<double> {
        std::vector<double> vec;
        std::istringstream iss(s);
        double n;
        // Extract numbers from the string
        while (iss >> n) {
            vec.push_back(n);
        }
        return vec;
        };


    for (std::string line; std::getline(f, line); )
    {
        // get numbers from the line
        std::vector<double> nums = stringToNumbers(line);

        // write numbers
        for (const auto& n : nums) {
            v.push_back(n);
        }
    }
   
    if (v.size() == maxC) { // if the number of elements is correct
        for (int i = 0; i < maxC; ++i) {
            gData[i] = v[i]; // write data
        }
    }
    else {
        throw ReadError("Unable to read from file, the data was incorrectly written!");
    }

}