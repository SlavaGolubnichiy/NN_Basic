#pragma once

#ifndef _MATOP_
#define _MATOP_

#include <float.h>
#include <cmath>

#include "Matrix.h"
#include "float64.h"


namespace matop
{
	static const double e = std::exp(1.0);
	static const double pi = 3.141592653589793238463;

	// parallelable
	bool isPositive(const Matrix& mat)
	{
		for (unsigned int i = 0; i < mat.rows(); i++)
		{
			for (unsigned int j = 0; i < mat.cols(); i++)
			{
				if (mat.get(i, j) < 0)
				{
					return false;
				}
			}
		}
		return true;
	}

	double min(const Matrix& mat)
	{
		double min = DBL_MAX;
		double buf;
		for (unsigned int i = 0; i < mat.rows(); i++)
		{
			for (unsigned int j = 0; j < mat.cols(); j++)
			{
				buf = mat.get(i, j);
				if (buf < min)
				{
					min = buf;
				}
			}
		}
		return min;
	}

	double max(const Matrix& mat)
	{
		double max = DBL_MIN;
		double buf;
		for (unsigned int i = 0; i < mat.rows(); i++)
		{
			for (unsigned int j = 0; j < mat.cols(); j++)
			{
				buf = mat.get(i, j);
				if (buf > max)
				{
					max = buf;
				}
			}
		}
		return max;
	}

	void normalize(Matrix& mat)
	{
		double srcRangeMin = 0; //min(mat);	// rangeMin = min(mat) distorts proportions between mat's values
		double srcRangeMax = max(mat);
		for (unsigned int i = 0; i < mat.rows(); i++)
		{
			for (unsigned int j = 0; j < mat.cols(); j++)
			{
				mat.set(i, j, float64::normalize(mat.get(i, j), srcRangeMin, srcRangeMax - srcRangeMin));
			}
		}
	}

	/// <summary>
	/// Returns value in range (-1; +1) : when x->(-inf) => y->(-1); when x->(+inf) => y->(+1)
	/// </summary>
	void tanh(Matrix& mat)
	{
		for (unsigned int i = 0; i < mat.rows(); i++)
		{
			for (unsigned int j = 0; j < mat.cols(); j++)
			{
				mat.set(i, j, std::tanh(mat.get(i, j)));
			}
		}
	}

	/// <summary>
	/// Returns value in range (-1; +1) : when x->(-inf) => y->(-1); when x->(+inf) => y->(+1)
	/// Steepness defines how steep the transition from y=-1 to y=+1, based on x value, is.
	/// </summary>
	double tanhCustom(const double x, const double steepness)
	{
		double p = steepness * x;
		return
			(pow(e, p) - pow(e, -p)) /
			(pow(e, p) + pow(e, -p));
	}

	/// <summary>
	/// Returns value in range (-1; +1) : when x->(-inf) => y->(-1); when x->(+inf) => y->(+1)
	/// Steepness defines how steep the transition from y=-1 to y=+1, based on x value, is.
	/// </summary>
	void tanhCustom(Matrix& mat, double steepness)
	{
		for (unsigned int i = 0; i < mat.rows(); i++)
		{
			for (unsigned int j = 0; j < mat.cols(); j++)
			{
				mat.set(i, j, tanhCustom(mat.get(i, j), steepness));
			}
		}
	}




};



#endif
