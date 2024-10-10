#include "DataProvider.h"
#include "Exceptions.h"


/*
double gData[] = {
    8, // -- число объектов в файле



    1, // 1 -- тип "квадрат" со сторонами, параллельными осям координат
    4, // 4 -- количество чисел double
    0.0, 0.0, // координаты - левый нижний угол
    10.0, 10.0, // координаты - правый верхний угол



    2, // 2 -- тип "окружность"
    3, // 3 -- количество чисел
    0.0, 0.0, // координаты - центр окружности
    25.0, // радиус окружности



    3, // 3 -- пока неизвестный тип
    5, // 4 -- количество чисел
    1., 2., 3., 4., 5., // данные



    4, // 4 - дуга окружности
    5, // 5 -- количество чисел
    150., 150.,  // координаты -- центр окружности
    44.,         // радиус окружности
    PI/4., PI/2., // уголы -- начало и конец дуги


    2, // 2 -- тип "окружность"
    3,
    10.0, THROW_ERR, // при чтении объекта выбросится исключение
    25.0,



    5, // 5 -- тип многоугольник
    10,
    10., 20., // координаты точки
    20., 30., // координаты точки
    35., 25., // координаты точки
    0., 50., // координаты точки
    0., 0.,   // координаты точки



    6, // -- тип "ломаная"
    10,
    0,0, // координаты точки
    1,1, // координаты точки
    0,1, // координаты точки
    0,2, // координаты точки
    5,5, // координаты точки


    28, // неизвестный объект
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
        f << 7 << '\n'; // количество чисел
        f << center;
        f << radius << '\n';
        f << a->getAngles();
        f << a->getControlPoint();
    }
    else if (Circle* c = dynamic_cast<Circle*>(obj); c != nullptr) {
        Point2d center = c->getCenter();
        double radius = c->getRadius();
        f << Objects::CIRCLE << '\n';
        f << 3 << '\n'; // количество чисел
        f << center;
        f << radius << '\n';
    }
    else if (UnknownType* ut = dynamic_cast<UnknownType*>(obj); ut != nullptr) {
        std::vector<double> data = ut->getData();
        f << Objects::UNKNOWN << '\n';
        f << data.size() << '\n'; // количество чисел
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
        f << points.size()*2 << '\n'; // количество чисел

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