#pragma once

#ifndef __FLOAT64_H__
#define __FLOAT64_H__

#include <math.h>
#include <sstream>	// for toString(double) with precision
#include <iomanip>	// for toString(double) with precision

namespace float64
{
	static double floor(const double value, const unsigned int precision)
	{
		double k = pow(10, precision);
		return (std::floor(value * k) / k);	// res = floor(value*k) / k
	}

	static double round(const double value, const unsigned int precision)
	{
		double k = pow(10, precision);
		return std::floor(value * k + 0.5) / k;	// res = round(value*k) / 10^precision
	}

	static std::string toString(const double value, const unsigned int precision = 6)
	{
		std::ostringstream res;
		// or ? res << std::setprecision(precision) << std::fixed << value;
		// which's faster?
		res.precision(precision);
		res << std::fixed << value;
		return res.str();
	}

	double normalize(double srcRangeMin, double srcRangeMax, double destRangeMin, double destRangeMax);
};

#endif
