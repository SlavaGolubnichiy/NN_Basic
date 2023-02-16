#pragma once

#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <iostream>

class Matrix
{
	public:
	Matrix(const int& rows, const int& cols);
	Matrix(const uint8_t** a, const int& rows, const int& cols);
	~Matrix();
	Matrix(const Matrix& m);

	Matrix& operator=(const Matrix& m);
	Matrix& operator+=(const Matrix& m);
	Matrix& operator-=(const Matrix& m);
	Matrix& operator*=(const Matrix& m);
	Matrix& operator*=(const double& m);	// !!!
	Matrix& operator/=(const double& m);	// !!!

	std::string toString(const std::string& delim) const;

	public:		// !!!
	int rows_, cols_;
	unsigned int **p;

	void allocSpace();
	void freeSpace();
};

#endif
