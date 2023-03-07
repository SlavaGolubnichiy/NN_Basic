#pragma once
#ifndef AIS_BASE_H_
#define AIS_BASE_H_

#if 0

#include <vector>
#include <algorithm>

// README!

// User Manual [how to use AIS when it is a class)]:
// 1. Create your task-specified AIS class which inherits from AISBase, specifying Solution and Fitness type of your implementation
// 2. (obligatory) Implement methods: generateRandomSol(); fitness(...); crossover(...); mutate(...);
// 3. (optionally) Override other methods with more efficient implementations.
// 4. When needed to solve the task, use following: Solution best = getBestEvolved(populSize, genNum);
	// populSize: bigger => higher memory usage, more children in one population, higher chance of getting a good solution
	// genNum: bigger => higher mem. usage, more iterations through select-crossover-mutate-delete cycle, higher quality of the best solution
// * default implementation of getBestEvolved() might be ver time-consuming!

// example
/*

#include "AISBase.h"

using Solution = std::vector<unsigned int>;
using Fitness = double;

class AIS_VecUint : public AISBase<Solution, Fitness>
{
	private:
	const Solution answer = ... ;

	cSolution generateRandomSol() override
	{
		// implement generation of random solution
	}

	cFitness fitness(const cSolution& individual) override
	{
		// Evaluate the fitness of the individual
	}

	void crossover(const cSolution& p1, const cSolution& p2, cSolution& child1, cSolution& child2) override
	{
		// implement crossover of p1 and p2 into child1, child2
	}

	void mutate(cSolution& sol) override
	{
		// implement mutation of sol
	}

};

void test_AISClass()
{
	AIS_VecUint ais = AIS_VecUint();
	for (unsigned int i = 1; i < 10; i++)
	{
		AIS_VecUint::cSolution sol = ais.getBestEvolved(i * 10, i * 100);
		printf("closest to answer=%s is (populSize=%u, genNum=%u): %s \n", toString(ais.getAnswer()).c_str(), i * 10, i * 100, toString(sol).c_str());
	}
}

*/

template <typename SolutionType, typename FitnessType>
class AISBase
{
	public:
	using cSolution = SolutionType;

	protected:
	
	using cFitness = FitnessType;
	struct cSolEntry { cSolution solution; cFitness fitness; };
	using cSolsEntriesList = std::vector<cSolEntry>;

	void sortByFitnessDesc(cSolsEntriesList& solutions)
	{
		std::sort
		(
			solutions.begin(),
			solutions.end(),
			[](const cSolEntry& a, const cSolEntry& b)	// lambda-function
			{
				return a.fitness > b.fitness;
			}
		);
	}

	virtual cSolution generateRandomSol() = 0;					/// custom, inherit & override
	virtual cFitness fitness(const cSolution& individual) = 0;	/// custom, inherit & override
	virtual void crossover(const cSolution& p1, const cSolution& p2, cSolution& child1, cSolution& child2) = 0;		/// custom, inherit & override
	virtual void mutate(cSolution& sol) = 0;					/// custom, inherit & override

	private:

	/// <summary>
	/// Returns true if list of solution entries is sorted by fitness in descending order, otherwise - returns false.
	/// Descending order means: greatest fitness value has index 0, lower - next.
	/// </summary>
	bool isSortedByFitnessDesc(const cSolsEntriesList& solutions)
	{
		for (unsigned int i = 0; i < solutions.size() - 1; ++i)
		{
			if (solutions[i].fitness < solutions[i + 1].fitness)
			{
				return false;
			}
		}
		return true;
	}

	// divide it into: void sortDesc(SolsEntriesList& list)
	// and	
	// getFirstNSolutions(SolsEntriesList& solsSrc, std::vector<Solution>& solsDest) ?
	/// <summary>
	/// Copy best bestNum number of Solutions from a SolsEntriesList to bestDest.
	/// Excepts if:
	///		population is empty;
	///		bestNum==0;
	///		bestNum > population_size;
	///		population is not sorted in descending order.
	/// </summary>
	/// <param name="population">
	/// Population. Must not be empty and must be sorted by fitness in descending order.
	/// </param>
	/// <param name="bestDest">
	/// Destination vector where the result is to be put.
	/// bestDest vector will be cleared and overwritten.
	/// </param>
	/// <param name="bestNum">
	/// Specifies how many of the best solutions to copy from population.
	/// </param>
	/// <returns>
	///	void
	/// </returns>
	void select(const cSolsEntriesList& population, std::vector<cSolution>& bestDest, const unsigned int bestNum)
	{
		if (population.empty())
		{
			throw std::invalid_argument("population must not be empty");
		}
		if (bestNum == 0)
		{
			throw std::invalid_argument("bestNum must be greater than 0");
		}
		if (bestNum > population.size())
		{
			throw std::logic_error("bestNum exceeds the size of population to select from");
		}
		if (!isSortedByFitnessDesc(population))
		{
			throw std::logic_error("population must be sorted in descending order");
		}

		bestDest.resize(bestNum);
		// copy bestNum elements from the begining of population to the begining of bestDest
		for (unsigned int i = 0; i < bestNum && i < population.size(); i++)
		{
			bestDest[i] = population[i].solution;
		}
	}

	void deleteLastN(cSolsEntriesList& sortedPopulation, const unsigned int N)
	{
		if (!N)
		{
			return;
		}
		if (N >= sortedPopulation.size())
		{
			throw std::exception("N (number of solutions to delete) is greater that size of the population");
		}
		else
		{
			// erase the last N elements of the vector
			sortedPopulation.erase(sortedPopulation.end() - N, sortedPopulation.end());
		}
	}

	public:
	// the genetic algorithm function.
	// If divided to initPopul(); and evolvePopul(); => population can be saved under the class object and solution can 
	// be evolved, then evolved more and more on genNum generations with each evolvePopul(unsigned int genNum); call (on-test learning ? :) )
	cSolution getBestEvolved(const unsigned int populSize, const unsigned int genNum)
	{
		// Initialize the population
		cSolsEntriesList population(populSize);
		cSolution randSol;
		for (unsigned int i = 0; i < populSize; i++)
		{
			randSol = generateRandomSol();
			population[i] = { randSol, fitness(randSol) };
		}

		// Sort population by fitness
		sortByFitnessDesc(population);

		// Evolution loop
		for (unsigned int gen = 0; gen < genNum; gen++)
		{
			// Select parents for crossover
			unsigned int parentsSize = populSize / 2;
			parentsSize = !(parentsSize % 2) ? parentsSize : (parentsSize - 1);	// isEven(x) { return !(x % 2) }
			if (parentsSize < 2)	// if result parentsSize is too small
			{
				throw std::exception("Error: Selection of parents became too small");
			}
			std::vector<cSolution> parents;
			select(population, parents, parentsSize);

			// Create children by crossed parents
			unsigned int childrenSize = parentsSize;	// even as well
			cSolsEntriesList children;
			cSolution ch1, ch2;
			for (unsigned int i = 0; i < childrenSize; i += 2)
			{
				crossover(parents[i], parents[i + 1], ch1, ch2);
				mutate(ch1);
				mutate(ch2);
				// add children to population
				population.push_back({ ch1, fitness(ch1) });	/// insertion boosts the performance
				population.push_back({ ch2, fitness(ch2) });	/// insert, conditional to child's max. index in population (fitness), cuts unnecessary, restricts mem. usage
			}

			// Sort population by fitness
			sortByFitnessDesc(population);

			// Delete worst solutions
			unsigned int delNum = 0;
			if (population.size() > populSize)
			{
				delNum = population.size() - populSize;
			}
			deleteLastN(population, delNum);
		}

		// Return the best individual
		if (population.empty())
		{
			throw std::runtime_error("population is empty, no solution found");
		}
		else
		{
			return population[0].solution;
		}

	}

};

#endif

#endif	// AIS_BASE_H_
