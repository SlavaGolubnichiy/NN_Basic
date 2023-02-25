#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <string>


#if 0

#define TEST_GENALG 1



/// todo:
// test non-custom functions
// test custom functions
// test AIS in a whole (unsigned int)
// make AIS as a Solution-and-Fitness-types-template class with custom functions as pure virtual methods
// use it as described in User Manual, test as a template



// implement:
// generateRandomSol();	-	generate single solution (used at init stage)
// fitness(Solution);	-	calculate fitness value of a solution (how good solution is)
// crossover(...);		-	generate children of two parents (aa + bb = [ab, ba])
// mutate(Sol);			-	mutate (make a small random change) in a Solution
/// other methods are already implemented and do not depend on Solution type !!!



using Solution			= std::vector<unsigned int>;
using Fitness			= double;
struct SolEntry			{ Solution solution; Fitness fitness; };
using SolsEntriesList	= std::vector<SolEntry>;





namespace genalg
{
	/// <summary>
	/// Returns true if list of solution entries is sorted by fitness in descending order, otherwise - returns false.
	/// Descending order means: greatest fitness value has index 0, lower - next.
	/// </summary>
	bool isSortedByFitnessDesc(const SolsEntriesList& solutions)
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
	
	/// your custom implementation
	Solution generateRandomSol()
	{
		// implement generation of random solution
		Solution res;
		return res;
	}
	
	/// your custom implementation
	Fitness fitness(const Solution& individual)
	{
		// Evaluate the fitness of the individual
		Fitness res = 0;
		return res;
	}
	
	void sortByFitnessDesc(SolsEntriesList& solutions)
	{
		std::sort
		(
			solutions.begin(),
			solutions.end(),
			[](const SolEntry& a, const SolEntry& b)	// lambda-function
			{
				return a.fitness > b.fitness;
			}
		);
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
	/// Any present elements will be overwritten.
	/// </param>
	/// <param name="bestNum">
	/// Specifies how many of the best solutions to copy from population.
	/// </param>
	/// <returns>
	///	void
	/// </returns>
	void select(const SolsEntriesList& population, std::vector<Solution>& bestDest, const unsigned int bestNum)
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
	
	/// your custom implementation
	void crossover(const Solution& p1, const Solution& p2, Solution& child1, Solution& child2)
	{
		// implement crossover of p1 and p2 into child1, child2
	}
	
	/// your custom implementation
	void mutate(Solution& sol)
	{
		// implement mutation of sol
	}
	
	void deleteLastN(SolsEntriesList& sortedPopulation, const unsigned int N)
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
	
	// the genetic algorithm function
	Solution getBestEvolved(const unsigned int populSize, const unsigned int genNum)
	{
		// Initialize the population
		SolsEntriesList population(populSize);
		Solution randSol;
		for (unsigned int i = 0; i < populSize; i++)
		{
			randSol = generateRandomSol();
			population[i] = { randSol, fitness(randSol)};
		}
	
		// Sort population by fitness
		sortByFitnessDesc(population);
	
		// Evolution loop
		for (unsigned int gen = 0; gen < genNum; gen++)
		{
			// Select parents for crossover
			unsigned int parentsSize = populSize / 2;
			parentsSize = !(parentsSize % 2) ? parentsSize : (parentsSize - 1);	// isEven?
			if (parentsSize < 2)	// if result parentsSize is too small
			{
				throw std::exception("Error: Selection of parents became too small");
			}
			std::vector<Solution> parents(parentsSize);
			select(population, parents, parentsSize);
	
			// Create children by crossed parents
			unsigned int childrenSize = parentsSize;	// even as well
			SolsEntriesList children;
			Solution ch1, ch2;
			for (unsigned int i = 0; i < childrenSize; i+=2)
			{
				crossover(parents[i], parents[i+1], ch1, ch2);
				mutate(ch1);
				mutate(ch2);
				// add children to population
				population.push_back({ch1, fitness(ch1)});	/// insertion boosts the performance
				population.push_back({ch2, fitness(ch2)});	/// insert, conditional to child's max. index in population (fitness), cuts unnecessary, restricts mem. usage
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

}



#if TEST_GENALG

namespace test_genalg
{
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

	void test_getBestEvolved()
	{
		for (unsigned int i = 1; i < 10; i++)
		{
			Solution sol = genalg::getBestEvolved(i * 10, i * 100);
			printf("closest to answer [1,2,3,4,5] solution is (populSize=%u, genNum=%u): %s \n", i * 10, i * 100, toString(sol).c_str());
		}
	}
};

#endif

#endif