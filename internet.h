#pragma once

#include <vector>
#include "Exceptions.h"

namespace InternetRW
{
	std::vector<double> data = {
    8, // -- ����� �������� � �����



    1, // 1 -- ��� "�������" �� ���������, ������������� ���� ���������
    8, // 8 -- ���������� ����� double
    1.0, 1.0,
    1.0, 5.0,
    5.0, 5.0,
    5.0, 1.0,



    2, // 2 -- ��� "����������"
    3, // 3 -- ���������� �����
    0.0, 0.0, // ���������� - ����� ����������
    25.0, // ������ ����������



    3, // 3 -- ���� ����������� ���
    5, // 4 -- ���������� �����
    1., 2., 3., 4., 5., // ������



    4, // 4 - ���� ����������
    7, // 7 -- ���������� �����
    0., 0.,  // ���������� -- ����� ����������
    44.,         // ������ ����������
    PI / 4., PI / 2., // ����� -- ������ � ����� ����
    44.0 * cos(3 * PI / 2), 44.0 * sin(3 * PI / 2), // �����


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
	int c = 0;

	void checkC() {
		if (c == data.size()) {
			throw EndOfFile();
		}
	}

	int getInt() {
		checkC();
        double* dt = &data[0];
		int i = (int)dt[c++];
		return i;
	}
    
	double getDouble() {
		checkC();
        double* dt = &data[0]; // it's faster
		double d = dt[c++];
		return d;
	}
}