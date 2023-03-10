// Vector operations
// Provides function for operating/analysing/etc. of std::vectors.

#pragma once
#ifndef VECOP_H_
#define VECOP_H_

#include <vector>

namespace vecop
{
	template<typename DataType>
	bool isUniqueValue(DataType value, std::vector<DataType> population)
	{
		for (uint64_t i = 0; i < population.size(); i++)
		{
			if (population[i] == value)
			{
				return false;
			}
		}
		return true;
	}
};

#endif	// VECOP_H_
