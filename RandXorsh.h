// XORSHF pseudo-random number generator v_2023.03.06_17-24

// Implemented by SlavaGolubnichiy
//
// Very fast subtype of LFSR generators, discovered by G.Marsaglia.
//
// How to use:
//	1) Include RandXorsh.h file into your project:
//		#include "RandXorsh.h"
//	2) Use it:
//		uint32_t num = xorsh::generate32();
//
// Improved versions:
//	1.
//		XORWOW (2003)
//		Marsaglia proposed XORWOW generator
//		(the output of the xorshift generator is summed with a Weyl sequence).
//		XORWOW - default generator in the nVidia CUDA API CURAND library for GPUs.
//	2.
//		Xoroshiro128+ (2018)
//		Modification of Marsaglia's Xorshift generator. Among the fastest generators for 64-bit CPUs.
//		Related: xoroshiro128**, xoshiro256+ and xoshiro256***.
//
// State parameter and parameters in generate() (.ccp file) have to be selected carefully
// in order to provide efficient period.
//

#pragma once
#ifndef RAND_XORSH_H_
#define RAND_XORSH_H_

#include <cstdint>

// named namespace = public section
namespace xorsh
{
	uint32_t generate32();
	//uint32_t generate32(uint32_t min, uint32_t max);	// use xorsh32
	uint64_t generate64();
	//uint64_t generate64(uint64_t min, uint64_t max);	// xorsh64
	double generateDouble();
	double generateDoubleRange01();

	void test_generate32(const unsigned int genUniqueNums);
}

#endif	// RAND_XORSH_H_
