#pragma once

#ifndef __FLOAT64_H__
#define __FLOAT64_H__



#include <cmath>
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

	static double normalize(double src, const double src_range_min, const double src_range_size)
	{
		if (src_range_size > 0)
		{
			return 0.0 + (src - src_range_min) / src_range_size;
		}
		else
		{
			throw std::exception("src_range_size must be greater than 0.");
		}
	}

	/// define methods with "static" !!! without it, you may have "multiple definitions of float64::XXX()" error
};

#endif
