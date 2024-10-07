// CivilDeveloperTask.cpp : Defines the entry point for the console application.
//

#include "DataProvider.h"
#include "WDraw.h"
#include "Exceptions.h"
#include <tchar.h>

using namespace geometry;
/*
* 
n == 1 : create Rect
n == 2 : create Circle
n == 3 : create UnknownType
n == 4 : create Arch
n == 5 : create Polygon
n == 6 : create Polyline

*/
Shape* factory(int n) {
	
	switch (n) {
	case Objects::RECT:
		return new Rect();
	case Objects::CIRCLE:
		return new Circle();
	case Objects::ARCH:
		return new Arch();
	case Objects::POLYGON:
		return new Polygon();
	case Objects::POLYLINE:
		return new Polyline();
	default:
		return new UnknownType();
	}
}

void convertArrayToObj(DataProvider* dataProvider, std::vector<Shape*>& shapes) {

	try {
		// check the data (just for debugging)
		//for (int i = 0; i < size + 1; ++i) { // Looping to size + 1 to check the EndOfFile exception 
		//	std::cout << dataProvider->rdDouble() << std::endl;
		//}

		// Create objects and print them
		size_t numberOfObj = dataProvider->rdInt();
		std::cout << "There are " << numberOfObj << " objects." << std::endl;

		while (1) { // It will stop  on the end of the array (the exception will be caught)
			int type = dataProvider->rdInt();
			int n = dataProvider->rdInt();
			// read n doubles (for the object)
			int i = 0;
			std::vector<double> nums;
			while (i < n) {
				nums.push_back(dataProvider->rdDouble());
				i++;
			}
			std::vector<Point2d> points;

			if (type == Objects::RECT || type == Objects::POLYGON || type == Objects::POLYLINE) {// Not for Circle, Arch, Unknown
				for (int i = 1; i < n; i += 2) { // write points
					points.push_back(Point2d(nums[i - 1], nums[i]));
				}
			}
			// convert doubles to obj
			switch (type) {
			case Objects::RECT: {
				// Create obj
				Shape* rect = factory(Objects::RECT);
				((Rect*)rect)->set(points[0], points[1], points[2], points[3]);

				shapes.push_back(rect);

				break;
			}
			case Objects::CIRCLE: {
				// Create obj
				Shape* circle = factory(Objects::CIRCLE);
				// First two numbers are for the point and the last is radius
				((Circle*)circle)->set(Point2d(nums[0], nums[1]), nums[2]);

				shapes.push_back(circle);

				break;
			}
			case Objects::ARCH: {
				// Create obj

				Shape* arch = factory(Objects::ARCH);
				// First two numbers are for the point and the next is radius
				((Arch*)arch)->set(Point2d(nums[0], nums[1]), nums[2]);
				((Arch*)arch)->setAngles(nums[3], nums[4]); // Next are angles
				((Arch*)arch)->setPoint(Point2d(nums[n - 2], nums[n - 1])); // Two last are for control point

				shapes.push_back(arch);

				break;
			}
			case Objects::POLYGON: {
				// Create obj
				Shape* polygon = factory(Objects::POLYGON);
				for (const auto& p : points) {
					((Polygon*)polygon)->addPoint(p);
				}

				shapes.push_back(polygon);

				break;
			}
			case Objects::POLYLINE: {
				// Create obj

				Shape* polyline = factory(Objects::POLYLINE);
				for (const auto& p : points) {
					((Polyline*)polyline)->addPoint(p);
				}

				shapes.push_back(polyline);

				break;
			}
			default: {
				// Create obj
				Shape* ut = factory(Objects::UNKNOWN);
				for (const auto& e : nums) {
					((UnknownType*)ut)->addEl(e);
				}

				shapes.push_back(ut);
				break;
			}
			}

		}
	}
	catch (const EndOfFile& e) {
		std::cout << e.what();
	}

}


int _tmain(int argc, _TCHAR* argv[])
{
	// Create objects
	Shape* rect1 = factory(Objects::RECT);
	Rect* rect2 = new Rect();
	Shape* circle1 = factory(Objects::CIRCLE);
	Circle* circle2 = new Circle();
	Shape* arch1 = factory(Objects::ARCH);
	Arch* arch2 = new Arch();
	Arch* arch3 = new Arch(); // to test perimeter and bounding box
	Arch* arch4 = new Arch(); // to test perimeter and bounding box
	Shape* polygon1 = factory(Objects::POLYGON);
	Polygon* polygon2 = new Polygon();
	Shape* polyline1 = factory(Objects::POLYLINE);
	Polyline* polyline2 = new Polyline();
	Shape* unknown1 = factory(Objects::UNKNOWN);
	UnknownType* unknown2 = new UnknownType();

	// Set them

	{ // Rect
		Point2d p1(1.0, 1.0);
		Point2d p2(1.0, 5.0);
		Point2d p3(5.0, 5.0);
		Point2d p4(5.0, 1.0);
		((Rect*)rect1)->set(p1, p2, p3, p4);
		rect2->set(p1, p3);
	}
	{// Circle
		Point2d p1(1.0, 1.0);
		((Circle*)circle1)->set(p1, 5.0);
		circle2->set(p1, 5.0);
	}
	{ // Arch
		Point2d p1(150., 150.);
		((Arch*)arch1)->set(p1,44.0);
		((Arch*)arch1)->setAngles(PI/2, PI/2);
		((Arch*)arch1)->setPoint(Point2d(150.0, 194.0));
		(arch2)->set(p1, 44.0);
		(arch2)->setAngles(PI / 2, PI / 2);
		(arch2)->setPoint(Point2d(150.0, 194.0));
		(arch3)->set(p1, 44.0);
		(arch3)->setAngles(0.0, PI / 2);
		(arch3)->setPoint(Point2d(44.0 * cos(PI/4) + 150.0, 44.0 * sin(PI / 4) + 150.0)); // 45 degree
		(arch4)->set(p1, 44.0);
		(arch4)->setAngles(0.0, PI / 2); // bigger part
		(arch4)->setPoint(Point2d(44.0 * cos(3*PI / 2) + 150.0, 44.0 * sin(3*PI / 2) + 150.0)); // 3*PI/2

	}
	{ // Polyline
		Point2d p1(0.0, 0.0);
		Point2d p2(1.0, 1.0);
		Point2d p3(0.0, 1.0);
		Point2d p4(0.0, 2.0);
		Point2d p5(5.0, 5.0);
		((Polyline*)polyline1)->set(p1, p2, p3, p4, p5);
		polyline2->set(p1, p2, p3, p4, p5);
	}
	{ // Polygon
		Point2d p1(10.0, 20.0);
		Point2d p2(20.0, 30.0);
		Point2d p3(35.0, 25.0);
		Point2d p4(0.0, 50.0);
		Point2d p5(0.0, 0.0);
		((Polygon*)polygon1)->set(p1, p2, p3, p4, p5);
		polygon2->set(p1, p2, p3, p4, p5);
	}
	{ // Unknowntype
		for (int i = 0; i < 10; ++i) {
			((UnknownType*)unknown1)->addEl(i*1.4);
			unknown2->addEl(i * 1.4);
		}
	}

	////////////////////////////////////////////////////////////
	// test objs
	std::vector<Point2d> expectedRes = {Point2d(1.0, 1.0), Point2d(5.0, 5.0), Point2d(1.0, 5.0), Point2d(5.0, 1.0)};
	std::vector<Point2d> res = rect1->boundingBox();
	if (res != expectedRes)
		printf("FAIL 1\n");
	std::vector<Point2d> expectedResCircle = {Point2d(-4.0, -4.0), Point2d(6.0, 6.0), Point2d(-4.0, 6.0), Point2d(6.0, -4.0)};
	std::vector<Point2d> resCircle = circle1->boundingBox();
	if (resCircle != expectedResCircle)
		printf("FAIL 2\n");
	std::vector<Point2d> expectedResPolyline = {Point2d(0.0, 0.0), Point2d(5.0, 5.0), Point2d(0.0, 5.0), Point2d(5.0, 0.0)};
	std::vector<Point2d> resPolyline = polyline1->boundingBox();
	if (resPolyline != expectedResPolyline)
		printf("FAIL 3\n");
	std::vector<Point2d> expectedResPolygon = {Point2d(0.0, 0.0), Point2d(35.0, 50.0), Point2d(0.0, 50.0), Point2d(35.0, 0) };
	std::vector<Point2d> resPolygon = polygon1->boundingBox();
	if (resPolygon != expectedResPolygon)
		printf("FAIL 4\n");
	std::vector<Point2d> expectedResArch = { Point2d(106.0, 106.0), Point2d(194.0, 106.0), Point2d(106.0, 150.0), Point2d(194.0, 150.0) };
	std::vector<Point2d> resArch = arch1->boundingBox();
	if (resArch != expectedResArch)
		printf("FAIL 5\n");


	// perimeter
    double expectedPerRect = 16;
	double resPerRec = rect1->perimeter();
	if (resPerRec != expectedPerRect)
		printf("FAIL 6\n");
	double expectedPerCircle = 10*PI;
	double resPerCircle = circle1->perimeter();
	if (resPerCircle != expectedPerCircle)
		printf("FAIL 7\n");
	double expectedPerPolyline = 9.245;
	double resPerPolyline = std::round(polyline1->perimeter() * 10 * 10* 10)/(10*10*10); // round the result
	if (resPerPolyline != expectedPerPolyline)
		printf("FAIL 8\n");
	double expectedPerPolygon = 145.326;
	double resPerPolygon = std::round(polygon1->perimeter() * 10 * 10 * 10) / (10 * 10 * 10); // round the result
	if (resPerPolygon != expectedPerPolygon)
		printf("FAIL 9\n");
	double expectedPerArch = PI*44.0; // 180 degree so it will work
	double resPerArch = arch1->perimeter();
	if (resPerArch != expectedPerArch)
		printf("FAIL 10\n");
	double expectedPerArch3 = PI * 22.0; 
	double resPerArch3 = arch3->perimeter();
	if (resPerArch3 != expectedPerArch3)
		printf("FAIL 11\n");
	double expectedPerArch4 = std::round((2*PI*44.0 - PI * 22.0) *10*10*10) / (10 * 10 * 10);
	double resPerArch4 = std::round(arch4->perimeter()*10*10*10) / (10*10*10);
	if (resPerArch4 != expectedPerArch4)
		printf("FAIL 12\n");


	///////////////////////////////////////////////////////////


	WDraw drawer = WDraw();


	size_t size = 121;
	// Create singleton
	DataProvider* dataProvider = DataProvider::GetInstance(size);

	// write them to file
	std::ofstream file("file.txt");

	file << 12 << '\n'; // Number of objects
	file.close();
	dataProvider->writeToFile(rect1, "file.txt");
	dataProvider->writeToFile(rect2, "file.txt");
	dataProvider->writeToFile(circle1, "file.txt");
	dataProvider->writeToFile(circle2, "file.txt");
	dataProvider->writeToFile(arch1, "file.txt");
	dataProvider->writeToFile(arch2, "file.txt");
	dataProvider->writeToFile(polygon1, "file.txt");
	dataProvider->writeToFile(polygon2, "file.txt");
	dataProvider->writeToFile(polyline1, "file.txt");
	dataProvider->writeToFile(polyline2, "file.txt");
	dataProvider->writeToFile(unknown1, "file.txt");
	dataProvider->writeToFile(unknown2, "file.txt");
	
	bool read = true;
	try {
		// Read
		dataProvider->readFromFile("file.txt");
	}
	catch (const ReadError& e) {
		read = false;
		std::cout << e.what();
	}

	// The data should be in the array
	//double* data = dataProvider->getData();

	if (read) { // only if data is in the array
		std::vector<Shape*> shapes;

		try {
			convertArrayToObj(dataProvider, shapes);
		}
		catch (const ReadError& e) {
			std::cout << e.what();
			// clear the data which has been written
			for (int i = 0; i < shapes.size(); ++i) {
				delete shapes[i];
			}
		}

		
		// print
		for (int i = 0; i < shapes.size(); ++i) {
			drawer.drawObject(shapes[i]);
		}

		// do somethidng....

		// Clean up
		for (int i = 0; i < shapes.size(); ++i) {
			delete shapes[i];
		}
	}


	////////////////////////////////////////////////////////////////////////////////
	// working with the array
	


	// without the file
	double* gData = new double[73]; 
	gData[0] = 8; // -- число объектов в файле



	gData[1] = 1; // 1 -- тип "квадрат" со сторонами, параллельными осям координат
	gData[2] = 8; // 8 -- количество чисел double
	gData[3] = 1.0; gData[4] = 1.0;
	gData[5] = 1.0; gData[6] = 5.0;
	gData[7] = 5.0; gData[8] = 5.0;
	gData[9] = 5.0; gData[10] = 1.0;


	gData[11] = 2; // 2 -- тип "окружность"
	gData[12] = 3; // 3 -- количество чисел
	gData[13] = 0.0; gData[14] = 0.0; // координаты - центр окружности
	gData[15] = 25.0; // радиус окружности


	gData[16] = 3; // 3 -- пока неизвестный тип
	gData[17] = 5; // 5 -- количество чисел
	gData[18] = 1.; gData[19] = 2.; gData[20] = 3.; gData[21] = 4.; gData[22] = 5.; // данные



	gData[23] = 4; // 4 - дуга окружности
	gData[24] = 7; // 7 -- количество чисел
	gData[25] = 150.; gData[26] = 150.;  // координаты -- центр окружности
	gData[27] = 44.;         // радиус окружности
	gData[28] = PI / 4.; gData[29] = PI / 2.; // уголы -- начало и конец дуги
	gData[30] = 150.0; gData[31] = 194.0; // точка


	gData[32] = 2; // 2 -- тип "окружность"
	gData[33] = 3;
	gData[34] = 10.0; gData[35] = THROW_ERR; // при чтении объекта выбросится исключение
	gData[36] = 25.0;



	gData[37] = 5; // 5 -- тип многоугольник
	gData[38] = 10;
	gData[39] = 10.; gData[40] = 20.; // координаты точки
	gData[41] = 20.; gData[42] = 30.; // координаты точки
	gData[43] = 35.; gData[44] = 25.; // координаты точки
	gData[45] = 0.;  gData[46] = 50.; // координаты точки
	gData[47] = 0.; gData[48] = 0.;   // координаты точки



	gData[49] = 6; // -- тип "ломаная"
	gData[50] = 10;
	gData[51] = 0; gData[52] = 0; // координаты точки
	gData[53] = 1; gData[54] = 1; // координаты точки
	gData[55] = 0; gData[56] = 1; // координаты точки
	gData[57] = 0; gData[58] = 2; // координаты точки
	gData[59] = 5; gData[60] = 5; // координаты точки


	gData[61] = 28; // неизвестный объект
	gData[62] = 10;
	gData[63] = 90; gData[64] = 91; gData[65] = 92; gData[66] = 93; gData[67] = 94; gData[68] = 95; gData[69] = 96; gData[70] = 97; gData[71] = 98; gData[72] = 99;
	

	dataProvider->setArray(gData, 72);


	std::vector<Shape*> shapes;
	try {
		convertArrayToObj(dataProvider, shapes);
	}
	catch (const ReadError& e) {
		std::cout << e.what();
		// clear the data which has been written
		for (int i = 0; i < shapes.size(); ++i) {
			delete shapes[i];
		}
	}

	//////////////////////////////////////////////////////////////////////////////////

	DataProvider::DestroyInstance();
	delete rect1;
	delete rect2;
	delete circle1;
	delete circle2;
	delete arch1;
	delete arch2;
	delete arch3;
	delete arch4;
	delete polygon1;
	delete polygon2;
	delete polyline1;
	delete polyline2;
	delete unknown1;
	delete unknown2;

	return 0;
}
