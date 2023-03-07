#pragma once
#ifndef _NNETWORK_H_
#define _NNETWORK_H_

#include "Matrix.h"
#include "Layer.h"

/// train (train dataset)
// teacher gives prepared inputs (from dataset) to NN
// NN gets inputs and creates output
// teacher checks the output =>
	// if not ok => ask NN to readjust weights
// give next exercise to NN

/// test (test dataset)
// NN gets inputs and creates output
// human evaluates if NN did a good job or not

class NNetwork
{
	unsigned int inputsNum = 0;
	Layer* inputLayer;
	Layer* layers;
	unsigned int layersNum = 0;
	bool areWeightsReadonly = false;

	public:
	/// <summary>
	/// Constructor of Neural Network that takes inputsNum inputs and initialises layersNum number of layers and interlayer-matrices.
	/// </summary>
	/// <param name="inputsNum">
	/// Number of inputs (size of std::vector(unsigend int) input vector) that NN takes in to calc output. 
	/// </param>
	/// <param name="layersNum">
	/// Number of layers of NN, NOT counting inputLayer(!). So, total num. of layers = 1 + layersNum.
	/// </param>
	NNetwork(const unsigned int inputsNum, const unsigned int layersNum)
		:
		inputsNum(inputsNum),
		layersNum(layersNum)
	{
		inputLayer = new Layer(inputsNum, true);	// use pointer to matrix initialisation function?
		layers = new Layer[layersNum] {};	/// why {Layer(inputsNum)}; does not work ??? bc it initialises only first layer in this way
		for (unsigned int i = 0; i < layersNum; i++)
		{
			layers[i] = Layer(inputsNum);
		}
	}

	Matrix runOnce(const double* input, const unsigned int inputSize)
	{
		if (inputsNum != inputSize)
		{
			throw std::exception("inputsNum must be equal to inputsSize");
		}

		double** inputMat = nullptr;
		allocMem(inputMat, inputsNum, 1);
		for (unsigned int i = 0; i < inputsNum; i++)
		{
			inputMat[i][0] = input[i];
		}
		Matrix x((const double**)inputMat, inputsNum, 1);
		return runOnce(x);
	}

	Matrix runOnce(const Matrix& xColumn)
	{
		if (inputsNum != xColumn.rows())
		{
			throw std::exception("Number of inputs (1-column rows) must be equal to inputsSize specified");
		}
		if (!matop::isPositive(xColumn))
		{
			throw std::invalid_argument("Negative inputs are not yet supported!");
		}

		areWeightsReadonly = true;
		inputLayer->calcMe(xColumn);
		layers[0].calcMe(inputLayer->getValues());
		for (unsigned int i = 1; i < layersNum; i++)
		{
			layers[i].calcMe(layers[i - 1].getValues());
		}
		areWeightsReadonly = false;
		return layers[layersNum - 1].getValues();
	}

	Matrix runOnceNormalized(const Matrix& xColumn)
	{
		if (inputsNum != xColumn.rows())
		{
			throw std::exception("Number of inputs (1-column rows) must be equal to inputsSize specified");
		}
		if (!matop::isPositive(xColumn))
		{
			throw std::invalid_argument("Negative inputs are not yet supported!");
		}

		areWeightsReadonly = true;
		inputLayer->calcMe(xColumn);
		layers[0].calcMeNormalized(inputLayer->getValues());
		for (unsigned int i = 1; i < layersNum; i++)
		{
			layers[i].calcMeNormalized(layers[i - 1].getValues());
		}
		areWeightsReadonly = false;
		return layers[layersNum - 1].getValues();
	}

	Matrix runOnceTanh(const Matrix& xColumn, double steepness = 3.14159)	// pi - just standard value, can be any
	{
		if (inputsNum != xColumn.rows())
		{
			throw std::exception("Number of inputs (1-column rows) must be equal to inputsSize specified");
		}
		if (!matop::isPositive(xColumn))
		{
			throw std::invalid_argument("Negative inputs are not yet supported!");
		}

		areWeightsReadonly = true;
		inputLayer->calcMe(xColumn);
		layers[0].calcMeTanh(inputLayer->getValues(), steepness);
		for (unsigned int i = 1; i < layersNum; i++)
		{
			layers[i].calcMeTanh(layers[i - 1].getValues(), steepness);
		}
		areWeightsReadonly = false;
		return layers[layersNum - 1].getValues();
	}

	void setWeights(const unsigned int layerIndex, const Matrix& newWeightsValues)
	{
		if (layerIndex <= layersNum)
		{
			throw std::invalid_argument("layerIndex is out of range of valid indexes");
		}
		if (areWeightsReadonly)
		{
			throw std::runtime_error("Editing of weights while NN calculates output is not allowed");
		}
		
		layers[layerIndex].setWeightsValues(newWeightsValues);
	}

	void printLastRunData() const
	{
		printf("inputLayer.weights = \n%s\n", inputLayer->getWeights().toString(" ").c_str());
		printf("inputLayer.values = \n%s\n", inputLayer->getValues().toString(" ").c_str());

		printf("layer%u.weights = \n%s\n", 0, layers[0].getWeights().toString(" ").c_str());
		printf("layer%u.values = \n%s\n", 0, layers[0].getValues().toString(" ").c_str());
		for (unsigned int i = 1; i < layersNum; i++)
		{
			printf("layer%u.weights = \n%s\n", i, layers[i].getWeights().toString(" ").c_str());
			printf("layer%u.values = \n%s\n", i, layers[i].getValues().toString(" ").c_str());
		}
		printf("^ last matrix is output \n\n");
	}

};

#endif	// _NNETWORK_H_
