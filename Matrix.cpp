#include <string>
#include <vector>

#include "Matrix.h"
#include "float64.h"


/* PUBLIC MEMBER FUNCTIONS
 ********************************/

void Matrix::exceptIfDenyDimensions(const int rows, const int cols)
{
	if (rows <= 0 || cols <= 0)
	{
		throw std::invalid_argument("rows and/or cols must be greater than zero");
	}
}

void Matrix::exceptIfDenyIndexes(const int i, const int j) const
{
	if (i < 0 || rows_ < i)
	{
		throw std::invalid_argument("row(i) must be in range [0; rows_]");
	}
	if (j < 0 || cols_ < j)
	{
		throw std::invalid_argument("column(j) must be in range [0; cols_]");
	}
}

void Matrix::exceptIfDenyAddSub(const Matrix& m1, const Matrix& m2)
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
	allocMem(this->p, rows_, cols_);
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
	allocMem(this->p, rows_, cols_);
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

	allocMem(this->p, rows_, cols_);
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
		allocMem(this->p, rows_, cols_);
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
		allocMem(this->p, rows_, cols_);
		for (int i = 0; i < dataSize; ++i)
		{
			p[0][i] = data[i];
		}
	}
}

Matrix::~Matrix()
{
	freeMem(this->p, this->rows_);
}

Matrix::Matrix(const Matrix& m)
	:
	rows_(m.rows_),
	cols_(m.cols_)
{
	allocMem(this->p, rows_, cols_);
	for (int i = 0; i < rows_; ++i)
	{
		for (int j = 0; j < cols_; ++j)
		{
			p[i][j] = m.p[i][j];
		}
	}
}



double Matrix::get(const unsigned int i, const unsigned int j) const
{
	exceptIfDenyIndexes(i, j);
	return p[i][j];
}

void Matrix::set(const unsigned int i, const unsigned int j, const double value)
{
	exceptIfDenyIndexes(i, j);
	p[i][j] = value;
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
		freeMem(this->p, this->rows_);
		rows_ = m.rows_;
		cols_ = m.cols_;
		allocMem(this->p, rows_, cols_);
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

#include <sstream>	// for toString(double) with precision
#include <iomanip>	// for toString(double) with precision
std::string Matrix::toString(const std::string& delim, const unsigned int precision) const
{
	std::string s = "";
	for (int i = 0; i < rows_; ++i)
	{
		s += float64::toString(p[i][0], precision);
		for (int j = 1; j < cols_; ++j)
		{
			s += delim + float64::toString(p[i][j], precision);
		}
		s += "\n";
	}
	return s;

	/// variant 2 (might have better performance)
	/*
	std::ostringstream res;
	res << std::fixed << std::setprecision(precision);
	for (int i = 0; i < rows_; ++i)
	{
		res << p[i][0];
		for (int j = 1; j < cols_; ++j)
		{
			res << delim << p[i][j];
		}
		res << "\n";
	}
	return res.str();
	*/

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
