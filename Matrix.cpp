#include <stdexcept>
#include <string>

#include "Matrix.h"



/* PUBLIC MEMBER FUNCTIONS
 ********************************/

Matrix::Matrix(const int& rows, const int& cols)
	:
	rows_(rows),
	cols_(cols)
{
	allocSpace();
	for (int i = 0; i < rows_; ++i)
	{
		for (int j = 0; j < cols_; ++j)
		{
			p[i][j] = 0;
		}
	}
}

Matrix::Matrix(const uint8_t** a, const int& rows, const int& cols)
	:
	rows_(rows),
	cols_(cols)
{
	allocSpace();
	for (int i = 0; i < rows_; ++i)
	{
		for (int j = 0; j < cols_; ++j)
		{
			p[i][j] = a[i][j];
		}
	}
}

Matrix::~Matrix()
{
	freeSpace();
}

Matrix::Matrix(const Matrix& m)
	:
	rows_(m.rows_),
	cols_(m.cols_)
{
	allocSpace();
	for (int i = 0; i < rows_; ++i)
	{
		for (int j = 0; j < cols_; ++j)
		{
			p[i][j] = m.p[i][j];
		}
	}
}

Matrix& Matrix::operator=(const Matrix& m)
{
	if (this == &m)
	{
		return *this;
	}

	if ((rows_ != m.rows_) || (cols_ != m.cols_))
	{
		freeSpace();
		rows_ = m.rows_;
		cols_ = m.cols_;
		allocSpace();
	}

	for (int i = 0; i < rows_; ++i)
	{
		for (int j = 0; j < cols_; ++j)
		{
			p[i][j] = m.p[i][j];
		}
	}
	return *this;
}

Matrix& Matrix::operator+=(const Matrix& m)
{
	for (int i = 0; i < rows_; ++i)
	{
		for (int j = 0; j < cols_; ++j)
		{
			p[i][j] += m.p[i][j];
		}
	}
	return *this;
}

Matrix& Matrix::operator-=(const Matrix& m)
{
	for (int i = 0; i < rows_; ++i)
	{
		for (int j = 0; j < cols_; ++j)
		{
			p[i][j] -= m.p[i][j];
		}
	}
	return *this;
}

Matrix& Matrix::operator*=(const Matrix& m)
{
	Matrix temp(rows_, m.cols_);
	for (int i = 0; i < temp.rows_; ++i)
	{
		for (int j = 0; j < temp.cols_; ++j)
		{
			for (int k = 0; k < cols_; ++k)
			{
				temp.p[i][j] += (p[i][k] * m.p[k][j]);
			}
		}
	}
	return (*this = temp);
}

Matrix& Matrix::operator*=(const double& num)
{
	for (int i = 0; i < rows_; ++i)
	{
		for (int j = 0; j < cols_; ++j)
		{
			p[i][j] *= num;
		}
	}
	return *this;
}

Matrix& Matrix::operator/=(const double& num)
{
	for (int i = 0; i < rows_; ++i)
	{
		for (int j = 0; j < cols_; ++j)
		{
			p[i][j] /= num;
		}
	}
	return *this;
}

std::string Matrix::toString(const std::string& delim) const
{
	std::string s = "";
	for (int i = 0; i < rows_; ++i)
	{
		s += std::to_string(p[i][0]);
		for (int j = 1; j < cols_; ++j)
		{
			s += delim + std::to_string(p[i][j]);
		}
		s += "\n";
	}
	return s;
}



/* PRIVATE HELPER FUNCTIONS
 ********************************/

void Matrix::allocSpace()
{
	p = new unsigned int*[rows_];
	for (unsigned int i = 0; i < rows_; ++i)
	{
		p[i] = new unsigned int[cols_];
	}
}

void Matrix::freeSpace()
{
	for (int i = 0; i < rows_; ++i)
	{
		delete[] p[i];
		p[i] = nullptr;
	}
	delete[] p;
	p = nullptr;
}



/* NON-MEMBER FUNCTIONS
 ********************************/

Matrix operator+(const Matrix& m1, const Matrix& m2)
{
	Matrix temp(m1);
	return (temp += m2);
}

Matrix operator-(const Matrix& m1, const Matrix& m2)
{
	Matrix temp(m1);
	return (temp -= m2);
}

Matrix operator*(const Matrix& m1, const Matrix& m2)
{
	Matrix temp(m1);
	return (temp *= m2);
}

Matrix operator*(const Matrix& m, const double& num)
{
	Matrix temp(m);
	return (temp *= num);
}

Matrix operator*(const double& num, const Matrix& m)
{
	Matrix temp(m);
	return (temp *= num);
}

Matrix operator/(const Matrix& m, const double& num)
{
	Matrix temp(m);
	return (temp /= num);
}
