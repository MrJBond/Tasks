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
  //  f << '\n';
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
void DataProvider::cleanShapes(std::vector<Shape*>& shapes) {
	// Clean up
	for (int i = 0; i < shapes.size(); ++i) {
		delete shapes[i];
	}
	shapes.clear();
}

/****************************************************************************************
            Read any file and write the data to the shapes vector
****************************************************************************************/

std::string DataProvider::readNthLine(std::ifstream& file, int n) {

	// Reset file stream position to the beginning and clear any error flags
	file.clear();  // Clear any error flags (like EOF flag)
	file.seekg(0, std::ios::beg); // Go back to the start of the file
	if (!file.is_open()) {
		std::cerr << "Could not open the file " << std::endl;
		return "";  // Return an empty string if the file can't be opened
	}

	std::string line;
	int currentLine = 0;

	// Iterate through the file line by line
	while (std::getline(file, line)) {
		++currentLine;
		if (currentLine == n) {  // When the current line matches n
			return line;  // Return the nth line
		}
	}
	// If n exceeds the number of lines in the file, return an empty string
	return "";
}
int DataProvider::countLinesInFile(std::ifstream& file) {
	// Reset file stream position to the beginning and clear any error flags
	file.clear();  // Clear any error flags (like EOF flag)
	file.seekg(0, std::ios::beg); // Go back to the start of the file
	if (!file.is_open()) {
		std::cerr << "Could not open the file " << std::endl;
		return -1;  // Return an error code
	}

	int lineCount = 0;
	std::string line;
	while (std::getline(file, line)) {
		++lineCount;  // Count each line
	}
	return lineCount;
}
std::vector<double> DataProvider::stringToNumbers(std::string s) {
	std::vector<double> vec;
	std::istringstream iss(s);
	double n;
	// Extract numbers from the string
	while (iss >> n) {
		vec.push_back(n);
	}
	return vec;
}
Point2d DataProvider::getPoint(std::string point) {
	std::vector<double> p = stringToNumbers(point);

	if (p[0] == THROW_ERR || p[1] == THROW_ERR) {
		throw ReadError("Error with the point!");
	}

	return Point2d(p[0], p[1]);
}
void DataProvider::getRect(std::ifstream& file, std::vector<Shape*>& shapes, std::vector<Point2d>& points, int& i) {
	// Read 4 lines with points
	for (int j = 0; j < 4; ++j) {
		std::string point = readNthLine(file, ++i);

		Point2d p;
		try {
			p = getPoint(point);
		}
		catch (const ReadError& e) {
			std::cout << e.what();
			return; // can't create
		}

		// write points all 
		points.push_back(p);
	}
	Rect* rect = new Rect();
	rect->set(points[0], points[1], points[2], points[3]);
	shapes.push_back(rect);
}
void DataProvider::getCircle(std::ifstream& file, std::vector<Shape*>& shapes, std::vector<Point2d>& points, int& i) {
	std::string center_ = readNthLine(file, ++i);
	Point2d center;

	try {
		center = getPoint(center_);
	}
	catch (const ReadError& e) {
		std::cout << e.what();
		return; // can't create
	}
	std::string s = readNthLine(file, ++i);
	double radius = stringToNumbers(s)[0];

	if (radius == THROW_ERR) {
		std::cout << "Can't create!" << std::endl;
		return;
	}

	Circle* c = new Circle();
	c->set(center, radius);
	shapes.push_back(c);
}
void DataProvider::getArch(std::ifstream& file, std::vector<Shape*>& shapes, std::vector<Point2d>& points, int& i) {
	std::string center_ = readNthLine(file, ++i);
	Point2d center;

	try {
		center = getPoint(center_);
	}
	catch (const ReadError& e) {
		std::cout << e.what();
		return; // can't create
	}
	std::string s = readNthLine(file, ++i);
	double radius = stringToNumbers(s)[0];

	if (radius == THROW_ERR) {
		std::cout << "Can't create!" << std::endl;
		return;
	}

	std::string angles_ = readNthLine(file, ++i);
	std::vector<double> angles = stringToNumbers(angles_);

	if (angles[0] == THROW_ERR || angles[1] == THROW_ERR) {
		std::cout << "Can't create!" << std::endl;
		return;
	}

	std::string controlP_ = readNthLine(file, ++i);
	Point2d controlP;

	try {
		controlP = getPoint(controlP_);
	}
	catch (const ReadError& e) {
		std::cout << e.what();
		return; // can't create
	}

	Arch* a = new Arch(center, radius, angles[0], angles[1], controlP);
	shapes.push_back(a);
}
void DataProvider::getPolyline(std::ifstream& file, int numberNumbersToRead, std::vector<Shape*>& shapes, std::vector<Point2d>& points, int& i) {
	// numbersToRead/2 for points
	for (int j = 0; j < numberNumbersToRead / 2; ++j) {
		std::string point = readNthLine(file, ++i);
		// write points all 

		Point2d p;
		try {
			p = getPoint(point);
		}
		catch (const ReadError& e) {
			std::cout << e.what();
			return; // can't create
		}

		points.push_back(p);
	}
	Polyline* polyline = new Polyline();
	for (const auto& p : points) {
		polyline->addPoint(p);
	}
	shapes.push_back(polyline);
}
void DataProvider::getPolygon(std::ifstream& file, int numberNumbersToRead, std::vector<Shape*>& shapes, std::vector<Point2d>& points, int& i) {
	// numbersToRead/2 for points
	for (int j = 0; j < numberNumbersToRead / 2; ++j) {
		std::string point = readNthLine(file, ++i);
		// write points all 

		Point2d p;
		try {
			p = getPoint(point);
		}
		catch (const ReadError& e) {
			std::cout << e.what();
			return; // can't create
		}

		points.push_back(p);
	}
	Polygon* polygon = new Polygon();
	for (const auto& p : points) {
		polygon->addPoint(p);
	}
	shapes.push_back(polygon);
}
void DataProvider::getUnknown(std::ifstream& file, std::vector<Shape*>& shapes, int& i) {
	// only one line with numbers
	std::string line = readNthLine(file, ++i);
	std::vector<double> v = stringToNumbers(line);
	UnknownType* ut = new UnknownType();
	for (int j = 0; j < v.size(); ++j) {
		ut->addEl(v[j]);
	}
	shapes.push_back(ut);
}
void DataProvider::readShapesFromFile(std::vector<Shape*>& shapes, std::string fileName) {
	cleanShapes(shapes);

	std::ifstream file(fileName);
	if (!file.is_open()) {
		std::cerr << "Could not open the file " << fileName << std::endl;
		return;
	}
	int numberOfLines = countLinesInFile(file);

	std::string line;

	std::string n = readNthLine(file, 1);
	std::cout << "There are " << n << " objects in the file" << std::endl;
	// from the second line
	for (int i = 2; i < numberOfLines; ++i) {
		std::string line = readNthLine(file, i);
		int type = stringToNumbers(line)[0]; // in case the type is more than one digit long
		std::string s = readNthLine(file, ++i);
		int numberNumbersToRead = stringToNumbers(s)[0];


		std::vector<Point2d> points;
		switch (type) {
		case Objects::RECT: {
			getRect(file, shapes, points, i);
		}
		break;
		case Objects::CIRCLE: {
			getCircle(file, shapes, points, i);
		}
		break;
		case Objects::ARCH: {
			getArch(file, shapes, points, i);
		}
		break;
		case Objects::POLYLINE: {
			getPolyline(file, numberNumbersToRead, shapes, points, i);
		}
		break;
		case Objects::POLYGON: {
			getPolygon(file, numberNumbersToRead, shapes, points, i);
		}
		break;
		default: {
			getUnknown(file, shapes, i);
		}
		break;
		}
	}

	file.close();
}

/**************************************************************
				Get data from any source
***************************************************************/

// use double instead of string for general case 
void DataProvider::createObjectsFromNumbers(std::vector<Shape*>& shapes, double n, int iteration) {

	static int numberObjs;
	static double type; // double to store NaN correctly
	static int numberNumsToExpect;
	static int numberNumsToExpectMem;
	static std::vector<double> obj;

	if (iteration == 1) { // assign them only once
		type = std::nan("1");

		numberNumsToExpect = 0;
		numberNumsToExpectMem = 0; /* to memorize the numberNumsToExpect(we will need it in the switch)
		because the  numberNumsToExpect will have been decreased by that moment*/

		numberObjs = n;
		if (numberObjs == THROW_ERR) {
			std::cerr << "The number of objects is broken!" << std::endl;
			// we can go and check the type
		}
		else
			std::cout << "There are " << numberObjs << " objects in the source" << std::endl;
		return;
	}

	// after some iterations
	if (std::isnan(type)) {
		type = n; // if type is THROW_ERR, it will return the default type
		return;
	}

	if (!isnan(type) && numberNumsToExpect == 0) {
		numberNumsToExpect = n;
		numberNumsToExpectMem = n;

		if (numberNumsToExpect == THROW_ERR) {
			// we don't know how many numbers to wait for
			// some objects may be in the shapes vector at this moment
			std::string err = "The number of numbers to wait for is broken!";
			std::string iter = "\nThe iteration number is " + std::to_string(iteration);
			throw ReadError(err + iter);
		}

		return;
	}

	// check n before pushing
	if (n == THROW_ERR) {
		// if n is broken, create the unknown type
		type = INT_MIN;
	}

	// Accumulate object data (one number per call)
	obj.push_back(n);
	numberNumsToExpect--;  // Decrease the count of expected numbers
	if (numberNumsToExpect != 0) { // if numbers are still there
		return;
	}

	// Once all expected numbers are collected, create the object
	if (obj.size() != numberNumsToExpectMem) { // in case something has gone wrong
		std::string err = "Wrong number of numbers for the object!";
		std::string iter = "\nThe iteration number is " + std::to_string(iteration);
		throw ReadError(err + iter);
	}

	Shape* object = nullptr;
	switch ((int)type) {
	case Objects::RECT: {
		object = new Rect();
		((Rect*)object)->set(Point2d(obj[0], obj[1]),
			Point2d(obj[2], obj[3]),
			Point2d(obj[4], obj[5]),
			Point2d(obj[6], obj[7])
		);
	}
	break;
	case Objects::CIRCLE: {
		object = new Circle();
		((Circle*)object)->set(Point2d(obj[0], obj[1]),
			obj[2]
		);
	}
	break;
	case Objects::ARCH: {
		object = new Arch();
		((Arch*)object)->set(Point2d(obj[0], obj[1]),
			obj[2]
		);
		((Arch*)object)->setAngles(obj[3], obj[4]);
		((Arch*)object)->setPoint(Point2d(obj[5], obj[6]));
	}
	break;
	case Objects::POLYGON: {
		object = new Polygon();
		for (int i = 1; i < obj.size(); i+=2) {
			((Polygon*)object)->addPoint(Point2d(obj[i - 1], obj[i]));
		}
	}
	break;
	case Objects::POLYLINE: {
		object = new Polyline();
		for (int i = 1; i < obj.size(); i+=2) {
			((Polyline*)object)->addPoint(Point2d(obj[i - 1], obj[i]));
		}
	}
	break;
	default: {
		object = new UnknownType();
		for (const auto& e : obj) {
			((UnknownType*)object)->addEl(e);
		}
	}
	 break;
	}

	// add the constructed object
	shapes.push_back(object);

	// reset vars
	type = std::nan("1");
	obj.clear();
	//numberNumsToExpect = 0; // it is 0 at this point
	numberNumsToExpectMem = 0;
}
