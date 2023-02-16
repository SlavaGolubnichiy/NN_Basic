#pragma once

#ifndef __LAYER_H__
#define __LAYER_H__

#include "../RandomXorsh.h"
#include "../float64.h"
#include "../Matrix.h"



static RandomXorsh random;



class Layer
{
	Matrix* N = nullptr;	// values
	Matrix* w = nullptr;	// weights[thisLayerNeuronIndex][leftLayerNeuronIndex]

	public:
	Layer() {}

	// always set second param to "inputLayer"
	Layer(const char* ignoredInputLayerStr, const unsigned int inputsSize)
	{
		N = new Matrix(inputsSize, 1);
		w = new Matrix(Matrix::getIdentityMatrix(inputsSize, inputsSize));
	}

	Layer(const unsigned int leftLayerNeuronsNum)
	{
		N = new Matrix(leftLayerNeuronsNum, 1);
		w = new Matrix(leftLayerNeuronsNum, leftLayerNeuronsNum);
		randomiseMatrix(*w);
	}

	const Matrix getValues() const
	{
		return *N;
	}

	const Matrix getWeights() const
	{
		return *w;
	}

	void calcMe(const Matrix& leftLayerValues)
	{
		*N = (*w) * leftLayerValues;
	}

	private:
	void randomiseMatrix(Matrix& mat)
	{
		for (unsigned int i = 0; i < mat.rows_; i++)
		{
			for (unsigned int j = 0; j < mat.cols_; j++)
			{
				mat.set(i, j, float64::floor(random.generateDoubleRange01(), 2));
			}
		}
	}

};

#endif
