#pragma once

#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <iostream>

static inline void allocMem(double**& p, const unsigned int rows, const unsigned int cols)
{
	p = new double*[rows];
	for (unsigned int i = 0; i < rows; ++i)
	{
		p[i] = new double[cols] {0};
	}
}

static inline void freeMem(double**& p, const unsigned int rows)
{
	for (int i = 0; i < rows; ++i)
	{
		delete[] p[i];
		p[i] = nullptr;
	}
	delete[] p;
	p = nullptr;
}

class Matrix
{
	public:
	static Matrix getIdentityMatrix(const int rows, const int cols);
	Matrix(const int rows, const int cols, const double value = 0);
	Matrix(const double** data, const int rows, const int cols);
	Matrix(const std::vector<std::vector<double>> mat);
	Matrix(const std::vector<double> data, bool isSingleColumn);
	~Matrix();
	Matrix(const Matrix& m);

	Matrix& operator=(const Matrix& m);
	Matrix& operator+=(const Matrix& m);
	Matrix& operator-=(const Matrix& m);
	Matrix& operator*=(const Matrix& m);
	Matrix& operator*=(const double& m);	// !!!
	Matrix& operator/=(const double& m);	// !!!

	std::string toString(const std::string& delim) const;

	public:		/// !!!
	int rows_ = -1, cols_ = -1;
	double **p = nullptr;

	void allocSpace();
	void freeSpace();
};

Matrix operator+(const Matrix& m1, const Matrix& m2);

Matrix operator-(const Matrix& m1, const Matrix& m2);

Matrix operator*(const Matrix& m1, const Matrix& m2);

Matrix operator*(const Matrix& m, const double& num);

Matrix operator*(const double& num, const Matrix& m);

Matrix operator/(const Matrix& m, const double& num);


#endif