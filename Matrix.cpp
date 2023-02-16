#include <stdexcept>
#include <string>
#include <vector>

#include "Matrix.h"

static void exceptIfDenyDimensions(const int rows, const int cols)
{
	if (rows <= 0 || cols <= 0)
	{
		throw std::invalid_argument("rows and/or cols must be greater than zero");
	}
}

static void exceptIfDenyAddSub(const Matrix& m1, const Matrix& m2)
{
	if (m1.rows_ != m2.rows_)
	{
		throw std::invalid_argument("m1.rows must be equal to m2.rows");
	}
	if (m1.cols_ != m2.cols_)
	{
		throw std::invalid_argument("m1.cols must be equal to m2.cols");
	}
}



/* PUBLIC MEMBER FUNCTIONS
 ********************************/

// matrix like:
// 1 0 0
// 0 1 0
// 0 0 1
Matrix Matrix::getIdentityMatrix(const int rows, const int cols)
{
	exceptIfDenyDimensions(rows, cols);
	double** src = nullptr;
	allocMem(src, rows, cols);
	for (int i = 0; i < rows; ++i)
	{
		for (int j = 0; j < cols; ++j)
		{
			if (i != j)
			{
				src[i][j] = 0;
			}
			else
			{
				src[i][j] = 1;
			}
		}
	}
	return Matrix((const double**)src, rows, cols);
}

Matrix::Matrix(const int rows, const int cols, const double value)
	:
	rows_(rows),
	cols_(cols)
{
	exceptIfDenyDimensions(rows, cols);
	allocSpace();
	for (int i = 0; i < rows_; ++i)
	{
		for (int j = 0; j < cols_; ++j)
		{
			p[i][j] = value;
		}
	}
}

Matrix::Matrix(const double** ptr, const int rows, const int cols)
	:
	rows_(rows),
	cols_(cols)
{
	exceptIfDenyDimensions(rows, cols);
	allocSpace();
	for (int i = 0; i < rows_; ++i)
	{
		for (int j = 0; j < cols_; ++j)
		{
			p[i][j] = ptr[i][j];
		}
	}
}

Matrix::Matrix(const std::vector<std::vector<double>> mat)
	:
	rows_(mat.size()),
	cols_(mat[0].size())
{
	exceptIfDenyDimensions(mat.size(), mat[0].size());
	unsigned int matCols = mat[0].size();
	for (unsigned int i = 0; i < mat.size(); i++)
	{
		if (mat[i].size() != matCols)
		{
			throw std::invalid_argument("all mat's rows must contain equal number of elements (rectangle matrix)");
		}
	}

	allocSpace();
	for (int i = 0; i < rows_; ++i)
	{
		for (int j = 0; j < cols_; ++j)
		{
			p[i][j] = mat[i][j];
		}
	}
}

// isSingleColumn: true [column-vector], false [row-vector]
Matrix::Matrix(const std::vector<double> data, bool isSingleColumn)
{
	unsigned int dataSize = data.size();
	if (!dataSize)
	{
		throw std::invalid_argument("data.size must not be 0");
	}
	
	// column-vector
	if (isSingleColumn)
	{
		rows_ = dataSize;
		cols_ = 1;
		exceptIfDenyDimensions(rows_, cols_);
		allocSpace();
		for (int i = 0; i < dataSize; ++i)
		{
			p[i][0] = data[i];
		}
	}
	// row-vector
	else
	{
		rows_ = 1;
		cols_ = dataSize;
		exceptIfDenyDimensions(rows_, cols_);
		allocSpace();
		for (int i = 0; i < dataSize; ++i)
		{
			p[0][i] = data[i];
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
	if (!this || !&m)
	{
		throw std::invalid_argument("this or &m is nullptr! Both of them must not");
	}
	
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
	exceptIfDenyAddSub(*this, m);
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
	exceptIfDenyAddSub(*this, m);
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
	//	[ 1 2 3 ]			[ 7 ]   [ (1*7+2*8+3*9) ]
	//	[ 4 5 6 ]		*	[ 8 ] = [ (4*7+5*8+6*9) ]
	//						[ 9 ] 
	//   2x3				 3x1		2x1
	//	  \ \______=________/ /
	//	   \_res(rows,cols)__/___________^

	if (this->cols_ != m.rows_)
	{
		throw std::invalid_argument("this.cols must be equal to m.rows");
	}
	Matrix res(rows_, m.cols_);
	
	for (int i = 0; i < res.rows_; ++i)
	{
		for (int j = 0; j < res.cols_; ++j)
		{
			for (int k = 0; k < cols_; ++k)
			{
				res.p[i][j] += (p[i][k] * m.p[k][j]);
			}
		}
	}
	return (*this = res);
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
	p = new double*[rows_];
	for (unsigned int i = 0; i < rows_; ++i)
	{
		p[i] = new double[cols_] {0};
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

Matrix operator*(const double& num, const Matrix& m)
{
	Matrix temp(m);
	return (temp *= num);
}

Matrix operator*(const Matrix& m, const double& num)
{
	Matrix temp(m);
	return (temp *= num);
}

Matrix operator/(const Matrix& m, const double& num)
{
	Matrix temp(m);
	return (temp /= num);
}
