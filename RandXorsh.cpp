#include <cstdint>
#include <chrono>

#include "RandXorsh.h"
#include "vecop.h"

/// note !
// .h - public section
// .cpp::unnamednamespace	- private section, not accessible from .h
// .cpp						- private section, accessible from .h

// private: (unnamed namespace in .cpp = private section, visibility = this file only)
namespace
{
	static inline uint64_t getNsFromEpoch()
	{
		uint64_t ns =
			std::chrono::duration_cast<std::chrono::nanoseconds>
			(std::chrono::system_clock::now().time_since_epoch()).count();
		return ns;
	}

	static uint64_t state = getNsFromEpoch(); // careful: same initial values produce same sequences

}

// public: (definitions)

uint32_t xorsh::generate32()	// xorsh32
{
	// state must be initialized with non-zero value
	/* Algorithm "xor" from p. 4 of Marsaglia, "Xorshift RNGs" */
	uint32_t x = (uint32_t)state;
	x ^= x << 13;
	x ^= x >> 17;
	x ^= x << 5;
	return state = x;
}

uint64_t xorsh::generate64()
{
	uint64_t x = state;
	x ^= x << 13;
	x ^= x >> 7;
	x ^= x << 17;
	return state = x;
}

double xorsh::generateDouble()
{
	uint64_t integer = generate64();
	double dfloat = (double)integer;
	return dfloat;
	/// dfloat = dfloat / (max - min) + min;		/// min ? max ???
}

double xorsh::generateDoubleRange01()
{
	uint32_t integer = generate32();
	double dfloat = (double)integer / (double)UINT32_MAX;
	return dfloat;
}

// TESTS

void xorsh::test_generate32(const unsigned int generations)
{
	using testType = uint32_t;
	std::vector<testType> data;
	testType val = 0;
	for (uint32_t i = 0; i < generations; i++)
	{
		val = xorsh::generate32();
		if (vecop::isUniqueValue<testType>(val, data))
		{
			data.push_back(val);
			printf("prn #%u = %u \n", i, val);
		}
		else
		{
			throw std::logic_error("value is not unique");
		}
	}
}
