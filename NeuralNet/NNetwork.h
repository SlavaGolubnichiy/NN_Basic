#pragma once

#include "../Matrix.h"
#include "../RandomXorsh.h"
#include "Layer.h"



/// train
// teacher gives prepared inputs to NN
// NN gets inputs and creates output
// teacher checks the output =>
	// if not ok => ask NN to readjust weights
// give next exercise to NN

/// test
// NN gets inputs and creates output
// human evaluates if NN did a good job or not



class NNetwork
{
	unsigned int inputsNum = 0;
	Layer* inputLayer;
	Layer* layers;
	unsigned int layersNum = 0;

	public:
	NNetwork(const unsigned int inputsNum, const unsigned int layersNum)
		:
		inputsNum(inputsNum),
		layersNum(layersNum)
	{
		inputLayer = new Layer(inputsNum, "inputs");	// use pointer to matrix initialisation function?
		layers = new Layer[layersNum] {};	/// why {LayerV2(inputsNum)}; does not work ???
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
		
		printf("x = \n%s\n", x.toString(" ").c_str());
		
		printf("inputLayer.weights = \n%s\n", inputLayer->getWeights().toString(" ").c_str());
		inputLayer->calcMe(x);
		printf("inputLayer.values = \n%s\n", inputLayer->getValues().toString(" ").c_str());
		
		layers[0].calcMe(inputLayer->getValues());
		printf("layer%u.weights = \n%s\n", 0, layers[0].getWeights().toString(" ").c_str());
		printf("layer%u.values = \n%s\n", 0, layers[0].getValues().toString(" ").c_str());
		
		for (unsigned int i = 1; i < layersNum; i++)
		{
			layers[i].calcMe(layers[i-1].getValues());
			printf("layer%u.weights = \n%s\n", i, layers[i].getWeights().toString(" ").c_str());
			printf("layer%u.values = \n%s\n", i, layers[i].getValues().toString(" ").c_str());
		}

		return layers[layersNum - 1].getValues();
	}

	Matrix runOnce(const Matrix& inputColumnMatrix)
	{
		if (inputsNum != inputColumnMatrix.rows_)
		{
			throw std::exception("Number of inputs (1-column rows) must be equal to inputsSize specified");
		}

		Matrix x(inputColumnMatrix);
		printf("x = \n%s\n", x.toString(" ").c_str());

		inputLayer->calcMe(x);
		printf("inputLayer.weights = \n%s\n", inputLayer->getWeights().toString(" ").c_str());
		printf("inputLayer.values = \n%s\n", inputLayer->getValues().toString(" ").c_str());
		
		layers[0].calcMe(inputLayer->getValues());
		printf("layer%u.weights = \n%s\n", 0, layers[0].getWeights().toString(" ").c_str());
		printf("layer%u.values = \n%s\n", 0, layers[0].getValues().toString(" ").c_str());
		
		for (unsigned int i = 1; i < layersNum; i++)
		{
			layers[i].calcMe(layers[i - 1].getValues());
			printf("layer%u.weights = \n%s\n", i, layers[i].getWeights().toString(" ").c_str());
			printf("layer%u.values = \n%s\n", i, layers[i].getValues().toString(" ").c_str());
		}

		return layers[layersNum - 1].getValues();
	}
};
