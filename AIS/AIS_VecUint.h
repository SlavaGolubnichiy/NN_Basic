#pragma once
#ifndef AIS_VECUINT_H_
#define AIS_VECUINT_H_

#if 0

#include <string>

#include "AISBase.h"
#include "RandomXorsh.h"

using Solution = std::vector<unsigned int>;
using Fitness = double;

class AIS_VecUint : public AISBase<Solution, Fitness>
{
	private:
	const Solution answer = Solution{ 1,2,3,4,5,6,7,8,9,0};
	RandomXorsh rand;

	unsigned int getRandomChromo()
	{
		return (rand.generate32() % 100);
	}

	cSolution generateRandomSol() override
	{
		// implement generation of random solution
		Solution random(answer.size());
		for (unsigned int i = 0; i < random.size(); i++)
		{
			random[i] = getRandomChromo();
		}
		return random;
	}

	cFitness fitness(const cSolution& individual) override
	{
		// Evaluate the fitness of the individual
		if (individual.size() != answer.size())
		{
			throw std::invalid_argument("Individual vector must be of the same size as answer vector");
		}

		double sum = 0.0;
		for (size_t i = 0; i < answer.size(); i++)
		{
			// inversion of sum of the distances between corresponding values
			sum -= std::abs(static_cast<double>(answer[i]) - static_cast<double>(individual[i]));
		}
		return sum;
	}

	void crossover(const cSolution& p1, const cSolution& p2, cSolution& child1, cSolution& child2) override
	{
		if (p1.size() != p2.size())
		{
			throw std::invalid_argument("parent vectors must be of the same size");
		}
		child1.clear();
		child2.clear();
		child1.resize(p1.size());
		child2.resize(p1.size());

		// implement crossover of p1 and p2 into child1, child2
		unsigned int crossIndex = rand.generate32() % p1.size();
		unsigned int i = 0;
		for (; i < crossIndex; i++)
		{
			child1[i] = p1[i];
			child2[i] = p2[i];
		}
		for (; i < p1.size(); i++)
		{
			child1[i] = p2[i];	// pN visa-versa
			child2[i] = p1[i];	// pN visa-versa
		}
	}

	void mutate(cSolution& sol) override
	{
		// implement mutation of sol
		unsigned int mutationIndex = rand.generate32() % sol.size();
		sol[mutationIndex] = getRandomChromo();
	}

	public:
	const Solution getAnswer() const
	{
		return answer;
	}

};



std::string toString(Solution sol)
{
	std::string res = "[";
	for (unsigned int i = 0; i < sol.size(); i++)
	{
		res += std::to_string(sol[i]) + " ";
	}
	res += "]";
	return res;
}

void test_AISClass()
{
	AIS_VecUint ais = AIS_VecUint();

	for (unsigned int i = 1; i < 10; i++)
	{
		AIS_VecUint::cSolution sol = ais.getBestEvolved(i * 10, i * 100);
		printf("closest to answer=%s is (populSize=%u, genNum=%u): %s \n", toString(ais.getAnswer()).c_str(), i * 10, i * 100, toString(sol).c_str());
	}
}

#endif

#endif	// AIS_VECUINT_H_
