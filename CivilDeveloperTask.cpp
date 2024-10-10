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
		size_t numberOfObj = 0;
		try {
			numberOfObj = dataProvider->rdInt();
		}
		catch (const ReadError& e) {
			std::cout << e.what();
			return; // We don't know the number of objects because of the error
		}
		std::cout << "There are " << numberOfObj << " objects." << std::endl;

		while (1) { // It will stop  on the end of the array (the exception will be caught)

			int type = INT_MAX; // default in case of an error
			try {
				type = dataProvider->rdInt();
			}
			catch (const ReadError& e) {
				std::cout << e.what();
				// it will be default in case of an error (unknown type will be returned) 
			}
			int n = 0;
			try {
				n = dataProvider->rdInt();
			}
			catch (const ReadError& e) {
				std::cout << e.what();

				// We don't know how many objects to count
				// so return what we have already created (some objects before this one may be in the "shapes")
				return;  
			}

			// read n doubles (for the object)
			int i = 0;
			std::vector<double> nums;
			while (i < n) {

				double number = DBL_MAX; // default in case of an error
				try {
					number = dataProvider->rdDouble();
				}
				catch (const ReadError& e) {
					std::cout << e.what();
					// we cannot create an object with the error 
					// so create an unknown type
					type = INT_MAX;
				}

				nums.push_back(number);
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

double* initArray() {

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
	gData[25] = 0.; gData[26] = 0.;  // координаты -- центр окружности
	gData[27] = 44.;         // радиус окружности
	gData[28] = PI / 4.; gData[29] = PI / 2.; // уголы -- начало и конец дуги
	gData[30] = 44.0 * cos(3*PI / 2); gData[31] = 44.0 * sin(3*PI / 2); // точка

	
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
	gData[51] = 10; gData[52] = 0; // координаты точки
	gData[53] = 16; gData[54] = 12; // координаты точки
	gData[55] = 20; gData[56] = 21; // координаты точки
	gData[57] = 25; gData[58] = 23; // координаты точки
	gData[59] = 30; gData[60] = 35; // координаты точки


	gData[61] = 28; // неизвестный объект
	gData[62] = 10;
	gData[63] = 90; gData[64] = 91; gData[65] = 92; gData[66] = 93; gData[67] = 94; gData[68] = 95; gData[69] = 96; gData[70] = 97; gData[71] = 98; gData[72] = 99;

	return gData;
}

void test(const std::vector<Shape*>& objects) {

	Shape* rect1 = objects[0];
	Shape* rect2 = objects[1];
	Shape* circle1 = objects[2];
	Shape* circle2 = objects[3];
	Shape* arch1 = objects[4];
	Shape* arch2 = objects[5];
	Shape* arch3 = objects[6];
	Shape* arch4 = objects[7];
	Shape* polygon1 = objects[8];
	Shape* polygon2 = objects[9];
	Shape* polyline1 = objects[10];
	Shape* polyline2 = objects[11];
	Shape* unknown1 = objects[12];
	Shape* unknown2 = objects[13];

	// test objs
	std::vector<Point2d> expectedRes = { Point2d(1.0, 1.0), Point2d(5.0, 5.0), Point2d(1.0, 5.0), Point2d(5.0, 1.0) };
	std::vector<Point2d> res = rect1->boundingBox();
	if (res != expectedRes)
		printf("FAIL 1\n");
	std::vector<Point2d> expectedResCircle = { Point2d(5.0, 5.0), Point2d(15.0, 15.0), Point2d(5.0, 15.0), Point2d(15.0, 5.0) };
	std::vector<Point2d> resCircle = circle1->boundingBox();

	if (resCircle != expectedResCircle)
		printf("FAIL 2\n");
	std::vector<Point2d> expectedResPolyline = { Point2d(15.0, 10.0), Point2d(30.0, 30.0), Point2d(15.0, 30.0), Point2d(30.0, 10.0) };
	std::vector<Point2d> resPolyline = polyline1->boundingBox();

	if (resPolyline != expectedResPolyline)
		printf("FAIL 3\n");
	std::vector<Point2d> expectedResPolygon = { Point2d(0.0, 0.0), Point2d(35.0, 50.0), Point2d(0.0, 50.0), Point2d(35.0, 0) };
	std::vector<Point2d> resPolygon = polygon1->boundingBox();
	if (resPolygon != expectedResPolygon)
		printf("FAIL 4\n");
	std::vector<Point2d> expectedResArch = { Point2d(15.0, 20.0), Point2d(25.0, 20.0), Point2d(15.0, 25.0), Point2d(25.0, 25.0) };
	std::vector<Point2d> resArch = arch1->boundingBox();
	if (resArch != expectedResArch)
		printf("FAIL 5\n");

	std::vector<Point2d> expectedResArch3 = { Point2d(20.0, 20.0), Point2d(30.0, 20.0), Point2d(20.0, 30.0), Point2d(30.0, 30.0) };
	std::vector<Point2d> resArch3 = arch3->boundingBox();
	if (resArch3 != expectedResArch3)
		printf("FAIL 6\n");

	std::vector<Point2d> expectedResArch4 = { Point2d(5.0, 5.0), Point2d(35.0, 5.0), Point2d(5.0, 35.0), Point2d(35.0, 35.0) };
	std::vector<Point2d> resArch4 = arch4->boundingBox();
	if (resArch4 != expectedResArch4)
		printf("FAIL 7\n");


	// perimeter
	double expectedPerRect = 16;
	double resPerRec = rect1->perimeter();
	if (resPerRec != expectedPerRect)
		printf("FAIL 8\n");
	double expectedPerCircle = 10 * PI;
	double resPerCircle = circle1->perimeter();
	if (resPerCircle != expectedPerCircle)
		printf("FAIL 9\n");
	double expectedPerPolyline = 46.086;
	double resPerPolyline = std::round(polyline1->perimeter() * 10 * 10 * 10) / (10 * 10 * 10); // round the result
	if (resPerPolyline != expectedPerPolyline)
		printf("FAIL 10\n");
	double expectedPerPolygon = 145.326;
	double resPerPolygon = std::round(polygon1->perimeter() * 10 * 10 * 10) / (10 * 10 * 10); // round the result
	if (resPerPolygon != expectedPerPolygon)
		printf("FAIL 11\n");
	double expectedPerArch = PI * 5.0; // 180 degree so it will work
	double resPerArch = arch1->perimeter();
	if (resPerArch != expectedPerArch)
		printf("FAIL 12\n");
	double expectedPerArch3 = PI * 5;
	double resPerArch3 = arch3->perimeter();
	if (resPerArch3 != expectedPerArch3)
		printf("FAIL 13\n");
	double expectedPerArch4 = std::round((2 * PI * 15.0 - PI * 7.5) * 10 * 10 * 10) / (10 * 10 * 10);
	double resPerArch4 = std::round(arch4->perimeter() * 10 * 10 * 10) / (10 * 10 * 10);
	if (resPerArch4 != expectedPerArch4)
		printf("FAIL 14\n");
}

bool fileWriteRead(DataProvider* dataProvider, const std::vector<Shape*>& objects, int num) {
	// write them to file
	std::ofstream file("file.txt");

	file << num << '\n'; // Number of objects
	file.close();
	
	for (const auto& obj : objects) {
		dataProvider->writeToFile(obj, "file.txt");
	}

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

	return read;
}
void printShapes(WDraw& drawer, const std::vector<Shape*>& shapes) {
	// print
	for (int i = 0; i < shapes.size(); ++i) {
		drawer.drawObject(shapes[i]);
	}
}
void drawGraphics(WDraw& drawer, const std::vector<Shape*>& shapes) {
	// graphics
	drawer.initGraph(shapes);
}
void menu() {
	system("cls");
	std::cout << "Test the objects (1)" << std::endl;
	std::cout << "Write to file and read from it (2)" << std::endl;
	std::cout << "Print the objects (3)" << std::endl;
	std::cout << "Draw the objects (4)" << std::endl;
	std::cout << "Set another array (5)" << std::endl;
	std::cout << "Exit (6)" << std::endl;
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
		Point2d p1(10.0, 10.0);
		((Circle*)circle1)->set(p1, 5.0);
		circle2->set(p1, 5.0);
	}
	{ // Arch
		Point2d p1(20., 20.);
		((Arch*)arch1)->set(p1,5.0);
		((Arch*)arch1)->setAngles(PI / 2, PI / 2);
		((Arch*)arch1)->setPoint(Point2d(20.0, 25.0));
		(arch2)->set(p1, 5.0);
		(arch2)->setAngles(PI / 2, PI / 2 );
		(arch2)->setPoint(Point2d(20.0, 25.0));
		(arch3)->set(p1, 10.0);
		(arch3)->setAngles(0.0, PI / 2);
		(arch3)->setPoint(Point2d(10.0 * cos(PI / 4) + 20.0, 10.0 * sin(PI / 4) + 20.0)); // 45 degree
		(arch4)->set(p1, 15.0);
		(arch4)->setAngles(0.0, PI / 2); // bigger part
		(arch4)->setPoint(Point2d(15.0 * cos(3*PI / 2) + 20.0, 15.0 * sin(3*PI / 2) + 20.0)); // 3*PI/2

	}
	{ // Polyline
		Point2d p1(20.0, 20.0);
		Point2d p2(21.0, 23.0);
		Point2d p3(15.0, 10.0);
		Point2d p4(30.0, 30.0);
		Point2d p5(27.0, 28.0);
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
	std::vector<Shape*> objects;
	objects.push_back(rect1);
	objects.push_back(rect2);
	objects.push_back(circle1);
	objects.push_back(circle2);
	objects.push_back(arch1);
	objects.push_back(arch2);
	objects.push_back(arch3);
	objects.push_back(arch4);
	objects.push_back(polygon1);
	objects.push_back(polygon2);
	objects.push_back(polyline1);
	objects.push_back(polyline2);
	objects.push_back(unknown1);
	objects.push_back(unknown2);

	
	double* gData = nullptr;


	WDraw drawer = WDraw();

	size_t size = 139;
	// Create singleton
	DataProvider* dataProvider = DataProvider::GetInstance(size);
	bool read = false;
	std::vector<Shape*> shapes;
	bool exit = false;
	while (1) {

		if (exit) {
			break;
		}

		menu();
		int answer = -1;
		std::cin >> answer;
		switch (answer) {
		case 1:
			test(objects);
			break;
		case 2: { // write to file and read
			if (dataProvider->get_size() != 139) { // reset the size if the array has been changed before 
				dataProvider->DestroyInstance(); // destroying (not using setArray) because we don't have an array at this point 
				dataProvider = nullptr;          // we will get it from file
				dataProvider = DataProvider::GetInstance(139); 

				for (int i = 0; i < shapes.size(); ++i) {
					delete shapes[i];
				}
				shapes.clear();
			}
			read = fileWriteRead(dataProvider, objects, 14);

			if (read) { // only if data is in the array
					// reset the position if it has been changed
					if (dataProvider->getC() != 0) {
						dataProvider->setC(0);
					}
					convertArrayToObj(dataProvider, shapes);
			}
			break;
		}
		case 3: { // print
			printShapes(drawer, shapes);
		}
			  break;
		case 4: { // draw
			drawGraphics(drawer, shapes);
		}
			  break;
		case 5: { // set another array
			// Clean up
			for (int i = 0; i < shapes.size(); ++i) {
				delete shapes[i];
			}
			shapes.clear();

			if (gData == nullptr) {
				gData = initArray();
			}
			dataProvider->setArray(gData, 73);

			gData = nullptr; // Reset the pointer (the array is owned by dataProvider now)
			convertArrayToObj(dataProvider, shapes);
		}
			break;
		case 6:
			exit = true;
			break;
		default:
			std::cout << "Wrong answer!" << std::endl;
		}
		if (!exit) { // skip when "exit"
			char c;
			std::cout << "Press any key to continue..." << std::endl;
			std::cin >> c;
		}
	}
	// Clean up
	for (int i = 0; i < shapes.size(); ++i) {
		delete shapes[i];
	}




	DataProvider::DestroyInstance();
	
	if (gData != nullptr) { // if something went wrong
		delete[] gData;
		gData = nullptr;
	}

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
